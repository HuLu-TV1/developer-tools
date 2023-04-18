#include "acceptor.h"
#include <arpa/inet.h>
#include "channel.h"
#include "inet_address.h"
#include "socket.h"
using namespace net;
Acceptor::Acceptor(EventLoop *event_loop, const InetAddress &listen_addr) : eventloop_(event_loop) {
  socket_ = std::make_unique<Socket>();
  printf("new socket_ fd %d!\n", socket_->get_socket_fd());
  socket_->CreateSocket(AF_INET);
  socket_fd_ = socket_->get_socket_fd();
  socket_->set_nonblocking(socket_fd_);
  socket_->set_reuser_addr(socket_fd_);
  socket_->Bind(socket_fd_, listen_addr);
  socket_->Listen(socket_fd_);
  accept_channel_ = std::make_unique<Channel>(eventloop_, socket_fd_);
  std::function<void()> cb = std::bind(&Acceptor::AcceptConnection, this);
  accept_channel_->set_handle_read_event(cb);
  accept_channel_->EnableRead();
}

Acceptor::~Acceptor() {}

void Acceptor::AcceptConnection() {
  clnt_sock = std::make_unique<Socket>(socket_->Accept(socket_fd_));
  // Socket* clnt_sock = new Socket(socket_->Accept(socket_fd_));
  printf("new client fd %d!\n", clnt_sock->get_socket_fd());
  clnt_sock->set_nonblocking(clnt_sock->get_socket_fd());
  new_connect_callback_(clnt_sock.get());
}
