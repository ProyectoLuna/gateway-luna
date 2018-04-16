#ifndef LUNA_APIREST_APIREST_H
#define LUNA_APIREST_APIREST_H

#include <QObject>
#include <qhttpserver.hpp>
#include <qhttpserverresponse.hpp>
#include <qhttpserverrequest.hpp>

#include "device/devicemanager.h"
#include "common/servicebase.h"

class QJsonObject;

namespace luna
{
namespace apirest
{

class Apirest : public common::ServiceBase
{
    Q_OBJECT
public:
    Apirest(QObject *parent = nullptr);

    void setDeviceManager(QSharedPointer<device::DeviceManager> deviceManager);

signals:

public slots:
    bool start();
    void stop();

protected:
    bool checkUrl(const QString &url);
    QJsonObject handleRequest(const QString &url);
    static QByteArray serializeJson(const QJsonObject &jsonObj);

private:
    qhttp::server::QHttpServer* _server;
    QSharedPointer<device::DeviceManager> _deviceManager;
};

}
}
#endif // LUNA_APIREST_APIREST_H
