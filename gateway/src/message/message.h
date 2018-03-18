#ifndef LUNA_MESSAGE_MESSAGE_H
#define LUNA_MESSAGE_MESSAGE_H

namespace luna
{
namespace message
{

template<class T>
class Message
{
public:
    Message();
    Message(T* proto);
    ~Message();
    void setProto(T* proto);

private:
    T* _proto;
};

template<class T>
Message<T>::Message()
{
    _proto = nullptr;
}

template<class T>
Message<T>::Message(T *proto)
{
    _proto = proto;
}

template<class T>
Message<T>::~Message()
{
    if (_proto)
    {
        delete _proto;
    }
}

template<class T>
void Message<T>::setProto(T *proto)
{
    _proto = proto;
}

} // message
} // luna
#endif // LUNA_MESSAGE_MESSAGE_H
