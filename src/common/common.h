#pragma once 

#include <functional>
#include <string>
#include <cstdio>

#define CAPI

struct file_event_handlers {
    file_event_handlers()
        : before_open(nullptr)
        , after_open(nullptr)
        , before_close(nullptr)
        , after_close(nullptr)
    {}

    std::function<void(const std::string& filename)> before_open;
    std::function<void(const std::string& filename, std::FILE* file_stream)> after_open;
    std::function<void(const std::string& filename, std::FILE* file_stream)> before_close;
    std::function<void(const std::string& filename)> after_close;
};