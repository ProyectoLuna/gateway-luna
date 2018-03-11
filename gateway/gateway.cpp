#include <Logger.h>
#include "radiorf24.h"
#include "radio_manager.h"
#include "gateway.h"
#include "apirest.h"

using namespace luna;

Gateway::Gateway(QObject *parent) : QObject(parent)
{
    _radioManager = new radio::RadioManager(this);
    _apirest = new Apirest(this);
}

bool Gateway::start()
{
    _radioManager->start();
    _apirest->start();
    return true;
}

void Gateway::stop()
{
    LOG_INFO("Stopping gateway...");
    _apirest->stop();
    _radioManager->stop();
    LOG_INFO("Gateway stopped");
    emit stopped();
}
