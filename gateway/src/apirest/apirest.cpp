#include <QJsonObject>
#include <QJsonDocument>

#include <qhttpserver.hpp>
#include <qhttpserverresponse.hpp>
#include <qhttpserverrequest.hpp>
#include <Logger.h>

#include "device/device.h"
#include "protos/nanopb/lunapb.h"
#include "apirest.h"
#include "message/messagemanager.h"

using namespace luna;
using namespace apirest;

Apirest::Apirest(QObject *parent) : common::ServiceBase(parent)
{
    _status = common::ServiceBase::Status::STOPPED;
    _name = "ApiTest";
    _server = new qhttp::server::QHttpServer(this);

}

bool Apirest::start()
{
    if (_status == common::ServiceBase::Status::STARTED)
    {
        LOG_WARNING("ApiRest is already started");
        return false;
    }

    // listening tcp port or Unix path
    _server->listen(QHostAddress::Any, 8080, [&](qhttp::server::QHttpRequest* req, qhttp::server::QHttpResponse* res) {
        req->collectData();

        const auto& h = req->headers();
        // optionally let the clients to shut down the server

        // just for fun! print meta information:
        qDebug("\n--> %s : %s",
                qhttp::Stringify::toString(req->method()),
                qPrintable(req->url().toString().toUtf8())
              );

        QJsonObject jsonObject = this->handleRequest(req->url().toString());
        QByteArray jsonResp = serializeJson(jsonObject);

        if (jsonResp.isEmpty())
        {
            LOG_INFO("Error serializating json response");
        }

        qDebug("[Headers (%d)]", h.size());
        h.forEach([](auto iter) {
            qDebug(" %s : %s",
                    iter.key().constData(),
                    iter.value().constData()
                  );
        });

        req->onEnd([req, res, jsonResp](){
            res->setStatusCode(qhttp::ESTATUS_OK); // status 200
            res->addHeader("connection", "close"); // optional(default) header

            int size = req->collectedData().size();
            auto message = [size, jsonResp]() -> QByteArray {
                return jsonResp;
            };

            res->end(message());  // reponse body data
        });

    });

    if (not _server->isListening()) {
        LOG_ERROR(QString("Apirest failed to listen, %1").arg(strerror(errno)));
        return false;
    }

    _status = common::ServiceBase::Status::STARTED;
    LOG_INFO("ApiRest started");
    return true;
}

void Apirest::stop()
{
    if (_status == common::ServiceBase::Status::STOPPED)
    {
        LOG_WARNING("Apirest is already stopped");
        return;
    }

    _status = common::ServiceBase::Status::STOPPED;

    LOG_INFO("ApiRest stopped");
}

QJsonObject Apirest::handleRequest(const QString &url)
{
    // /gateway/device/{devid}/command/toggle
    // /gateway/device/{devid}/status
    // /gateway/device/list
    QJsonObject jsonObjectMain;
    QJsonObject jsonObjectResp;

    jsonObjectResp["result"] = false;

    if (not checkUrl(url))
    {
        QString retString = QString("Bad URL format: %1").arg(url);
        jsonObjectResp["error"] = retString;
        jsonObjectMain["application/json"] = jsonObjectResp;
        return jsonObjectMain;
    }

    QStringList urlList = url.split("/");
    if (urlList[3] == "list")
    {
        int deviceCount = 0;
        for (auto device : _deviceManager->getDevices())
        {
            if (not device)
            {
                continue;
            }
            QJsonObject jsonObjectDevice;
            jsonObjectDevice["id"] = QString::number(device->getUniqueId(), 16);
            QHashIterator<SensorUnits, qint32> i(device->getSensorData());
            while (i.hasNext())
            {
                i.next();
                jsonObjectDevice[sensorUnitsTranslator[i.key()]] = i.value();
            }

            jsonObjectResp[QString("device%1").arg(deviceCount)] = jsonObjectDevice;
        }
        jsonObjectResp["result"] = true;
        jsonObjectMain["application/json"] = jsonObjectResp;
        return jsonObjectMain;
    }

    bool ok;
    quint64 devid = urlList[3].toULongLong(&ok, 16);
    if (not ok)
    {
        QString retString = QString("Bad Device Id: %1").arg(urlList[3]);
        jsonObjectResp["error"] = retString;
        jsonObjectMain["application/json"] = jsonObjectResp;
        return jsonObjectMain;
    }

    QString typeAction = urlList[4];
    if (typeAction == "command")
    {
        QString command = urlList[5];

        if (command == "toggle")
        {
            if (not _deviceManager->execCommand(devid, SensorCommandType_SCT_TOGGLE))
            {
                QString retString = QString("Error executing %1 command in device 0x%2").arg(command).arg(QString::number(devid, 16));
                jsonObjectResp["error"] = retString;
                jsonObjectMain["application/json"] = jsonObjectResp;
                return jsonObjectMain;
            }
            jsonObjectResp["result"] = true;
            jsonObjectMain["application/json"] = jsonObjectResp;
            return jsonObjectMain;
        }
        else
        {
            QString retString = QString("Comamnd %1 not found").arg(command);
            jsonObjectResp["error"] = retString;
            jsonObjectMain["application/json"] = jsonObjectResp;
            return jsonObjectMain;
        }
    }
    else if (typeAction == "status")
    {
        QJsonObject jsonObjectDevice;
        auto device = _deviceManager->getDevice(devid);
        if (not device)
        {
            QString retString = QString("Device 0x%1 not found").arg(QString::number(devid, 16));
            jsonObjectResp["error"] = retString;
            jsonObjectMain["application/json"] = jsonObjectResp;
            return jsonObjectMain;
        }
        jsonObjectDevice["id"] = QString::number(device->getUniqueId(), 16);
        QHashIterator<SensorUnits, qint32> i(device->getSensorData());
        while (i.hasNext())
        {
            i.next();
            jsonObjectDevice[sensorUnitsTranslator[i.key()]] = i.value();
        }

        jsonObjectResp["status"] = jsonObjectDevice;
        jsonObjectResp["result"] = true;
        jsonObjectMain["application/json"] = jsonObjectResp;
        return jsonObjectMain;
    }
    else
    {
        QString retString = QString("Type of action %1 not found").arg(typeAction);
        jsonObjectResp["error"] = retString;
        jsonObjectMain["application/json"] = jsonObjectResp;
        return jsonObjectMain;
    }
}

QByteArray Apirest::serializeJson(const QJsonObject &jsonObj)
{
    QJsonDocument jsonDoc = QJsonDocument(jsonObj);
    if (jsonDoc.isEmpty())
    {
        return QByteArray();
    }

    return jsonDoc.toJson();
}

bool Apirest::checkUrl(const QString &url)
{
    QStringList urlList = url.split("/");
    QString keyValue = urlList[1];
    QString subModule = urlList[2];
    if (keyValue != "gateway" ||
        subModule != "device")
    {
        return false;
    }
    return true;
}

void Apirest::setDeviceManager(QSharedPointer<device::DeviceManager> deviceManager)
{
    _deviceManager = deviceManager;
}
