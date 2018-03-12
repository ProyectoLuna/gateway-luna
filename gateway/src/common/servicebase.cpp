#include <Logger.h>
#include "servicebase.h"

using namespace luna;
using namespace common;

ServiceBase::ServiceBase(QObject *parent) : QObject(parent)
{
    _name = "ServiceBase";
    _status = ServiceBase::Status::STOPPED;
}

QString ServiceBase::getName()
{
    return _name;
}

QObject *ServiceBase::getObject()
{
    return this;
}

bool ServiceBase::start()
{
    if (_status == ServiceBase::Status::STARTED)
    {
        LOG_WARNING("ServiceBase is already started");
        return false;
    }
    return true;
}

void ServiceBase::stop()
{
    if (_status == ServiceBase::Status::STOPPED)
    {
        LOG_WARNING("ServiceBase is already stopped");
        return;
    }
}
