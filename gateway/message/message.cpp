#include "message.h"

using namespace luna;
using namespace message;

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
