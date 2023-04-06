
#include <thread>
#include <atomic>
#include <assert.h>
#include <cstdlib>
#include <cstdio>
#include "log_queue.h"
#include "test_util.h"

const int KSize = 10;
auto global_queue = BufferQueue<int>(KSize);
std::atomic<int> p_cnt(0);
std::atomic<int> c_cnt(0);

void produer(int p_num) {
    long long tid = tid_to_ll(std::this_thread::get_id());
    printf("[tid:%lld] %d elements should be produced\n", tid, p_num);
    int i = 0;
    while (i < p_num) {
        bool ret = global_queue.Push(i);
        if (!ret) {
            printf("***[tid:%lld] push failed, buffer queue is full\n", tid);
        }
        else {
            printf("+++[tid:%lld] produced no.%d element\n", tid, i+1);
            p_cnt.fetch_add(1);
            i++;
        }
    }
}

void consumer(int c_num)
{
    long long tid = tid_to_ll(std::this_thread::get_id());
    printf("[tid:%lld] %d elements should be consumed\n", tid, c_num);
    int temp;
    for(int i=0; i<c_num; i++)
    {
        global_queue.Pop(temp);
        printf("---[tid:%lld] consumed no.%d element\n", tid, i+1);
        c_cnt.fetch_add(1);
    }
}

void fill_queue(BufferQueue<int>& q)
{
    for(int i=0; i<q.GetCapacity(); i++)
        q.Push(i);    
}

int main() {
    assert(global_queue.IsEmpty());
    assert(global_queue.GetCapacity()==KSize);

    fill_queue(global_queue);
    assert(global_queue.IsFull());

    global_queue.Clear();
    assert(global_queue.IsEmpty());


    int temp;
    auto start = std::chrono::steady_clock::now();
    bool ret= global_queue.Pop(temp, 2000);  
    auto end = std::chrono::steady_clock::now(); 
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    printf("cond wait_for duration: %d ms\n" ,static_cast<int>(duration.count()));
    assert(!ret);

    printf("buffer queue function test passed!\n\n\n");

    global_queue.Clear();
    fill_queue(global_queue);
    int origin_num = global_queue.GetSize();
    assert(origin_num==KSize);
    printf("------------------------------------------\n");
    printf("buffer queue capacity is %d\n", KSize);
    printf("origin element num is %d\n", origin_num);
    printf("------------------------------------------\n");
    int p_n1 = 100;
    int p_n2 = 50;
    int c_n1 = 160;
    std::thread p1(produer, p_n1);
    std::thread p2(produer, p_n2);
    std::thread c1(consumer, c_n1);
 
    p1.join();
    p2.join();
    c1.join();

    printf("\n");
    printf("origin element num is %d\n", origin_num);
    printf("totally produced %d elements\n", p_cnt.load());
    printf("totally consumed %d elements\n", c_cnt.load());

    assert( global_queue.GetSize() == ( p_n1 + p_n2 + KSize - c_n1 ) );
    printf("buffer queue mutithread test passed!\n");

    return 0;
}