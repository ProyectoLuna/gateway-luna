#include <Logger.h>

#include "gateway.h"
#include "radio/radio_manager.h"
#include "apirest/apirest.h"
#include "message/messagemanager.h"
#include "device/devicemanager.h"

using namespace luna;
using namespace gateway;

Gateway::Gateway(QObject *parent) : QObject(parent)
{
    _radioManager = QSharedPointer<radio::RadioManager>(new radio::RadioManager());
    _apirest = QSharedPointer<apirest::Apirest>(new apirest::Apirest());
    _messageManager = QSharedPointer<message::MessageManager>(new message::MessageManager());
    _deviceManager = QSharedPointer<device::DeviceManager>(new device::DeviceManager());

    _deviceManager->setMessageMng(_messageManager);
    _radioManager->setDeviceManager(_deviceManager);
    _apirest->setDeviceManager(_deviceManager);
    QObject::connect(_messageManager.data(), &message::MessageManager::sendMessage, _radioManager.data(), &radio::RadioManager::onTxMessage);
}

bool Gateway::start()
{
    bool ret = _radioManager->start();
    ret &= _apirest->start();
    ret &= _messageManager->start();
    ret &= _deviceManager->start();

    return ret;
}

void Gateway::stop()
{
    LOG_INFO("Stopping gateway...");

    _deviceManager->stop();
    _messageManager->stop();
    _apirest->stop();
    _radioManager->stop();

    LOG_INFO("Gateway stopped");

    emit stopped();
}
