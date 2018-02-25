#ifndef IRADIO_H
#define IRADIO_H

#include <QObject>
#include <QString>

namespace luna
{
namespace radio
{

class IRadio : public QObject
{
    Q_OBJECT

public:
    virtual ~IRadio(){}
    virtual QString getName() = 0;
    virtual QObject *getObject() = 0;

signals:
    bool rxMessage(const QString &message);

public slots:
    virtual bool start() = 0;
    virtual void stop() = 0;
    virtual void quit() = 0;
};
} // radio
} // luna
#endif // RADIO_MANAGER_H
