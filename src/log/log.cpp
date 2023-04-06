#include <sys/stat.h>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <iostream>
#include "log.h"

using namespace CommonLog;

const int PATH_MAX = 255;

const char *KLogLevelName[LogLevel::LOG_NUM] = {
    "[ERROR]",
    "[WARN]",
    "[INFO]",
    "[DEBUG]",
};

std::string GetLogPath() {
    char path[PATH_MAX]{0};
    struct stat s;
    char *user_home = getenv("HOME");
    if (user_home != nullptr) {
        snprintf(path, PATH_MAX, "%s/.dlog", user_home);
    }
    if (stat(path, &s) == -1 || (s.st_mode & S_IFMT) != S_IFDIR) {
        mkdir(path, S_IRWXU);
    }
    return std::string(path);
}

std::string GetLogName() {
    std::stringstream ss;
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    ss << std::put_time(&tm, "log_%Y-%m-%d_%H-%M-%S.log");
    return ss.str();
}

std::string GetCurrentTime() {
    std::stringstream ss;
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

Logger* Logger::GetInstance() {
    static Logger log;
    return &log;
}

Logger::~Logger(){
    if (async_thread_) {
        is_thread_stop_ = true;
        if (async_thread_->joinable()) {
            buffer_queue_->notify();
            async_thread_->join();
        }
        delete async_thread_;
    }

    std::lock_guard<std::mutex> lock(mutex_);
    if (fp_ != nullptr) {
        fclose(fp_);
    }

    if (buf_) {
        delete [] buf_;
    }

    if (buffer_queue_) {
        delete buffer_queue_;
    }
}

bool Logger::Init(LogType type, int buffer_queue_size, 
                int buffer_size, int split_lines) {
    if (is_inited_) {
        std::cout << "Logger has been initialized, do not try again!" << std::endl;
        return false;
    }

    if (buffer_queue_size >= 1) {
        buffer_queue_ = new BufferQueue<std::string>(buffer_queue_size);
        is_async_ = true;
        async_thread_ = new std::thread(&Logger::AsyncFlush, this);
    }

    buf_size_ = buffer_size;
    buf_  = new char[buf_size_];
    memset(buf_, '\0', buf_size_);
    split_lines_ = split_lines;

    if (type == LogType::LOG_PRINT) {
        fp_ = stdout;
        is_inited_ = true;
        is_std_ = true;
        return true;
    }
    file_dir_ = GetLogPath();
    file_name_ = GetLogName();
    std::string full_file = file_dir_ + "/" + file_name_;
    fp_ = fopen(full_file.c_str(), "a");

    if (fp_ == nullptr) {
        std::cout << "open file failed!" << std::endl;
        return false;
    }
    is_inited_ = true;
    return true;
}

void Logger::WriteLog(const char* file_name, const char* callback_name, int line_no, LogLevel level, const char* format, ...) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        count_++;
        if (count_ % split_lines_ == 0 && !is_std_) {
            std::cout << "start to create a new log file\n" <<std::endl;
            fflush(fp_);
            fclose(fp_);
            std::string new_file = file_dir_ + "/" + GetLogName();
            fp_ = fopen(new_file.c_str(), "a");
        }
    }
    va_list valist;
    va_start(valist, format);
    std::string log_str;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        std::string current_time = GetCurrentTime();
        int n = snprintf(buf_, 300, "%s %s [%s:%s:%d] ", current_time.c_str(), KLogLevelName[level],
                    file_name, callback_name, line_no);
        int m = vsnprintf(buf_+n, buf_size_-1, format, valist);
        // buf_[n+m] = '\n';
        buf_[n+m+1] = '\0';
        log_str = buf_;
    }
    va_end(valist);
    if (is_async_) {
        while (!buffer_queue_->Push(log_str) && !is_thread_stop_) {}
    }
    else {
        std::lock_guard<std::mutex> lock(mutex_);
        fputs(log_str.c_str(), fp_);
    }
}

void Logger::Flush() {
    std::lock_guard<std::mutex> lock(mutex_);
    fflush(fp_);
}
