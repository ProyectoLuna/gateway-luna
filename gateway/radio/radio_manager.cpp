#include <Logger.h>

#include "radiorf24.h"
#include "radio_manager.h"

using namespace luna;

RadioManager::RadioManager(QObject *parent) : QObject(parent)
{
    // Add rf24 to radio list
    QSharedPointer<RadioRF24> radiorf24 =
            QSharedPointer<RadioRF24>(new RadioRF24);
    radioList.append(radiorf24);

    //QSharedPointer<RadioRF24> r = qSharedPointerCast<RadioRF24>(radiorf24);
    QObject::connect(radiorf24.data(), SIGNAL(rxMessage(QString)), this, SLOT(onRxMessage(QString)));

    radiorf24->check_remotes();
}

bool RadioManager::onRxMessage(const QString &message)
{
    LOG_INFO(message);
}
