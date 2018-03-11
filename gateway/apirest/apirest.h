#ifndef APIREST_H
#define APIREST_H

#include <QObject>
#include <qhttpserver.hpp>
#include <qhttpserverresponse.hpp>
#include <qhttpserverrequest.hpp>

#include "servicebase.h"

namespace luna
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
#endif // APIREST_H
