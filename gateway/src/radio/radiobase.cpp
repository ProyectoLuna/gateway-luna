#include <QString>

#include <Logger.h>
#include "radiobase.h"
#include "message/message.h"
#include "protos/nanopb/lunapb.h"

using namespace luna;
using namespace radio;

RadioBase::RadioBase(QObject *parent) : QObject(parent)
{
    _name = "RadioBase";
    _radioId = RadioId_RID_UNKNOWN;
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

bool RadioBase::send(QSharedPointer<message::Message<RepeatedSensorCommand>> message)
{
    Q_UNUSED(message);
    LOG_INFO("Generic send");
    return true;
}

RadioId RadioBase::getRadioId() const
{
    return _radioId;
}
