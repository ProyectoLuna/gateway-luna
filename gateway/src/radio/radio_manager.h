#ifndef LUNA_RADIO_RADIO_MANAGER_H
#define LUNA_RADIO_RADIO_MANAGER_H

#include <QObject>
#include <QList>
#include <QSharedPointer>

#include "common/servicebase.h"
#include "protos/nanopb/lunapb.h"
#include "message/message.h"

namespace luna
{

namespace device
{
class DeviceManager;
}

namespace radio
{

class RadioBase;

class RadioManager : public common::ServiceBase
{
    Q_OBJECT

public:
    RadioManager(QObject* parent = nullptr);
    void setDeviceManager(QSharedPointer<device::DeviceManager> deviceManager);

public slots:
    bool onRxMessage(RemoteDevMessage* rawMessage);
    bool onTxMessage(QSharedPointer<message::Message<RepeatedDevData>> message);
    bool start();
    void stop();

private:
    QList<QSharedPointer<RadioBase>> _radioList;
    QList<QSharedPointer<QThread>> _radioThreadList;
    QSharedPointer<device::DeviceManager> _deviceManager;
};

} // radio
} // luna
#endif // LUNA_RADIO_RADIO_MANAGER_H
