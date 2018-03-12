#ifndef LUNA_GATEWAY_GATEWAY_H
#define LUNA_GATEWAY_GATEWAY_H

#include <QObject>
#include "radio/radio_manager.h"
#include "apirest/apirest.h"

namespace luna
{
namespace gateway
{

class Gateway : public QObject
{
    Q_OBJECT

public:
    Gateway(QObject *parent = nullptr);
    bool start();

private:
    radio::RadioManager *_radioManager;
    apirest::Apirest *_apirest;

signals:
    void stopped();

public slots:
    void stop();
};

}
}
#endif // LUNA_GATEWAY_GATEWAY_H
