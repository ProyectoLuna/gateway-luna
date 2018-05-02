#ifndef LUNA_MESSAGE_MESSAGEMANAGER_H
#define LUNA_MESSAGE_MESSAGEMANAGER_H

#include <QObject>
#include <QSharedPointer>
#include "message.h"
#include "common/servicebase.h"
#include "protos/nanopb/lunapb.h"

static QHash<SensorUnits, QString> sensorUnitsTranslator =
{
    {SensorUnits::SensorUnits_SU_UNKNOWN, "Unknow"},
    {SensorUnits::SensorUnits_SU_MA, "mAh"},
    {SensorUnits::SensorUnits_SU_CELSIUS_DEGREE, "ºC"},
    {SensorUnits::SensorUnits_SU_FAHRENHEIT_DEGREE, "ºF"},
    {SensorUnits::SensorUnits_SU_KELVIN_DEGREE, "K"},
    {SensorUnits::SensorUnits_SU_BOOL, "Bool"},
    {SensorUnits::SensorUnits_SU_RELAYSTATUS, "Relay status"}
};

namespace luna
{

namespace device
{
class Device;
}

namespace message
{

class MessageManager : public common::ServiceBase
{
    Q_OBJECT

public:
    MessageManager(QObject *parent = nullptr);
    template<class T>
    static QSharedPointer<Message<T>> generateMessage(const device::Device &device, SensorCommandType commandId);
    bool sendCommand(const device::Device &device, SensorCommandType commandId);

signals:
    void sendMessage(QSharedPointer<message::Message<RepeatedDevData>> message);
};

template<class T>
QSharedPointer<Message<T>> MessageManager::generateMessage(const device::Device &device, SensorCommandType commandId)
{
    Q_UNUSED(device);
    Q_UNUSED(commandId);
    return QSharedPointer<Message<T>>();
}

template<>
QSharedPointer<Message<RepeatedDevData>> MessageManager::generateMessage(const device::Device &device, SensorCommandType commandId);

}
}
#endif // LUNA_MESSAGE_MESSAGEMANAGER_H
