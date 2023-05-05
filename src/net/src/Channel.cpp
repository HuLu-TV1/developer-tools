#include "channel.h"
#include "event_loop.h"
#include "log.h"
#include <iostream>
#include <poll.h>
using namespace net;

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop *ev_loop, int fd) : evloop_(ev_loop), fd_(fd) {}

void Channel::EnableRead() {
  events_ = kReadEvent; // POLLPRI ¾o¼±”µ“þ
  evloop_->UpdateChannel(this);
}

void Channel::EnableWrite() {
  events_ = kWriteEvent;
  evloop_->UpdateChannel(this);
}
void Channel::DisableAll() {
  events_ = kNoneEvent;
  evloop_->UpdateChannel(this);
}

void Channel::DisableWrite() {
  events_ &= ~kWriteEvent;
  evloop_->UpdateChannel(this);
}
void Channel::set_in_epoll() { inEpoll_ = true; }

bool Channel::get_in_epoll() { return inEpoll_; }

int Channel::get_events() { return events_; }
int Channel::get_revents() { return revents_; }

void Channel::set_revents(int event) { revents_ = event; }

int Channel::get_fd() { return fd_; }

void Channel::HandleEvent() {
  LOG_DEBUG("revents_ = %d", revents_);
  if ((revents_ & POLLHUP) && !(revents_ & POLLIN)) {
    if (close_cb_)
      close_cb_();
  }
  if (revents_ & POLLNVAL) {
    LOG_DEBUG("Channel::handle_event() POLLHUP");
  }
  if (revents_ & (POLLERR | POLLNVAL)) {
    if (error_cb_)
      error_cb_();
  }

  if (revents_ & (POLLIN | POLLPRI | POLLRDHUP)) {
    if (read_cb_)
      read_cb_();
  }
  if (revents_ & POLLOUT) {
    if (write_cb_)
      write_cb_();
  }
}
