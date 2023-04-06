#include <chrono>
#include <vector>
#include <atomic>
#include "log.h"
#include "test_util.h"

const int KItemNum = 11000;
const int KTNum = 5;
std::atomic<int> KTCnt(0);

void log_async_test() {
    long long tid = tid_to_ll(std::this_thread::get_id());
    printf("[threa_%d]tid is: %lld\n", KTCnt.fetch_add(1), tid);
    for(int i=0; i<KItemNum; i++)
    {
        LOG_INFO("[tid:%lld] log async test: %d\n", tid, i);
    }
}

int main() {
    CommonLog::Logger::GetInstance()->Init(CommonLog::LogType::LOG_FILE, 1);
    std::vector<std::thread> threads;
    printf("sync log test started\n");
    printf("start to log\n");
    auto start = std::chrono::steady_clock::now();
    for(int i=0; i<KTNum; i++)
    {
        threads.emplace_back(log_async_test);       
    }
    for(int i=0; i<KTNum; i++)
    {
        threads[i].join();   
    }
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    printf("end logging\n"); 
    printf("totally write %d items into files\n", KTNum * KItemNum);
    printf("costed time: %d ms\n" ,static_cast<int>(duration.count()));
    return 0;
}