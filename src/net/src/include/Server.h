#ifndef __SERVER__
#define __SERVER__

#include <map>
#include <noncopyable.h>
class InetAddress;
class EventLoop;
class Socket;
class Connection;
class Server : noncopyable {
 public:
  Server(EventLoop *eventloop, InetAddress &addr);

 private:
  int server_fd;
  EventLoop *_eventloop;
  Socket *_socket;
  void newConnect(Socket *server_socket);
  void deleteConnection(Socket * sock);
  std::map<int,Connection*>connections;
};
#endif