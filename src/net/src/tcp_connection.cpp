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
  int connect_fd_ = clntSock->get_socket_fd();
  std::cout << "connect_fd_ " << connect_fd_ << std::endl;
  if (eventloop != nullptr) {
    client_channel_ = std::make_unique<Channel>(eventloop_, connect_fd_);
    client_channel_->set_handle_read_event(std::bind(&Connection::HandleRead, this));
    client_channel_->set_handle_write_event(std::bind(&Connection::HandleWrite, this));
    client_channel_->set_handle_error_event(std::bind(&Connection::HandleError, this));
    client_channel_->set_handle_close_event(std::bind(&Connection::HandleClose, this));
    client_channel_->EnableRead();
  }
}

Connection::~Connection() {}

void Print() {
  char thread_name[256];
  pthread_getname_np(pthread_self(), thread_name, sizeof(thread_name));
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::cout << "thread_name :" << thread_name << std::endl;
  std::cout << "std::this_thread::get_id is : " << std::this_thread::get_id() << std::endl;
}

void Connection::HandleError() {
  int err = get_socket_error(client_channel_->get_fd());
  std::cout << "TcpConnection::HandleError ["
            << "] - SO_ERROR = " << err << " " << strerror(err);
}

void Connection::HandleRead() {
  int savedErrno;
  ssize_t len = input_buf_.ReadFd(client_channel_->get_fd(), &savedErrno);
  if (len > 0) {
    //std::cout << input_buf_.RetrieveAllAsString() << std::endl;
    message_callback_(this, &input_buf_);
  } else if (len == 0) {
    std::cout << "len == 0 " << client_channel_->get_fd() << std::endl;
    HandleClose();
  } else {
    errno = savedErrno;
    std::cout << "TcpConnection::handleRead error" << std::endl;
    HandleError();
  }
}
void Connection::HandleWrite() {}
void Connection::HandleClose() {
  std::cout << "HandleClose fd = " << client_channel_->get_fd() << std::endl;
    connectio_callback_(this);
  client_channel_->DisableAll();
  deleteConnectionCallback_(clntSock_);
}
void Connection::send( std::string str) {
  ssize_t nwrote = 0;
  size_t remaining = str.size();
  size_t len = str.size();
  bool faultError = false;
  if (!client_channel_->IsWriting() && output_buf_.ReadableBytes() == 0) {
    nwrote = write(clntSock_->get_socket_fd(), str.c_str(), len);
    if (nwrote >= 0) {
      remaining = len - nwrote;
      if (remaining == 0) {
        std::cout << "send complete" << std::endl;
      }
    } else {
      nwrote = 0;
      if (errno != EWOULDBLOCK) {
        if (errno == EPIPE || errno == ECONNRESET)  // FIXME: any others?
        {
          faultError = true;
        }
      }
    }
  }

  if (!faultError && remaining > 0) {
    // std::string tmp(str.c_str()+nwrote);
    output_buf_.Append(str.c_str()+nwrote, remaining);
    if (!client_channel_->IsWriting()) {
      client_channel_->EnableWrite();
    }
  }
}
