#include <thread>
#include <mutex>
#include <queue>
#include <vector>
#include <condition_variable>

using namespace std;
using namespace std::literals::chrono_literals;

using callback = void(*)(void*);


/*
 * 任务队列类
 * 参数1：任务执行函数，参数2：任务执行函数的参数
 */
class Task
{
public:
    Task(callback f, void* arg)
    {
        function = f;
        this->arg = arg;
    }

public:
    callback function;
    void* arg;
};

class ThreadPool
{
public:
    ThreadPool(int min, int max);
    // add task
    void addTask(callback f, void* arg);
    void addTask(Task task);
    // running task num
    int BusyTaskNum();
    // alive task num
    int AliveTaskNum();
    ~ThreadPool();

private:
    queue<Task> taskQueue;      // task queue
    thread manageID;            // manage ID
    vector<thread> threadIDs;
    int minTaskNum;             // min task num
    int maxTaskNum;             // max task num
    int BusyNum;                // busy task num
    int AliveNum;               // alive task num
    int exitNum;                // need destroy task num
    mutex mutexPool;            // Thread pool mutex
    condition_variable cond;     // if task queue empty please block Consumers thread
    bool shutdown;              // destroy Thread pool, 1:destroy 0:do nothing

    static void managerTask(void* arg); // manager task
    static void workTask(void* arg);    // Consumers task
};

