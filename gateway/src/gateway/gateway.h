#ifndef LUNA_GATEWAY_GATEWAY_H
#define LUNA_GATEWAY_GATEWAY_H

#include <QObject>
#include <QSharedPointer>

namespace luna
{

namespace radio
{
class RadioManager;
}

namespace device
{
class DeviceManager;
}

namespace apirest
{
class Apirest;
}

namespace message
{
class MessageManager;
}

namespace gateway
{

class Gateway : public QObject
{
    Q_OBJECT

public:
    Gateway(QObject *parent = nullptr);
    bool start();

private:
    QSharedPointer<radio::RadioManager> _radioManager;
    QSharedPointer<apirest::Apirest> _apirest;
    QSharedPointer<device::DeviceManager> _deviceManager;
    QSharedPointer<message::MessageManager> _messageManager;

signals:
    void stopped();

public slots:
    void stop();
};

}
}
#endif // LUNA_GATEWAY_GATEWAY_H
