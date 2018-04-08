#include <QHash>
#include <QSharedPointer>
#include <QMutex>

#include <Logger.h>

#include "common/servicebase.h"
#include "devicemanager.h"
#include "message/messagemanager.h"
#include "protos/nanopb/lunapb.h"
#include "device.h"

using namespace luna;
using namespace device;

DeviceManager::DeviceManager(QObject *parent) : common::ServiceBase(parent), _mutex(QMutex::Recursive)
{
    _status = common::ServiceBase::Status::STOPPED;
    _name = "DeviceManager";
}

bool DeviceManager::updateDevice(QSharedPointer<message::Message<RepeatedSensorData>> message)
{
    quint64 id64 = Device::getDeviceIdFromMessage(message);

    if (not _deviceHash.contains(id64))
    {
        QSharedPointer<Device> device = QSharedPointer<Device>(new Device(message));
        _deviceHash[device->getUniqueId()] = device;

        device->setMessageMng(_messageMng);
    }
    else
    {
        QSharedPointer<Device> device = _deviceHash[id64];
        device->update(message);
    }

    //QMutexLocker locker(&_mutex);
    //locker.unlock();

    return true;
}

bool DeviceManager::existsDevice(quint64 deviceId)
{
    return _deviceHash.contains(deviceId);
}

bool DeviceManager::execCommand(quint64 deviceId, SensorCommandType commandId)
{
    if (not _deviceHash.contains(deviceId))
    {
        LOG_WARNING(QString("Device 0x%1 not found").arg(deviceId, 1, 16));
        return false;
    }

    QSharedPointer<Device> device = _deviceHash[deviceId];

    if (not device->execCommand(commandId))
    {
        LOG_WARNING(QString("Error executing %1 command in device 0x%2")
                    .arg(commandId)
                    .arg(deviceId, 1, 16));
        return false;
    }
    return true;
}

void DeviceManager::setMessageMng(const QSharedPointer<message::MessageManager> messageMng)
{
    _messageMng = messageMng;
}
