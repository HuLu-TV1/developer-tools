#include "Server.h"
#include <Acceptor.h>
#include <unistd.h>
#include <cstring>
#include <functional>
#include "Channel.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Socket.h"
#include "Connection.h"
Server::Server(EventLoop *eventloop, InetAddress &addr) : _eventloop(eventloop) {
  Acceptor *accptor = new Acceptor(eventloop, addr);
  std::function<void(Socket *)> connectCb = std::bind(&Server::newConnect, this, std::placeholders::_1);
  accptor->setNewConnectionCallback(connectCb);
}

void Server::newConnect(Socket *sock) {
  Connection *connection = new Connection(_eventloop,sock);
  std::function<void(Socket*)>deleteCb = std::bind(&Server::deleteConnection,this,std::placeholders::_1);
  connection->setDeleteConnectionCallback(deleteCb);
  connections[sock->GetSocketFd()] = connection;
}

void Server::deleteConnection(Socket * sock){
    Connection *conn = connections[sock->GetSocketFd()];
    connections.erase(sock->GetSocketFd());
    delete conn;
}
