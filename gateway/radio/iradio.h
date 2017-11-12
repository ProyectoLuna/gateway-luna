#ifndef IRADIO_H
#define IRADIO_H

#include <QObject>

namespace luna
{
class IRadio : public QObject
{
    Q_OBJECT

public:
    virtual ~IRadio(){}
    virtual bool dummy() = 0;
signals:
    bool rxMessage(const QString &message);
};
}
#endif // RADIO_MANAGER_H
