#ifndef __SERVER__
#define __SERVER__

#include "noncopyable.h"
#include "memory"
#include <map>
#include <vector>
#include <tcp_connection.h>
namespace net {
class InetAddress;
class EventLoop;
class Socket;
class ThreadPool;
class Server : noncopyable {
 public:
  Server(EventLoop *eventloop, InetAddress &addr,int threads_num = 10);
  void Start();
  void set_message_cb(const OnMessageCallback& cb){message_callback_ = std::move(cb);}
  void set_connect_cb(const ConnectCallback& cb) {connection_callback_ = std::move(cb);}
 private:
  EventLoop *main_eventloop_;
  int thread_num_;
  int server_fd_;
  std::vector<std::unique_ptr<EventLoop>>sub_eventloops_;
  Socket *_socket_;
  void NewConnect(Socket *server_socket);
  void DeleteConnection(Socket *sock);
  std::map<int, Connection *> connections_;
  std::unique_ptr<ThreadPool>threads_pools_;
  ConnectCallback connection_callback_;
  OnMessageCallback message_callback_;
};
}  // namespace net
#endif