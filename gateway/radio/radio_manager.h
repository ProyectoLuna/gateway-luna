#ifndef RADIO_MANAGER_H
#define RADIO_MANAGER_H

#include <QObject>
#include <QList>
#include <QSharedPointer>

#include "iradio.h"
#include "servicebase.h"

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
#endif // RADIO_MANAGER_H
