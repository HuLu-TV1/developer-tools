#include "acceptor.h"
#include <arpa/inet.h>
#include "channel.h"
#include "inet_address.h"
#include "socket.h"
using namespace net;
Acceptor::Acceptor(EventLoop *event_loop, const InetAddress &listen_addr) : eventloop_(event_loop) {
  socket_ = new Socket();
  socket_->CreateSocket(AF_INET);
  socket_fd_ = socket_->get_socket_fd();
  socket_->set_nonblocking(socket_fd_);
  socket_->set_reuser_addr(socket_fd_);
  socket_->Bind(socket_fd_, listen_addr);
  socket_->Listen(socket_fd_);
  accept_channel_ = new Channel(eventloop_, socket_fd_);
  std::function<void()> cb = std::bind(&Acceptor::AcceptConnection, this);
  accept_channel_->set_handle_event(cb);
  accept_channel_->EnableReading();
}

Acceptor::~Acceptor() {
  delete socket_;
  delete accept_channel_;
}

void Acceptor::AcceptConnection() {
  Socket *clnt_sock = new Socket(socket_->Accept(socket_fd_));
  printf("new client fd %d!\n", clnt_sock->get_socket_fd());
  clnt_sock->set_nonblocking(clnt_sock->get_socket_fd());
  new_connect_callback_(clnt_sock);
}
void Acceptor ::set_new_connection_callback(const NewConnectCallback &cb) { new_connect_callback_ = cb; }
