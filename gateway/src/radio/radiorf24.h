#ifndef LUNA_RADIO_RADIO_RF24_H
#define LUNA_RADIO_RADIO_RF24_H

#include <QObject>
#include <QHash>

#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>
#include <RF24Mesh/RF24Mesh.h>

#include "radiobase.h"

namespace luna
{
namespace radio
{

class RadioRF24 : public RadioBase
{
    Q_OBJECT

public:
    RadioRF24(QObject* parent = nullptr);

private:
    bool _gameover;
//    QHash<RemoteDevHeader, RF24NetworkHeader> _deviceTable;

signals:

public slots:
    bool start();
    void stop();
    void quit();
};

} // radio
} // luna
#endif // LUNA_RADIO_RADIO_RF24_H
