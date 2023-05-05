#include "tcp_connection.h"
#include "channel.h"
#include "log.h"
#include "socket.h"
#include "sockets_ops.h"
#include <chrono>
#include <string.h>
#include <sys/prctl.h>
#include <thread>
#include <unistd.h>
using namespace net;
Connection::Connection(EventLoop *eventloop, int client_fd)
    : eventloop_(eventloop), clntSock_(std::make_unique<Socket>(client_fd)) {
  int connect_fd_ = clntSock_->get_socket_fd();
  clntSock_->set_nonblocking(client_fd);
  LOG_DEBUG("connect_fd = %d", connect_fd_);
  if (eventloop != nullptr) {
    client_channel_ = std::make_unique<Channel>(eventloop_, connect_fd_);
    client_channel_->set_handle_read_event(
        std::bind(&Connection::HandleRead, this));
    client_channel_->set_handle_write_event(
        std::bind(&Connection::HandleWrite, this));
    client_channel_->set_handle_error_event(
        std::bind(&Connection::HandleError, this));
    client_channel_->set_handle_close_event(
        std::bind(&Connection::HandleClose, this));
    client_channel_->EnableRead();
  }
}

Connection::~Connection() {}

void Print() {
  char thread_name[256];
  pthread_getname_np(pthread_self(), thread_name, sizeof(thread_name));
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  LOG_DEBUG("thread_name = %s", thread_name);
  LOG_DEBUG("std::this_thread::get_id is  = %d", std::this_thread::get_id());
}

void Connection::HandleError() {
  int err = get_socket_error(client_channel_->get_fd());
  LOG_DEBUG("TcpConnection::HandleError [] - SO_ERROR =%d err=%s", err,
            strerror(err));
}

void Connection::HandleRead() {
  int savedErrno;
  ssize_t len = input_buf_.ReadFd(client_channel_->get_fd(), &savedErrno);
  if (len > 0) {
    message_callback_(this, &input_buf_);
  } else if (len == 0) {
    LOG_DEBUG("len == 0");
    HandleClose();
  } else {
    errno = savedErrno;
    HandleError();
  }
}
void Connection::HandleWrite() {
  if(client_channel_->IsWriting()) {
    ssize_t n =write(clntSock_->get_socket_fd(), output_buf_.Peek(), output_buf_.ReadableBytes());
    if(n > 0) {
      output_buf_.Retrieve(n);
      if(output_buf_.ReadableBytes() == 0){
        client_channel_->DisableWrite();
      } else {
         LOG_DEBUG ( "TcpConnection::handleWrite");
      }
    }
  } else {
    LOG_DEBUG("Connection fd =  = %d,is down, no more writing",client_channel_->get_fd());

  }
}
void Connection::HandleClose() {
  LOG_DEBUG("close fd = %d", client_channel_->get_fd());
  connectio_callback_(this);
  client_channel_->DisableAll();
  deleteConnectionCallback_(clntSock_.get());
}
void Connection::send(std::string str) {
  ssize_t nwrote = 0;
  size_t remaining = str.size();
  size_t len = str.size();
  bool faultError = false;
  if (!client_channel_->IsWriting() && output_buf_.ReadableBytes() == 0) {
    nwrote = write(clntSock_->get_socket_fd(), str.c_str(), len);
    if (nwrote >= 0) {
      remaining = len - nwrote;
      if (remaining == 0) {
        char  thread_name[255];
        pthread_getname_np(pthread_self(), thread_name, sizeof(thread_name));
        LOG_DEBUG("thread_name =%s send message complete",thread_name);
      }
    } else {
      nwrote = 0;
      if (errno != EWOULDBLOCK) {
        if (errno == EPIPE || errno == ECONNRESET) // FIXME: any others?
        {
          faultError = true;
        }
      }
    }
  }

  if (!faultError && remaining > 0) {
    // std::string tmp(str.c_str()+nwrote);
    output_buf_.Append(str.c_str() + nwrote, remaining);
    if (!client_channel_->IsWriting()) {
      client_channel_->EnableWrite();
    }
  }
}

void Connection::ShutDownWrite() {
  if (!client_channel_->IsWriting()) {
    clntSock_->shutdownWrite();
  }
}
