#ifndef LUNA_APIREST_APIREST_H
#define LUNA_APIREST_APIREST_H

#include <QObject>
#include <qhttpserver.hpp>
#include <qhttpserverresponse.hpp>
#include <qhttpserverrequest.hpp>

#include "common/servicebase.h"

namespace luna
{
namespace apirest
{

class Apirest : public common::ServiceBase
{
    Q_OBJECT
public:
    Apirest(QObject *parent = nullptr);

signals:

public slots:
    bool start();
    void stop();

private:
    qhttp::server::QHttpServer* _server;
};

}
}
#endif // LUNA_APIREST_APIREST_H
