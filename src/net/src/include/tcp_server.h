#ifndef __SERVER__
#define __SERVER__

#include "noncopyable.h"

#include <map>

namespace net {
class InetAddress;
class EventLoop;
class Socket;
class Connection;

class Server : noncopyable {
 public:
  Server(EventLoop *eventloop, InetAddress &addr);

 private:
  int server_fd_;
  EventLoop *eventloop_;
  Socket *_socket_;
  void NewConnect(Socket *server_socket);
  void DeleteConnection(Socket *sock);
  std::map<int, Connection *> connections_;
};
}  // namespace net
#endif