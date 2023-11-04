#ifndef __CHANNEL__
#define __CHANNEL__
#include "callback.h"
#include "noncopyable.h"
#include <functional>
#include <sys/epoll.h>
namespace net {
class Epoll;
class EventLoop;
class Channel : noncopyable {
public:
  Channel(EventLoop *ev_loop, int fd);
  void EnableRead();
  void EnableWrite();
  void DisableAll();
  void DisableWrite();
  void set_in_epoll();
  bool get_in_epoll();
  int get_events();
  void set_revents(int event);
  int get_revents();
  int get_fd();
  void HandleEvent();

  void set_handle_read_event(const ReadEventCallback &cb) {
    read_cb_ = std::move(cb);
  }
  void set_handle_write_event(const WriteEventCallback &cb) {
    write_cb_ = std::move(cb);
  }
  void set_handle_error_event(const ErrorEventCallback &cb) {
    error_cb_ = std::move(cb);
  }
  void set_handle_close_event(const CloseEventCallback &cb) {
    close_cb_ = std::move(cb);
  }
  bool IsWriting() const { return events_ & kWriteEvent; }
  bool IsReading() const { return events_ & kReadEvent; }
  bool IsNoneEvent() const { return events_ == kNoneEvent; }
  

private:
  int events_;
  int revents_;
  bool inEpoll_;
  EventLoop *evloop_;
  int fd_;
  ReadEventCallback read_cb_;
  WriteEventCallback write_cb_;
  CloseEventCallback close_cb_;
  ErrorEventCallback error_cb_;

  static const int kNoneEvent;
  static const int kReadEvent;
  static const int kWriteEvent;
};
} // namespace net
#endif