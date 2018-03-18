#ifndef LUNA_RADIO_RADIOBASE_H
#define LUNA_RADIO_RADIOBASE_H

#include <QObject>
#include "iradio.h"

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

protected:
    QString _name;

signals:
    bool rxMessage(RemoteDevMessage* message);
    void finished();

public slots:
    bool start();
    void stop();
    void quit();
};

}
}

#endif // LUNA_RADIO_RADIOBASE_H
