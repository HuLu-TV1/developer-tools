#include "ThreadPool.h"
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;
const int NUMBER = 2;

ThreadPool::ThreadPool(int min, int max)
{
    do
    {
        minTaskNum = min;
        maxTaskNum = max;
        BusyNum = 0;
        AliveNum = min;

        shutdown = false;
        // this : 线程池本身
        manageID = thread(managerTask, this);

        threadIDs.resize(max);
        for (int i = 0; i < min; ++i){
            threadIDs[i] = thread(workTask, this);
        }
        return;
    }while(0);
}

ThreadPool::~ThreadPool()
{
    shutdown = true;
    // 阻塞回收的消费者线程
    if (manageID.joinable()) manageID.join();
    // 阻塞回收消费者线程
    cond.notify_all();
    for (int i = 0; i < maxTaskNum; ++i){
        if (threadIDs[i].joinable()) threadIDs[i].join();
    }
}

void ThreadPool::addTask(callback fun, void* arg)
{
    unique_lock<mutex> lck(mutexPool);
    if (shutdown){
        return;
    }
    // 任务添加
    taskQueue.push(Task(fun, arg));
    cond.notify_all();
}

int ThreadPool::BusyTaskNum()
{
    mutexPool.lock();
    int busy = BusyNum;
    mutexPool.unlock();
    return busy;
}

int ThreadPool::AliveTaskNum()
{
    mutexPool.lock();
    int alive = AliveNum;
    mutexPool.unlock();
    return alive;
}

void ThreadPool::workTask(void* arg)
{
    ThreadPool* pool = static_cast<ThreadPool*>(arg);
    // 消费者(工作线程)不停的读取任务队列
    while(true){
        unique_lock<mutex> lck(pool->mutexPool);
        // 判断当前任务队列是否为空, 且shoutdown为false
        while(pool->taskQueue.empty() && !pool->shutdown){
            // 工作队列为空，线程池未关闭，阻塞当前工作线程
            pool->cond.wait(lck);
            // 判断是否要销毁当前工作线程，管理者让该工作者现场自杀
            if (pool->exitNum > 0)
            {
                pool->exitNum --;
                if (pool->AliveNum > pool->minTaskNum){
                    pool->AliveNum --;
                    cout << "threadid: " << std::this_thread::get_id() << "exit ..." << endl;
                    lck.unlock();
                    return;
                }
            }
        }
        // 判断线程池是否关闭
        if (pool->shutdown){
            cout << "threadid: " << std::this_thread::get_id() << "exit ..." << endl;
            return;
        }
        
        // 从任务队列中去除一个任务
        Task task = pool->taskQueue.front();
        pool->taskQueue.pop();
        pool->BusyNum++;
        // 完成对任务队列的访问，后解锁
        lck.unlock();

        // 取出task任务，可以在当前线程中执行该任务
        cout << "threadid: " << std::this_thread::get_id() << "exit ..." << endl;
        task.function(task.arg);
        free(task.arg);
        task.arg = nullptr;

        // 任务完成，busytasknum 自减
        cout << "threadid: " << std::this_thread::get_id() << "exit ..." << endl;
        lck.lock();
        pool->BusyNum--;
        lck.unlock();
    }
}

// 检查是否需要添加线程还是销毁线程
void ThreadPool::managerTask(void* arg)
{
    ThreadPool* pool = static_cast<ThreadPool*>(arg);
    // managertask 监测
    while(!pool->shutdown){
        // 检测可以设置检查时间间隔
        std::this_thread::sleep_for(std::chrono::seconds(3));

        // 取出线程池中的数量和当前线程的数量，别的线程有可能在写数据，所以需要加锁
        // 目的是添加或者销毁线程
        unique_lock<mutex> lck(pool->mutexPool);
        int queuesize = pool->taskQueue.size();
        int AliveNum = pool->AliveNum;
        int BusyNum = pool->BusyNum;
        lck.unlock();

        // 销毁线程： 当前存活的线程太多，工作的线程太少
        // 忙的线程*2 < 存活的线程数 && 存活的线程数 >  最小的线程数
        if (BusyNum * 2 < AliveNum && AliveNum > pool->minTaskNum)
        {
            // 访问线程池，加锁
            lck.lock();
            pool->exitNum = NUMBER;
            lck.unlock();

            for(int i = 0; i < NUMBER; ++i)
                pool->cond.notify_all();
        }
    }
}
