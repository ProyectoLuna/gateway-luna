#include <Logger.h>
#include "messagemanager.h"
#include "protos/nanopb/lunapb.h"
#include "device/device.h"

using namespace luna;
using namespace message;

MessageManager::MessageManager(QObject *parent) : common::ServiceBase (parent)
{
}

bool MessageManager::sendCommand(const device::Device &device, SensorCommandType commandId)
{
    auto commandMsgPtr = generateMessage<RepeatedDevData>(device, commandId);

    if (not commandMsgPtr)
    {
        LOG_WARNING(QString("Error sending command"));
        return false;
    }

    emit sendMessage(commandMsgPtr);
    return true;
}

template<>
QSharedPointer<Message<RepeatedDevData>> MessageManager::generateMessage(const device::Device &device, SensorCommandType commandId)
{
    if (commandId < 0 || commandId > _SensorCommandType_MAX)
    {
        LOG_WARNING(QString("%1 Command not found").arg(commandId));
        return QSharedPointer<Message<RepeatedDevData>>();
    }

    RemoteDevMessage *nanopbMessage = new RemoteDevMessage;
    *nanopbMessage = RemoteDevMessage_init_zero;

    nanopbMessage->header.transaction_id = 0;
    nanopbMessage->header.unique_id.radio_id = device.getRadioId();
    nanopbMessage->header.unique_id.id32 = device.getId();

    RepeatedDevData *repeatedData = new RepeatedDevData;
    RemoteDevData *devData = new RemoteDevData;

    devData->sensor_command.command = commandId;
    devData->sensor_command.data = 0;
    devData->has_sensor_command = true;

    repeatedData->data = devData;
    repeatedData->num = 1;

    nanopbMessage->data.funcs.encode = &encode_repeated_devdata;
    nanopbMessage->data.arg = repeatedData;

    return QSharedPointer<Message<RepeatedDevData>>(new Message<RepeatedDevData>(nanopbMessage));
}
