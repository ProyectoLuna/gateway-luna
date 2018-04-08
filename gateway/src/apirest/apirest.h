#ifndef LUNA_APIREST_APIREST_H
#define LUNA_APIREST_APIREST_H

#include <QObject>
#include <qhttpserver.hpp>
#include <qhttpserverresponse.hpp>
#include <qhttpserverrequest.hpp>

#include "device/devicemanager.h"
#include "common/servicebase.h"

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

private:
    qhttp::server::QHttpServer* _server;
    QSharedPointer<device::DeviceManager> _deviceManager;
};

}
}
#endif // LUNA_APIREST_APIREST_H
