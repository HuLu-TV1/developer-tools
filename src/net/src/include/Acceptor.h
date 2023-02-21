#ifndef __ACCEPTOR__
#define __ACCEPTOR__
#include <functional>
#include "noncopyable.h"
class Socket;
class EventLoop;
class InetAddress;
class Channel;
class Acceptor : noncopyable {
 private:
  EventLoop *_eventloop;
  typedef std::function<void(Socket *)> NewConnectCallback;
  NewConnectCallback newConnectCallback;
  Socket *socket;
  Channel *accp_channel;
  int socket_fd;
  void acceptConnection();
 public:
  Acceptor(EventLoop *eventloop, const InetAddress &listenAddr);
  ~Acceptor();
  void setNewConnectionCallback(const NewConnectCallback &cb);
};
#endif