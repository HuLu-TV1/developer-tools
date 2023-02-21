#ifndef __EVENTLOOP_
#define __EVENTLOOP_
class Epoll;
class Channel;
#include "noncopyable.h"
class EventLoop : noncopyable {
 public:
  EventLoop();
  void loop();
  ~EventLoop();
  void updateChannel(Channel *channel);

 private:
  Epoll *_epoll;
  bool quit;
};
#endif