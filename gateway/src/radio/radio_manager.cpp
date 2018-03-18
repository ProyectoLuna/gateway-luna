#include <QThread>
#include <Logger.h>
#include <QSharedPointer>

#include "radiorf24.h"
#include "radio_manager.h"
#include "message/message.h"
#include "message/messagemanager.h"
#include "protos/nanopb/lunapb.h"

using namespace luna;
using namespace radio;

RadioManager::RadioManager(QObject *parent) : common::ServiceBase(parent)
{
    qRegisterMetaType<RemoteDevMessage*>("RemoteDevMessage*");

    _status = common::ServiceBase::Status::STOPPED;
    _name = "RadioManager";

    // Add rf24 to radio list
    QSharedPointer<RadioRF24> radiorf24 =
            QSharedPointer<RadioRF24>(new RadioRF24);
    _radioList.append(radiorf24);
}

bool RadioManager::start()
{
    if (_status == common::ServiceBase::Status::STARTED)
    {
        LOG_WARNING("Radio manager is already started");
        return false;
    }

    for (auto &radio: _radioList)
    {
        QSharedPointer<QThread> radioThread = QSharedPointer<QThread>(new QThread());
        radioThread->setObjectName(radio->getName());

        _radioThreadList.append(radioThread);

        radio->getObject()->moveToThread(radioThread.data());

        QObject::connect(radioThread.data(), SIGNAL(started()),  radio->getObject(), SLOT(start()));
        QObject::connect(radio->getObject(), SIGNAL(finished()), radioThread.data(), SLOT(quit()));
        QObject::connect(radio->getObject(), SIGNAL(finished()), radio->getObject(), SLOT(deleteLater()));
        QObject::connect(radioThread.data(), SIGNAL(finished()), radioThread.data(), SLOT(deleteLater()));

        QObject::connect(radio->getObject(), SIGNAL(rxMessage(RemoteDevMessage*)), this, SLOT(onRxMessage(RemoteDevMessage*)));
    }

    for (const auto &radio : _radioThreadList)
    {
        radio->start();
    }

    _status = common::ServiceBase::Status::STARTED;

    LOG_INFO("Radio manager started");
    return true;
}

void RadioManager::stop()
{
    if (_status == common::ServiceBase::Status::STOPPED)
    {
        LOG_WARNING("Radio manages is already stopped");
        return;
    }

    LOG_INFO("Stopping radio manager...");
    for (int i = _radioList.size() - 1; i >= 0; --i)
    {
        _radioList[i]->stop();
    }

    for (auto &radioThread : _radioThreadList)
    {
        radioThread->terminate();
        radioThread->wait();
        _radioThreadList.removeOne(radioThread);
    }

    for (int i = _radioList.size() - 1; i >= 0; --i)
    {
        _radioList[i]->quit();
    }

    _status = common::ServiceBase::Status::STOPPED;

    LOG_INFO("Radio manager stopped");
}

bool RadioManager::onRxMessage(RemoteDevMessage *rawMessage)
{
    message::Message<RemoteDevMessage> message(rawMessage);
    LOG_DEBUG(QString("ID: %1, radioID: %2, transaction: %3")
              .arg(rawMessage->header.unique_id.id32)
              .arg(rawMessage->header.unique_id.radio_id)
              .arg(rawMessage->header.transaction_id)
              );

    return true;
}

