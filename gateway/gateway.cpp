#include <Logger.h>
#include "radiorf24.h"
#include "radio_manager.h"
#include "gateway.h"

using namespace luna;

Gateway::Gateway(QObject *parent) : QObject(parent)
{
    _radioManager = new radio::RadioManager(this);
}

bool Gateway::start()
{
    _radioManager->start();
    return true;
}

void Gateway::stop()
{
    LOG_INFO("Stopping gateway...");
    _radioManager->stop();
    emit stopped();
    LOG_INFO("Gateway stopped");
}
