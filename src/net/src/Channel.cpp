#include "channel.h"
#include <iostream>
#include "event_loop.h"

using namespace net;
using std::cout;
using std::endl;

Channel::Channel(EventLoop *ev_loop, int fd) : evloop_(ev_loop), fd_(fd) {}

void Channel::EnableReading() {
  events_ = EPOLLIN | EPOLLET;
  evloop_->UpdateChannel(this);
}

void Channel::set_in_epoll() { inEpoll_ = true; }

bool Channel::get_in_epoll() { return inEpoll_; }

int Channel::get_events() { return events_; }
int Channel::get_revents() { return revents_; }

void Channel::set_revents(int event) {
  std::cout << "setRevents" << std::endl;
  revents_ = event;
}

int Channel::get_fd() { return fd_; }

void Channel::HandleEvent() { evloop_->AddThreadPool(handle_event_cb_); }
void Channel::set_handle_event(HandleEventCallback cb) { handle_event_cb_ = cb; }
