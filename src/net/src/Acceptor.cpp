#include "Acceptor.h"
#include "Channel.h"
#include "Socket.h"
#include "InetAddress.h"
#include <arpa/inet.h>
Acceptor::Acceptor(EventLoop *eventloop, const InetAddress &listenAddr) : _eventloop(eventloop) {
  socket = new Socket();
  socket->CreateSocket(AF_INET);
socket_fd = socket->GetSocketFd();
  socket->SetNonBlocking(socket_fd);
  socket->SetReuserAddr(socket_fd);
  socket->Bind(socket_fd, listenAddr);
  socket->Listen(socket_fd);
  accp_channel = new Channel(_eventloop, socket_fd);
  std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
  accp_channel->setHandleEvent(cb);
  accp_channel->enableReading();
}

Acceptor::~Acceptor() {
  delete socket;
  delete accp_channel;
}

void Acceptor::acceptConnection() {
    Socket * clnt_sock = new Socket(socket->Accept(socket_fd));
    printf("new client fd %d!\n", clnt_sock->GetSocketFd());
    clnt_sock->SetNonBlocking(clnt_sock->GetSocketFd());
    newConnectCallback(clnt_sock);
}
void Acceptor ::setNewConnectionCallback(const NewConnectCallback &cb) { newConnectCallback = cb; }

