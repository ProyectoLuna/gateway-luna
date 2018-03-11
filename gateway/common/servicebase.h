#ifndef SERVICEBASE_H
#define SERVICEBASE_H

#include <QObject>
#include "iservice.h"

namespace luna
{
namespace common
{

class ServiceBase : public QObject, virtual public IService
{
    Q_OBJECT

protected:
    enum class Status
    {
        STOPPED,
        STARTED
    };
    std::atomic<Status> _status;
    QString _name;

public:
    ServiceBase(QObject *parent = nullptr);
    ~ServiceBase(){}
    QString getName() override;
    QObject *getObject() override;

signals:

public slots:
    bool start() override;
    void stop() override;
};

} // common
} // luna
#endif // SERVICEBASE_H
