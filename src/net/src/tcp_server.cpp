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

using namespace net;
Server::Server(EventLoop *eventloop, InetAddress &addr) : eventloop_(eventloop) {
  Acceptor *accptor = new Acceptor(eventloop_, addr);
  std::function<void(Socket *)> connectCb = std::bind(&Server::NewConnect, this, std::placeholders::_1);
  accptor->set_new_connection_callback(connectCb);
}

void Server::NewConnect(Socket *sock) {
  Connection *connection = new Connection(eventloop_, sock);
  std::function<void(Socket *)> deleteCb = std::bind(&Server::DeleteConnection, this, std::placeholders::_1);
  connection->set_delete_connection_callback(deleteCb);
  connections_[sock->get_socket_fd()] = connection;
}

void Server::DeleteConnection(Socket *sock) {
  Connection *conn = connections_[sock->get_socket_fd()];
  connections_.erase(sock->get_socket_fd());
  delete conn;
}
