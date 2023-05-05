#include "thread_pool.h"
#include <iostream>
#include <sys/prctl.h>
#include <functional>
#include <chrono>
using namespace net;
void Print() {
    char thread_name[256];
    pthread_getname_np(pthread_self(), thread_name, sizeof(thread_name));
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout<<"thread_name :"<<thread_name<<std::endl;
    std::cout << "std::this_thread::get_id is : " << std::this_thread::get_id() << std::endl;
}

void Print_Str(std::string str) {
    char thread_name[256];
    pthread_getname_np(pthread_self(), thread_name, sizeof(thread_name));
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout<<"thread_name :"<<thread_name<<std::endl;
    std::cout << "std::this_thread::get_id is : " << std::this_thread::get_id() << std::endl;
    std::cout<<"str:"<<str<<std::endl;
}
int main(){
    int max_thread = 8;
    ThreadPool thread_poll(max_thread);
    std::cout<<"thread_num = "<<thread_poll.get_thread_num()<<std::endl;
    thread_poll.Start();
    for(int i = 0 ;i<max_thread;i++) {
        Task task = std::bind(Print);
        thread_poll.Run(task);
    }

    Task task = std::bind(Print_Str,std::string("hello "));
    thread_poll.Run(task);
    return 0;
    }