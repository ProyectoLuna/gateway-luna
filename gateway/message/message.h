#ifndef MESSAGE_H
#define MESSAGE_H

#include "lunapb.h"

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
#endif // MESSAGE_H
