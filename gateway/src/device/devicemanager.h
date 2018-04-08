#ifndef LUNA_DEVICE_DEVICEMANAGER_H
#define LUNA_DEVICE_DEVICEMANAGER_H

#include <QObject>
#include <QHash>
#include <QSharedPointer>
#include <QMutex>
#include "common/servicebase.h"
#include "protos/nanopb/lunapb.h"
#include "message/message.h"

namespace luna
{

namespace message
{
class MessageManager;
}

namespace device
{

class Device;

class DeviceManager : public common::ServiceBase
{
    Q_OBJECT

public:
    DeviceManager(QObject *parent = nullptr);
    bool updateDevice(QSharedPointer<message::Message<RepeatedSensorData>> message);
    bool existsDevice(quint64 deviceId);

    void setMessageMng(const QSharedPointer<message::MessageManager> messageMng);

signals:

public slots:
    bool execCommand(quint64 deviceId, SensorCommandType commandId);

private:
    QHash<quint64, QSharedPointer<Device>> _deviceHash;
    mutable QMutex _mutex;
    QSharedPointer<message::MessageManager> _messageMng;
};

} // device
} // luna
#endif // LUNA_DEVICE_DEVICEMANAGER_H
