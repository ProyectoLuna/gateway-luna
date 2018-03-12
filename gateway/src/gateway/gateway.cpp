#include "gateway.h"
#include <Logger.h>

using namespace luna;
using namespace gateway;

Gateway::Gateway(QObject *parent) : QObject(parent)
{
    _radioManager = new radio::RadioManager(this);
    _apirest = new apirest::Apirest(this);
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
