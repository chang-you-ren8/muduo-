#ifndef NET_CALLBACKS_H
#define NET_CAALBACKS_H

#include "Timestamp.h"

#include <functional>
#include <memory>

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;


template<typename T>
inline T *get_pointer(const std::shared_ptr<T> &ptr)
{
    return ptr.get();
}

template<typename T>
inline T *get_pointer(const std::unique_ptr<T> &ptr)
{
    return ptr.get();
}

class Buffer;
class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using ConnectionCallback = std::function<void(const TcpConnectionPtr &)>;
using CloseCallback = std::function<void(const TcpConnectionPtr &)>;
using MessageCallback = std::function<void(const TcpConnectionPtr &,
                                        Buffer *,
                                        Timestamp)>;
#endif 