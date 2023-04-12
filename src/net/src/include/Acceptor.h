#ifndef __ACCEPTOR__
#define __ACCEPTOR__
#include <functional>
#include "noncopyable.h"
namespace net {
class Socket;
class EventLoop;
class InetAddress;
class Channel;
class Acceptor : noncopyable {
 private:
  EventLoop *eventloop_;
  using NewConnectCallback = std::function<void(Socket *)>;
  NewConnectCallback new_connect_callback_;
  Socket *socket_;
  Channel *accept_channel_;
  int socket_fd_;
  void AcceptConnection();

 public:
  Acceptor(EventLoop *event_loop, const InetAddress &listen_addr);
  ~Acceptor();
  void set_new_connection_callback(const NewConnectCallback &cb);
};
}  // namespace net
#endif