#ifndef __EVENTLOOP_
#define __EVENTLOOP_
#include "noncopyable.h"
#include "thread_pool.h"
#include <functional>
#include <memory>
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
} // namespace net
#endif