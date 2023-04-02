#pragma once

#include <mutex>
#include <string>
#include <thread>
#include <atomic>
#include <cstdarg>
#include <cstring>
#include "log.h"
#include "log_queue.h"


namespace CommonLog { 

class LoggerImpl: public Logger {
public:
    LoggerImpl() {}
    ~LoggerImpl();
    bool IsInited() {
        return is_inited_;
    }
    bool Init(LogType type=LogType::LOG_FILE, int buffer_queue_size=0, 
                int buffer_size=8192, int split_lines=5000) override;
    void AsyncFlush() override {
        this->AsyncWrite();
    }

    void WriteLog(const char* file_name, const char* callback_name, int line_no, LogLevel level, const char* format, ...) override;
    void Flush() override;

private:
    void AsyncWrite() {
        std::string single_line;
        while (buffer_queue_->Pop(single_line) && !is_thread_stop_) {
            std::lock_guard<std::mutex> lock(mutex_);
            fputs(single_line.c_str(), fp_);
        }
    }
    
private:
    std::string file_name_;
    std::string file_dir_;
    int split_lines_ = 0;
    int buf_size_ = 0;
    long long count_ = 0;
    FILE* fp_;
    char* buf_ = nullptr;
    bool is_inited_ = false;
    BufferQueue<std::string> *buffer_queue_ = nullptr;
    bool is_async_ = false;
    std::atomic<bool> is_thread_stop_{false};
    std::mutex mutex_;
    std::thread *async_thread_;
};

}