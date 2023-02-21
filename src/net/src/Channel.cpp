#include "Channel.h"
#include <iostream>
#include "EventLoop.h"
using std::cout;
using std::endl;
Channel::Channel(EventLoop *evloop, int fd) : _evloop(evloop), _fd(fd) {}

void Channel::enableReading() {
  events = EPOLLIN | EPOLLET;
  _evloop->updateChannel(this);
}

void Channel::setInEpoll() { inEpoll = true; }

bool Channel::getInEpoll() { return inEpoll; }

int Channel::getEvents() { return events; }
int Channel::getRevents() { return revents; }

void Channel::setRevents(int event) {
  std::cout << "setRevents" << std::endl;
  revents = event;
}

int Channel::getFd() { return _fd; }

void Channel::handleEvent() { handleEventcb(); }
void Channel::setHandleEvent(handleEventCallback cb) { handleEventcb = cb; }
