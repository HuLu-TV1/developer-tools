#ifndef __THREADPOOL__
#define __THREADPOOL__

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
namespace net {

using Task = std::function<void()>;
class ThreadPool {
public:
  ThreadPool(int thread_num);
  ~ThreadPool();
  void Start();
  template <class F, class... Args>
  auto Run(F &&f, Args &&... args)
      -> std::future<typename std::invoke_result<F, Args...>::type>;
  int get_thread_num() const { return threads_.size(); }

  int get_task_num() const { return tasks_.size(); }

private:
  int thread_num_;
  bool stop_;
  std::vector<std::thread> threads_;
  std::queue<Task> tasks_;
  std::mutex tasks_mtx_;
  std::condition_variable cv_;
};
// 不能放在cpp文件，C++编译器不支持模版的分离编译
template <class F, class... Args>
auto ThreadPool::Run(F &&f, Args &&... args)
    -> std::future<typename std::invoke_result<F, Args...>::type> {
  using return_type = typename std::invoke_result<F, Args...>::type;

  auto task = std::make_shared<std::packaged_task<return_type()>>(
      std::bind(std::forward<F>(f), std::forward<Args>(args)...));

  std::future<return_type> res = task->get_future();
  {
    std::unique_lock<std::mutex> lock(tasks_mtx_);

    // don't allow enqueueing after stopping the pool
    if (stop_) {
      throw std::runtime_error("enqueue on stopped ThreadPool");
    }

    tasks_.emplace([task]() { (*task)(); });
  }
  cv_.notify_one();
  return res;
}

} // namespace net
#endif