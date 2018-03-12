#include <qhttpserver.hpp>
#include <qhttpserverresponse.hpp>
#include <qhttpserverrequest.hpp>
#include <Logger.h>

#include "apirest.h"

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
    _server->listen(QHostAddress::Any, 8080, [](qhttp::server::QHttpRequest* req, qhttp::server::QHttpResponse* res) {
        req->collectData();

        req->onEnd([req, res](){
            res->setStatusCode(qhttp::ESTATUS_OK); // status 200
            res->addHeader("connection", "close"); // optional(default) header

            int size = req->collectedData().size();
            auto message = [size]() -> QByteArray {
                if ( size == 0 )
                    return "Hello World!\n";

                char buffer[65] = {0};
                qsnprintf(buffer, 64, "Hello!\nyou've sent me %d bytes!\n", size);
                return buffer;
            };

            res->end(message());  // reponse body data
        });

        const auto& h = req->headers();
        // optionally let the clients to shut down the server

        // just for fun! print meta information:
        qDebug("\n--> %s : %s",
                qhttp::Stringify::toString(req->method()),
                qPrintable(req->url().toString().toUtf8())
              );
        qDebug("[Headers (%d)]", h.size());
        h.forEach([](auto iter) {
            qDebug(" %s : %s",
                    iter.key().constData(),
                    iter.value().constData()
                  );
        });
    });

    if (not _server->isListening()) {
        LOG_ERROR("Apirest failed to listen");
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
