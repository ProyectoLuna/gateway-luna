#ifndef LUNA_MESSAGE_MESSAGE_H
#define LUNA_MESSAGE_MESSAGE_H

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
    Message(T* proto);
    ~Message();
    void setProto(T* proto);

private:
    T* _proto;
};

}
}
#endif // LUNA_MESSAGE_MESSAGE_H
