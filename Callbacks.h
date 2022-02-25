#ifndef NET_CALLBACKS_H
#define NET_CALLBACKS_H

#include "Timestamp.h"

#include <functional>
#include <string>
#include <memory>

class InetAddress;
class TcpConnection;
class Buffer;

using std::string;

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

using TimerCallback = std::function<void()>;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using NewConnectionCallback = std::function<void(const TcpConnectionPtr &)>;
using MessageCallback = std::function<void(const TcpConnectionPtr &, Buffer *, Timestamp)>;
using CloseCallback = std::function<void(const TcpConnectionPtr &)>;

#endif 