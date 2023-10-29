#include "ThreadPool.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

void taskFunc(void* arg)
{
    int nNum = *(int*)arg;
    cout << "thread: " << std::this_thread::get_id() << ", number=" << nNum << endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main()
{
    // 设置线程池最小最大线程个数
    ThreadPool pool(5, 10);
    int i;
    // 往任务队列中添加100个任务
    for (i = 0; i < 100; ++i)
    {
        int* pNum = new int(i + 100);
        pool.addTask(taskFunc, (void*)pNum);
    }
	for (; i < 200; ++i)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		int* pNum = new int(i + 100);
		pool.addTask(taskFunc, (void*)pNum);
	}
    return 0;
}
