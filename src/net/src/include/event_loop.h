#ifndef __EVENTLOOP_
#define __EVENTLOOP_
#include <functional>
#include <memory>
#include "noncopyable.h"
#include "thread_pool.h"
namespace net {
class Epoll;
class Channel;

class EventLoop : noncopyable {
 public:
  EventLoop();
  void loop();
  ~EventLoop();
  void UpdateChannel(Channel *channel);
  void AddThreadPool(Task task);

 private:
  Epoll *epoll_;
  //  std::unique_ptr<ThreadPool>threads_;
  ThreadPool *threads_;
  bool quit_;
};
}  // namespace net
#endif