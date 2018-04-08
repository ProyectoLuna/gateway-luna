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
        LOG_WARNING(QString("%1 is already started").arg(_name));
        return false;
    }

    _status = ServiceBase::Status::STARTED;
    LOG_INFO(QString("%1 started").arg(_name));
    return true;
}

void ServiceBase::stop()
{
    if (_status == ServiceBase::Status::STOPPED)
    {
        LOG_WARNING(QString("%1 is already stopped").arg(_name));
        return;
    }

    _status = ServiceBase::Status::STOPPED;
    LOG_INFO(QString("%1 stopped").arg(_name));
}
