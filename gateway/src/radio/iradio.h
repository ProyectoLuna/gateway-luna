#ifndef LUNA_RADIO_IRADIO_H
#define LUNA_RADIO_IRADIO_H

#include <QObject>
#include "protos/nanopb/lunapb.h"

class QString;

namespace luna
{
namespace radio
{

class IRadio
{
public:
    virtual QString getName() = 0;
    virtual QObject *getObject() = 0;
};
} // radio
} // luna
#endif // LUNA_RADIO_IRADIO_H
