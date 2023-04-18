#include "tcp_server.h"
#include <acceptor.h>
#include <unistd.h>
#include <cstring>
#include <functional>
#include "channel.h"
#include "event_loop.h"
#include "inet_address.h"
#include "socket.h"
#include "tcp_connection.h"
#include "thread_pool.h"
using namespace net;
Server::Server(EventLoop *eventloop, InetAddress &addr,int threads_num) : main_eventloop_(eventloop),thread_num_(threads_num){
  Acceptor *accptor = new Acceptor(main_eventloop_, addr);
  std::function<void(Socket *)> connectCb = std::bind(&Server::NewConnect, this, std::placeholders::_1);
  accptor->set_new_connection_callback(connectCb);
  threads_pools_ = std::make_unique<ThreadPool>(thread_num_);
  threads_pools_->Start();
  for(int i = 0;i<thread_num_;i++) {
    std::unique_ptr<EventLoop>sub_loop = std::make_unique<EventLoop>();
    sub_eventloops_.emplace_back(std::move(sub_loop));
  }
}
void Server::Start() {
  for(int i  = 0 ; i<static_cast<int>(sub_eventloops_.size());i++) {
    std::function<void()> sub_loop = std::bind(&EventLoop::Loop,sub_eventloops_[i].get());
    threads_pools_->Run(sub_loop);
  }
  main_eventloop_->Loop();
}

void Server::NewConnect(Socket *sock) {
  static int next = 0;
 // Connection *connection = new Connection(sub_eventloops_[next].get(), sock);
 Connection *connection = new Connection(main_eventloop_, sock);
 connection->set_message_callback(message_callback_);
 connection->set_connect_callback(connection_callback_);
  next++;
  if(next >= static_cast<int>(sub_eventloops_.size())) {
      next = 0;
  }
  std::function<void(Socket *)> deleteCb = std::bind(&Server::DeleteConnection, this, std::placeholders::_1);
  connection->set_delete_connection_callback(deleteCb);
  connections_[sock->get_socket_fd()] = connection;
}

void Server::DeleteConnection(Socket *sock) {
  Connection *conn = connections_[sock->get_socket_fd()];
  connections_.erase(sock->get_socket_fd());
  delete conn;
}
