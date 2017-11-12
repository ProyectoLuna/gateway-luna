#ifndef RADIO_RF24_H
#define RADIO_RF24_H

#include <QObject>

#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>
#include <RF24Mesh/RF24Mesh.h>

#include "iradio.h"

namespace luna
{
class RadioRF24 : public IRadio
{
    Q_OBJECT

private:
public:
    RadioRF24(QObject* parent = nullptr);
    virtual ~RadioRF24();
    int check_remotes(void);
    bool dummy() override;
signals:
};
}
#endif // RADIO_RF24_H
