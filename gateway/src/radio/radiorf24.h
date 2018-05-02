#ifndef LUNA_RADIO_RADIO_RF24_H
#define LUNA_RADIO_RADIO_RF24_H

#include <QObject>
#include <QHash>
#include <QSharedPointer>

#include <RF24Network/RF24Network.h>
#include "radiobase.h"

struct RF24NetworkHeader;

namespace luna
{
namespace radio
{

class RadioRF24 : public RadioBase
{
    Q_OBJECT

public:
    RadioRF24(QObject* parent = nullptr);

signals:

public slots:
    bool start() override;
    void stop() override;
    void quit() override;
    bool send(QSharedPointer<message::Message<RepeatedDevData>> message) override;

private:
    bool _gameover;
    QHash<quint32, RF24NetworkHeader> _deviceTable;
};

} // radio
} // luna
#endif // LUNA_RADIO_RADIO_RF24_H
