#include <Logger.h>
#include "device.h"
#include "protos/nanopb/lunapb.h"
#include "message/messagemanager.h"

using namespace luna;
using namespace device;

Device::Device(QObject *parent) : QObject(parent)
{

}

Device::Device(QSharedPointer<message::Message<RepeatedDevData>> message, QObject *parent) : QObject(parent)
{
    update(message);
}

bool Device::execCommand(SensorCommandType commandId)
{
    bool ret = _messageMng->sendCommand(*this, commandId);

    if (not ret)
    {
        LOG_WARNING(QString("Error sending command"));
        return false;
    }

    return true;
}

bool Device::update(QSharedPointer<message::Message<RepeatedDevData>> message)
{
    RemoteDevMessage *nanopb = message->getProto();
    RepeatedDevData *devData = static_cast<RepeatedDevData*>(nanopb->data.arg);

    _radioId = nanopb->header.unique_id.radio_id;
    _id = nanopb->header.unique_id.id32;

    for (int i = 0; i < devData->num; ++i)
    {
        if (not devData->data[i].has_sensor_data)
        {
            continue;
        }
        _sensorData[devData->data[i].sensor_data.unit] = devData->data[i].sensor_data.value;
    }

    return true;
}

quint64 Device::getDeviceIdFromMessage(QSharedPointer<message::Message<RepeatedDevData> > message)
{
    RemoteDevMessage *nanopb = message->getProto();

    RadioId radioId = nanopb->header.unique_id.radio_id;
    quint32 devId = nanopb->header.unique_id.id32;
    quint64 id64 = radioId;
    id64 = id64 << 32;
    id64 |= devId;

    return id64;
}

RadioId Device::getRadioId() const
{
    return _radioId;
}

quint64 Device::getUniqueId() const
{
    quint64 id64 = _radioId;
    id64 = id64 << 32;
    id64 |= _id;

    return id64;
}

quint32 Device::getId() const
{
    return _id;
}

QHash<SensorUnits, qint32> Device::getSensorData() const
{
    return _sensorData;
}

void Device::setMessageMng(const QSharedPointer<message::MessageManager> messageMng)
{
    _messageMng = messageMng;
}
