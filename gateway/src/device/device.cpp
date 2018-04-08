#include <Logger.h>
#include "device.h"
#include "protos/nanopb/lunapb.h"
#include "message/messagemanager.h"

using namespace luna;
using namespace device;

static QHash<SensorUnits, QString> sensorUnitsTranslator =
{
    {SensorUnits::SensorUnits_SU_UNKNOWN, "Unknow"},
    {SensorUnits::SensorUnits_SU_MAH, "mAh"},
    {SensorUnits::SensorUnits_SU_CELSIUS_DEGREE, "ºC"},
    {SensorUnits::SensorUnits_SU_FAHRENHEIT_DEGREE, "ºF"},
    {SensorUnits::SensorUnits_SU_KELVIN_DEGREE, "K"},
    {SensorUnits::SensorUnits_SU_BOOL, "Bool"},
    {SensorUnits::SensorUnits_SU_RELAYSTATUS, "Relay status"}
};

Device::Device(QObject *parent) : QObject(parent)
{

}

Device::Device(QSharedPointer<message::Message<RepeatedSensorData>> message, QObject *parent) : QObject(parent)
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

bool Device::update(QSharedPointer<message::Message<RepeatedSensorData>> message)
{
    RemoteDevMessage *nanopb = message->getProto();
    RepeatedSensorData *sensorData = static_cast<RepeatedSensorData*>(nanopb->data.arg);

    _radioId = nanopb->header.unique_id.radio_id;
    _id = nanopb->header.unique_id.id32;

    for (int i = 0; i < sensorData->num; ++i)
    {
        _sensorData[sensorData->data[i].unit] = sensorData->data[i].value;
    }

    return true;
}

quint64 Device::getDeviceIdFromMessage(QSharedPointer<message::Message<RepeatedSensorData>> message)
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

void Device::setMessageMng(const QSharedPointer<message::MessageManager> messageMng)
{
    _messageMng = messageMng;
}
