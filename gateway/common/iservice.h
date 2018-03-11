#ifndef ISERVICE_H
#define ISERVICE_H

#include <QObject>
#include <QString>

namespace luna
{
namespace common
{

class IService
{
public:
    virtual QString getName() = 0;
    virtual QObject *getObject() = 0;

signals:

public slots:
    virtual bool start() = 0;
    virtual void stop() = 0;
};

} // common
} // luna
#endif // ISERVICE_H
