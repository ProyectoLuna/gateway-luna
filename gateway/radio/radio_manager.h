#ifndef RADIO_MANAGER_H
#define RADIO_MANAGER_H

#include <QObject>
#include <QList>
#include <QSharedPointer>

#include "iradio.h"

namespace luna
{
namespace radio
{

class RadioManager : public QObject
{
    Q_OBJECT

public:
    RadioManager(QObject* parent = nullptr);
    bool start();

private:
    enum class Status
    {
        RM_STOPPED,
        RM_STARTED
    };

    QList<QSharedPointer<IRadio>> _radioList;
    QList<QSharedPointer<QThread>> _radioThreadList;
    std::atomic<Status> _status;

public slots:
    bool onRxMessage(const QString &message);
    void stop();
};

} // radio
} // luna
#endif // RADIO_MANAGER_H
