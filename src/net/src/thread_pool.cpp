#include <iostream>
#include <string>
#include <sys/prctl.h>
#include <thread_pool.h>
using namespace net;
ThreadPool::ThreadPool(int thread_num)
    : thread_num_(thread_num), stop_(false) {}

void ThreadPool::Start() {
  for (int i = 0; i < thread_num_; i++) {
    threads_.emplace_back(std::thread([=] {
      std::string pthread_name = "thread num:" + std::to_string(i);
      pthread_setname_np(pthread_self(), pthread_name.c_str());
      while (true) {
        /*
        /需要把lock的作用域设置在task()之前，原因是执行到wait时，
        会释放锁并且阻塞到wait处，此时其他线程获取lock，也阻塞到wait处，
        当其中一个线程a获取到队列任务之后，wait会重新lock住，那么如果task（）在lock的作用域，
        会导致其他线程在线程a执行完之后，才能unlock，从而导致多线程按顺序执行
        */
        Task task;
        {
          std::unique_lock<std::mutex> lock(tasks_mtx_);
          cv_.wait(lock, [this]() { return stop_ | !tasks_.empty(); });
          if (stop_ && tasks_.empty())
            return;
          task = tasks_.front();
          tasks_.pop();
        }
        task();
      }
    }));
  }
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(tasks_mtx_);
    stop_ = true;
  }
  cv_.notify_all();
  for (std::thread &th : threads_) {
    if (th.joinable())
      th.join();
  }
}
