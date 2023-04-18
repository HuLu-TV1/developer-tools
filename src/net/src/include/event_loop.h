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
  void Loop();
  ~EventLoop();
  void UpdateChannel(Channel *channel);
  void AddThreadPool(Task task);

 private:
  Epoll *epoll_;
  bool quit_;
};
}  // namespace net
#endif