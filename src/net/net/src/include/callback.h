#ifndef __CALLBACK__
#define __CALLBACK__
#include <functional>
#include <memory>
namespace net {
class Socket;
class Connection;
class Buffer;
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;
// using ConnectPtr = std::shared_ptr<Connection>  ;
using DeleteConnectCallback = std::function<void(Socket *)>;
using ReadEventCallback = std::function<void()>;
using WriteEventCallback = std::function<void()>;
using CloseEventCallback = std::function<void()>;
using ErrorEventCallback = std::function<void()>;
using OnMessageCallback = std::function<void(Connection *, Buffer *buf)>;
using ConnectCallback = std::function<void(Connection *)>;
} // namespace net
#endif