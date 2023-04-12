#ifndef __CHANNEL__
#define __CHANNEL__
#include <sys/epoll.h>
#include <functional>
#include "noncopyable.h"


namespace net {
  class Epoll;
class EventLoop;
class Channel : noncopyable {
 public:
  using HandleEventCallback = std::function<void()>;
  Channel(EventLoop *ev_loop, int fd);
  void EnableReading();
  void set_in_epoll();
  bool get_in_epoll();
  int get_events();
  void set_revents(int event);
  int get_revents();
  int get_fd();
  void HandleEvent();
  void set_handle_event(HandleEventCallback cb);

 private:
  int events_;
  int revents_;
  bool inEpoll_;
  EventLoop *evloop_;
  int fd_;
  HandleEventCallback handle_event_cb_;
};
}  // namespace net
#endif