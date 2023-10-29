#pragma once
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <atomic>
#include <cstdarg>
#include <cstring>
#include <sstream>
#include "log_queue.h"

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

enum LoggingDestinationOption {
    LOG_TO_NONE = 0,
    LOG_TO_FILE = 1 << 0,
    LOG_TO_FILE_RESET = 1 << 1,
    LOG_TO_SYSTEM_LOG = 1 << 2,
    LOG_DEFAULT = LOG_TO_SYSTEM_LOG,
};

inline LogLevel GetMaxLogLevel() {
    return LOG_NUM;
} 

void InitLogSetting(LoggingDestinationOption destination);

typedef int LogSeverity;

class LogMessageVoidify {
    public:
        LogMessageVoidify() { }
        // This has to be an operator with a precedence lower than << but
        // higher than ?:
        void operator&(std::ostream&) { }
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

class LogMessage {
public:
    // Used for LOG(severity).
    LogMessage(const char* file, int line, LogSeverity severity);
    LogMessage(const LogMessage& ) = delete;
    LogMessage& operator=(const LogMessage&) = delete;

    ~LogMessage();

    void InitLog();

    std::ostream& stream() { return stream_; }

private:
    std::ostringstream stream_;
    LogSeverity severity_;
    std::mutex file_mutex_;
    const char* file_;
    const int line_;
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

#define COMPACT_OPENTE_LOG_EXT(severity, ClassName, ...) \
    ::CommonLog::ClassName(__FILE__, __LINE__, \
    ::CommonLog::LOG_##severity, ##__VA_ARGS__)
#define COMPACT_OPENTE_LOG(severity) COMPACT_OPENTE_LOG_EXT(severity, LogMessage)
#define LOG_STREAM(severity) COMPACT_OPENTE_LOG(severity).stream()

#define LAZY_STREAM(stream, condition)                                  \
    !(condition) ? (void) 0 : ::CommonLog::LogMessageVoidify() & (stream)

#define LOG_IS_ON(severity) \
    ((::CommonLog::LogLevel::LOG_ ## severity) <= ::CommonLog::GetMaxLogLevel())

#define CLOG(severity,msg)	LAZY_STREAM(LOG_STREAM(severity), LOG_IS_ON(severity)) << msg << " "