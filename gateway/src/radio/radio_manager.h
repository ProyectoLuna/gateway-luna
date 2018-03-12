#ifndef LUNA_RADIO_RADIO_MANAGER_H
#define LUNA_RADIO_RADIO_MANAGER_H

#include <QObject>
#include <QList>
#include <QSharedPointer>

#include "iradio.h"
#include "common/servicebase.h"

namespace luna
{
namespace radio
{

class RadioManager : public common::ServiceBase
{
    Q_OBJECT

public:
    RadioManager(QObject* parent = nullptr);

private:
    QList<QSharedPointer<IRadio>> _radioList;
    QList<QSharedPointer<QThread>> _radioThreadList;

public slots:
    bool onRxMessage(const QString &message);
    bool start();
    void stop();
};

} // radio
} // luna
#endif // LUNA_RADIO_RADIO_MANAGER_H
