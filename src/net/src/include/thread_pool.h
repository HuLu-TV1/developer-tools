#ifndef __THREADPOOL__
#define __THREADPOOL__

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

using Task = std::function<void()>;
class ThreadPool {
 public:
  ThreadPool(int thread_num);
  ~ThreadPool();
  void Start();
  void Run(Task task);
  int get_thread_num() const { return threads_.size(); }

  int get_task_num() const { return tasks_.size(); }

 private:
  int thread_num_;
  bool stop;
  std::vector<std::thread> threads_;
  std::queue<Task> tasks_;
  std::mutex tasks_mtx_;
  std::condition_variable cv_;
};

#endif