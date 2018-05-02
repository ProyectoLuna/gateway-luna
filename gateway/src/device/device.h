#ifndef LUNA_DEVICE_DEVICE_H
#define LUNA_DEVICE_DEVICE_H

#include <QObject>
#include <QSharedPointer>
#include <QJsonObject>

#include "protos/nanopb/lunapb.h"
#include "message/message.h"

namespace luna
{

namespace message
{
class MessageManager;
}

namespace device
{

class Device : public QObject
{
    Q_OBJECT

public:
    Device(QObject *parent = nullptr);
    Device(QSharedPointer<message::Message<RepeatedDevData>> message, QObject *parent = nullptr);
    bool execCommand(SensorCommandType commandId);
    bool update(QSharedPointer<message::Message<RepeatedDevData>> message);

    quint64 static getDeviceIdFromMessage(QSharedPointer<message::Message<RepeatedDevData>> message);

    void setMessageMng(const QSharedPointer<message::MessageManager> messageMng);
    RadioId getRadioId() const;
    quint64 getUniqueId() const;
    quint32 getId() const;
    QHash<SensorUnits, qint32> getSensorData() const;

signals:

private:
    QHash<SensorUnits, qint32> _sensorData;
    RadioId _radioId;
    quint32 _id;
    QSharedPointer<message::MessageManager> _messageMng;
};

}
}
#endif // LUNA_DEVICE_DEVICE_H
