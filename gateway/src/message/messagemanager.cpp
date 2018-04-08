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
    auto commandMsgPtr = generateMessage<RepeatedSensorCommand>(device, commandId);

    if (not commandMsgPtr)
    {
        LOG_WARNING(QString("Error sending command"));
        return false;
    }

    emit sendMessage(commandMsgPtr);
    return true;
}

template<>
QSharedPointer<Message<RepeatedSensorCommand>> MessageManager::generateMessage(const device::Device &device, SensorCommandType commandId)
{
    if (commandId < 0 || commandId > SensorCommandType_SCT_TOTAL_NUMBER)
    {
        LOG_WARNING(QString("%1 Command not found").arg(commandId));
        return QSharedPointer<Message<RepeatedSensorCommand>>();
    }

    RemoteDevMessage *nanopbMessage = new RemoteDevMessage;
    *nanopbMessage = RemoteDevMessage_init_zero;

    nanopbMessage->header.transaction_id = 0;
    nanopbMessage->header.unique_id.radio_id = device.getRadioId();
    nanopbMessage->header.unique_id.id32 = device.getId();

    RepeatedSensorCommand *repeatedCommand = new RepeatedSensorCommand;
    SensorCommand *command = new SensorCommand;

    command->command = commandId;

    repeatedCommand->data = command;
    repeatedCommand->num = 1;

    nanopbMessage->data.funcs.encode = &encode_repeated_sensorcommand;
    nanopbMessage->data.arg = repeatedCommand;

    return QSharedPointer<Message<RepeatedSensorCommand>>(new Message<RepeatedSensorCommand>(nanopbMessage));
}
