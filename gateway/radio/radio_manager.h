#ifndef RADIO_MANAGER_H
#define RADIO_MANAGER_H

#include <QObject>
#include <QList>
#include <QSharedPointer>

#include "iradio.h"

namespace luna
{
class RadioManager : public QObject
{
    Q_OBJECT

public:
    RadioManager(QObject* parent = nullptr);

private:
    QList<QSharedPointer<IRadio>> radioList;

public slots:
    bool onRxMessage(const QString &message);
};
}
#endif // RADIO_MANAGER_H
