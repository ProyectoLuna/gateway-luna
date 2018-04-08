#ifndef LUNA_MESSAGE_MESSAGE_H
#define LUNA_MESSAGE_MESSAGE_H

#include <QSharedPointer>
#include "protos/nanopb/lunapb.h"

namespace luna
{

namespace message
{

template<class T>
class Message
{
public:
    Message();
    Message(RemoteDevMessage* proto);
    ~Message();
    void setProto(RemoteDevMessage* proto);
    RemoteDevMessage* getProto();

private:
    RemoteDevMessage* _proto;
};

template<class T>
Message<T>::Message()
{
    _proto = nullptr;
}

template<class T>
Message<T>::Message(RemoteDevMessage *proto)
{
    _proto = proto;
}

template<class T>
Message<T>::~Message()
{
    if (_proto)
    {
        if ((_proto->data.arg) &&
                static_cast<T*>(_proto->data.arg)->data)
        {
            delete [] static_cast<T*>(_proto->data.arg)->data;
            delete static_cast<T*>(_proto->data.arg);
        }
        delete _proto;
    }
}

template<class T>
void Message<T>::setProto(RemoteDevMessage *proto)
{
    _proto = proto;
}

template<class T>
RemoteDevMessage* Message<T>::getProto()
{
    return _proto;
}

} // message
} // luna
#endif // LUNA_MESSAGE_MESSAGE_H
