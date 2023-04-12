#include "tcp_connection.h"
#include <string.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <chrono>
#include <iostream>
#include <thread>
#include "channel.h"
#include "socket.h"
#include "sockets_ops.h"

using namespace net;
Connection::Connection(EventLoop *eventloop, Socket *clntSock) : eventloop_(eventloop), clntSock_(clntSock) {
  int clnt_fd = clntSock->get_socket_fd();
  client_channel_ = std::make_unique<Channel>(eventloop_, clnt_fd);
  std::function<void()> cb = std::bind(&Connection::Echo, this, clnt_fd);
  client_channel_->set_handle_event(cb);
  client_channel_->EnableReading();
}

Connection::~Connection() { delete clntSock_; }

void Print() {
  char thread_name[256];
  pthread_getname_np(pthread_self(), thread_name, sizeof(thread_name));
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::cout << "thread_name :" << thread_name << std::endl;
  std::cout << "std::this_thread::get_id is : " << std::this_thread::get_id() << std::endl;
}

void Connection::Echo(int sockfd) {
  Print();
  int savedErrno = 0;
  size_t bytes_read = buf_.ReadFd(sockfd, &savedErrno);
  if (bytes_read > 0) {
    printf("message from client fd %d: buf_ = %s\n", sockfd, buf_.Peek());
    write(sockfd, buf_.Peek(), buf_.ReadableBytes());
  } else if (bytes_read == 0) {  // EOF，客户端断开连接
    printf("EOF, client fd %d disconnected\n", sockfd);
    // close(sockfd);   //关闭socket会自动将文件描述符从epoll树上移除
    deleteConnectionCallback(clntSock_);
    return;
  } else {
    errno = savedErrno;
    // LOG_SYSERR << "TcpConnection::handleRead";
    HandleError();
  }
}

void Connection::HandleError() {
  int err = get_socket_error(client_channel_->get_fd());
  std::cout << "TcpConnection::handleError ["
            << "] - SO_ERROR = " << err << " " << strerror(err);
}
