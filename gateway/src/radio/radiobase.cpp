#include <QString>

#include <Logger.h>
#include "radiobase.h"

using namespace luna;
using namespace radio;

RadioBase::RadioBase(QObject *parent) : QObject(parent)
{
    _name = "RadioBase";
}

QString RadioBase::getName()
{
    return _name;
}

QObject *RadioBase::getObject()
{
    return this;
}

bool RadioBase::start()
{
    LOG_INFO("Starting generic Radio...");
    LOG_INFO("Starting generic Radio");
    return true;
}

void RadioBase::stop()
{
    LOG_INFO("Stopping generic Radio...");
    LOG_INFO("Stopped generic Radio");
}

void RadioBase::quit()
{
    emit finished();
}
