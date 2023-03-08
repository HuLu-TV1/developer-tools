#ifndef __CONNECTION__
#define __CONNECTION__
#include <functional>
#include <memory>
#include "buffer.h"
#include "noncopyable.h"
namespace net {
class EventLoop;
class Socket;
class Channel;
class Connection : noncopyable {
 private:
  EventLoop *eventloop_;
  void Echo(int sockfd);
  std::function<void(Socket *)> deleteConnectionCallback;
  Socket *clntSock_;
  std::unique_ptr<Channel> client_channel_;
  Buffer buf_;

 public:
  Connection(EventLoop *eventloop, Socket *clntSock);
  ~Connection();
  void set_delete_connection_callback(std::function<void(Socket *)> cb) { deleteConnectionCallback = cb; }
  void HandleError();
};
}  // namespace net
#endif