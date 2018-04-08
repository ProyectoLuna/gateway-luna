#ifndef LUNA_RADIO_RADIOBASE_H
#define LUNA_RADIO_RADIOBASE_H

#include <QObject>
#include <QSharedPointer>
#include "iradio.h"
#include "protos/nanopb/lunapb.h"
#include "message/message.h"

namespace luna
{
namespace radio
{

class RadioBase : public QObject, public virtual IRadio
{
    Q_OBJECT

public:
    RadioBase(QObject *parent = nullptr);
    QString getName() override;
    QObject *getObject() override;
    RadioId getRadioId() const;

protected:
    QString _name;
    RadioId _radioId;

signals:
    bool rxMessage(RemoteDevMessage* message);
    void finished();

public slots:
    virtual bool start();
    virtual void stop();
    virtual void quit();
    virtual bool send(QSharedPointer<message::Message<RepeatedSensorCommand>> message);
};

}
}

#endif // LUNA_RADIO_RADIOBASE_H
