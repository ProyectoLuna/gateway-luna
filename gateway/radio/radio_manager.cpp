#include <Logger.h>

#include "radiorf24.h"
#include "radio_manager.h"

using namespace luna;

RadioManager::RadioManager(QObject *parent) : QObject(parent)
{
    // Add rf24 to radio list
    QSharedPointer<IRadio> radiorf24 =
            QSharedPointer<IRadio>(new RadioRF24);
    radioList.append(radiorf24);

    //QSharedPointer<RadioRF24> r = qSharedPointerCast<RadioRF24>(radiorf24);
    QObject::connect(radiorf24.data(), SIGNAL(rxMessage(QString)), this, SLOT(onRxMessage(QString)));
}

bool RadioManager::onRxMessage(const QString &message)
{
    LOG_INFO(message);
}
