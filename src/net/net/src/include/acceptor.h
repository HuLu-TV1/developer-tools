#ifndef __ACCEPTOR__
#define __ACCEPTOR__
#include "noncopyable.h"
#include <functional>
#include <memory>
namespace net {
class Socket;
class EventLoop;
class InetAddress;
class Channel;
class Acceptor : noncopyable {
private:
  EventLoop *eventloop_;
  using NewConnectCallback = std::function<void(int)>;
  NewConnectCallback new_connect_callback_;
  std::unique_ptr<Socket> socket_;
  std::unique_ptr<Channel> accept_channel_;
  int socket_fd_;
  void AcceptConnection();

public:
  Acceptor(EventLoop *event_loop, const InetAddress &listen_addr);
  ~Acceptor();
  void set_new_connection_callback(const NewConnectCallback &cb) {
    new_connect_callback_ = std::move(cb);
  };
};
} // namespace net
#endif