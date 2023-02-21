#include "EventLoop.h"
#include "Channel.h"
#include "Epoll.h"

EventLoop::EventLoop() : quit(false) { _epoll = new Epoll(); }

EventLoop::~EventLoop() { delete _epoll; }

void EventLoop::loop() {
  while (!quit) {
    std::vector<Channel *> chs;
    chs = _epoll->createWait();
    for (auto it : chs) {
      it->handleEvent();
    }
  }
}

void EventLoop::updateChannel(Channel *channel) { _epoll->update(channel); }