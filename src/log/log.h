#pragma once
#include <memory>

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

const char *KLogLevelName[LogLevel::LOG_NUM] = {
    "[ERROR]",
    "[WARN ]",
    "[INFO ]",
    "[DEBUG]",
};

class Logger {
public:
    static std::shared_ptr<Logger> GetSharedInstance();
    Logger() = default;
    Logger(const Logger&) = default;
    Logger& operator=(const Logger&) = default;
    virtual ~Logger() {}

    virtual bool IsInited() = 0;
    virtual bool Init(LogType type, int buffer_queue_size, 
                int buffer_size, int split_lines) = 0;  
    virtual void AsyncFlush() = 0;
    virtual void WriteLog(const char* file_name, const char* callback_name, int line_no, LogLevel level, const char* format, ...) = 0;
    virtual void Flush() = 0;
};

}

#define LOG_DEBUG(format, ...) \
    do {                       \
        if (!CommonLog::Logger::GetSharedInstance()->IsInited())   \
            std::cout << "logger must be inited before use!" << std::endl; \
        else \
        {                                                     \
            CommonLog::Logger::GetSharedInstance()->WriteLog(__FILE__, __FUNCTION__, __LINE__, LogLevel::LOG_DEBUG, format, ##__VA_ARGS__); \
            CommonLog::Logger::GetSharedInstance()->Flush();                    \
        }                                                                       \
    } while(0)
    

#define LOG_INFO(format, ...) \
    do {                       \
        if (!CommonLog::Logger::GetSharedInstance()->IsInited())   \
            std::cout << "logger must be inited before use!" << std::endl; \
        else \
        {                                                     \
            CommonLog::Logger::GetSharedInstance()->WriteLog(__FILE__, __FUNCTION__, __LINE__, LogLevel::LOG_INFO, format, ##__VA_ARGS__); \
            CommonLog::Logger::GetSharedInstance()->Flush();                    \
        }                                                                       \
    } while(0)

#define LOG_WARN(format, ...) \
    do {                      \
        if (!CommonLog::Logger::GetSharedInstance()->IsInited())   \
            std::cout << "logger must be inited before use!" << std::endl; \
        else \
        {                                                     \
            CommonLog::Logger::GetSharedInstance()->WriteLog(__FILE__, __FUNCTION__, __LINE__, LogLevel::LOG_WARN, format, ##__VA_ARGS__); \
            CommonLog::Logger::GetSharedInstance()->Flush();                    \
        }                                                                       \
    } while(0)

#define LOG_ERROR(format, ...) \
    do {                       \
        if (!CommonLog::Logger::GetSharedInstance()->IsInited())   \
            std::cout << "logger must be inited before use!" << std::endl; \
        else \
        {                                                     \
            CommonLog::Logger::GetSharedInstance()->WriteLog(__FILE__, __FUNCTION__, __LINE__, LogLevel::LOG_ERROR, format, ##__VA_ARGS__); \
            CommonLog::Logger::GetSharedInstance()->Flush();                    \
        }                                                                       \
    } while(0)
