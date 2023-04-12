#include "event_loop.h"
#include "channel.h"
#include "epoll.h"
using namespace net;
const int thread_num = 8;
EventLoop::EventLoop() : quit_(false) {
  epoll_ = new Epoll();
  threads_ = new ThreadPool(thread_num);
  threads_->Start();
}

EventLoop::~EventLoop() {
  delete epoll_;
  delete threads_;
}

void EventLoop::loop() {
  while (!quit_) {
    std::vector<Channel *> chs;
    chs = epoll_->CreateWait();
    for (auto it : chs) {
      it->HandleEvent();
    }
  }
}

void EventLoop::UpdateChannel(Channel *channel) { epoll_->Update(channel); }
void EventLoop::AddThreadPool(Task task) { threads_->Run(task); }