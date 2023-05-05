#ifndef __SERVER__
#define __SERVER__

#include "memory"
#include "noncopyable.h"
#include <map>
#include <tcp_connection.h>
#include <vector>
namespace net {
class InetAddress;
class EventLoop;
class Socket;
class ThreadPool;
class Server : noncopyable {
public:
  Server(EventLoop *eventloop, InetAddress &addr, int threads_num = 10);
  void Start();
  void set_message_cb(const OnMessageCallback &cb) {
    message_callback_ = std::move(cb);
  }
  void set_connect_cb(const ConnectCallback &cb) {
    connection_callback_ = std::move(cb);
  }

private:
  EventLoop *main_eventloop_;
  int thread_num_;
  int server_fd_;
  std::vector<std::unique_ptr<EventLoop>> sub_eventloops_;
  Socket *_socket_;
  void NewConnect(int client_fd);
  void DeleteConnection(Socket *sock);
  std::map<int, std::unique_ptr<Connection>> connections_;
  std::unique_ptr<ThreadPool> threads_pools_;
  OnMessageCallback message_callback_;
  ConnectCallback connection_callback_;
  static void DefaultMessageCb(Connection *connect, Buffer *buf);
  static void DefaultConnectCb(Connection *connect);
};
} // namespace net
#endif