#pragma once
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <atomic>
#include <cstdarg>
#include <cstring>
#include "log_queue.h"
#include <iostream>
namespace CommonLog {

enum class LogType {
    LOG_FILE,
    LOG_PRINT,
};

enum LogLevel {
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
    LOG_DEBUG,
    LOG_NUM,
};

class Logger {
public:
    static Logger* GetInstance();
    ~Logger();
    bool IsInited() {
        return is_inited_;
    }
    //If the type is LOG_PRINT, the paramter split_lines is valid.
    bool Init(LogType type=LogType::LOG_FILE, int buffer_queue_size=0, 
                int buffer_size=8192, int split_lines=5000);
    void AsyncFlush() {
        this->AsyncWrite();
    }

    void WriteLog(const char* file_name, const char* callback_name, int line_no, LogLevel level, const char* format, ...);
    void Flush();
private:
    Logger() = default;

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
    bool is_std_ = false;
    std::atomic<bool> is_thread_stop_{false};
    std::mutex mutex_;
    std::thread *async_thread_;
};


}

#define LOG_DEBUG(format, ...) \
    do {                       \
        if (!CommonLog::Logger::GetInstance()->IsInited())   \
            std::cout << "logger must be inited before use!" << std::endl; \
        else \
        {                                                     \
            CommonLog::Logger::GetInstance()->WriteLog(__FILE__, __FUNCTION__, __LINE__, CommonLog::LogLevel::LOG_DEBUG, format, ##__VA_ARGS__); \
            CommonLog::Logger::GetInstance()->Flush();                    \
        }                                                                       \
    } while(0)
    

#define LOG_INFO(format, ...) \
    do {                       \
        if (!CommonLog::Logger::GetInstance()->IsInited())   \
            std::cout << "logger must be inited before use!" << std::endl; \
        else \
        {                                                     \
            CommonLog::Logger::GetInstance()->WriteLog(__FILE__, __FUNCTION__, __LINE__, CommonLog::LogLevel::LOG_INFO, format, ##__VA_ARGS__); \
            CommonLog::Logger::GetInstance()->Flush();                    \
        }                                                                       \
    } while(0)

#define LOG_WARN(format, ...) \
    do {                      \
        if (!CommonLog::Logger::GetInstance()->IsInited())   \
            std::cout << "logger must be inited before use!" << std::endl; \
        else \
        {                                                     \
            CommonLog::Logger::GetInstance()->WriteLog(__FILE__, __FUNCTION__, __LINE__, CommonLog::LogLevel::LOG_WARN, format, ##__VA_ARGS__); \
            CommonLog::Logger::GetInstance()->Flush();                    \
        }                                                                       \
    } while(0)

#define LOG_ERROR(format, ...) \
    do {                       \
        if (!CommonLog::Logger::GetInstance()->IsInited())   \
            std::cout << "logger must be inited before use!" << std::endl; \
        else \
        {                                                     \
            CommonLog::Logger::GetInstance()->WriteLog(__FILE__, __FUNCTION__, __LINE__, CommonLog::LogLevel::LOG_ERROR, format, ##__VA_ARGS__); \
            CommonLog::Logger::GetInstance()->Flush();                    \
        }                                                                       \
    } while(0)
