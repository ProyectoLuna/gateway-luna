#ifndef LUNA_DEVICE_DEVICEMANAGER_H
#define LUNA_DEVICE_DEVICEMANAGER_H

#include <QObject>

namespace luna
{
namespace device
{

class DeviceManager : public QObject
{
    Q_OBJECT
public:
    explicit DeviceManager(QObject *parent = nullptr);

signals:

public slots:
};

}
}
#endif // LUNA_DEVICE_DEVICEMANAGER_H
