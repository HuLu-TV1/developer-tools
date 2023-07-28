#pragma once
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

const int PATH_MAX = 4096;

using CFileNameStr = CFileName<char>;

template<typename T>
class CFileName {
public:
    using base_type = std::basic_string<T>;
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    CFileName() = default;
    CFileName(const base_type& name): data_(name) {}
    CFileName(const_pointer name): data_(name) {}
    CFileName(const_pointer name, size_t n): data_(name, n) {}
    CFileName(const CFileName& rhs): data_(rhs.data_) {}
    CFileName& operator=(const CFileName& rhs) { 
        data_ = rhs.data_;
        return *this;
    }
    ~CFileName() {}
    size_t GetLength() const { return data_.size(); }
    pointer begin() { return &data_[0]; }
    const_pointer begin() const { return &data_[0]; }
    pointer end() { return &data_[GetLength()]; }
    const_pointer end() const { return &data_[GetLength()]; }
    base_type GetExt() const {
        for(auto iter = end(); iter != begin(); ) {
            T v = *--iter;
            if (v == '.') {
                return base_type(iter+1, end());
            }
            if (v == '\\' || v == '/') break;
        }
        return base_type(end(), end());
    }

    base_type GetPath() const {
        for (auto iter = end(); iter != begin(); ) {
            T v = *--iter;
            if (v == '\\' || v == '/') {
                return base_type(begin(), iter);
            }
        }
        return base_type(begin(), begin());
    }

    base_type GetPathBody() const {
        if (GetExt() == "") return data_;
        int length = GetLength() - GetExt().size() - 1;
        return base_type(begin(), length);
    }
    
    base_type GetBody() const {
        auto itend = end();
        for (auto iter = itend; iter != begin(); ) {
            T v = *--iter;
            if (v == '.') itend = iter;
            if (v == '\\' || v == '/') {
                return base_type(iter+1, itend);
            }
        }
        return base_type(begin(), itend);
    }

    static bool FoldExists(const char* path) {
        struct stat s;
        if (stat(path, &s) == -1) return false;
        return ((s.st_mode & S_IFMT) == S_IFDIR);
    }

    static bool CreateFold(const char* path) {
        return !mkdir(path, S_IRWXU);   
    }

    static void GetHomePath(char* path) {
        if (path == nullptr) return;
        char home[PATH_MAX]{0};
        char* user_home = getenv("HOME");
        if (user_home) {
            snprintf(home, PATH_MAX, "%s/.ot", user_home);
        }
        if (realpath(home, path)) {
            snprintf(path, PATH_MAX, "%s", home);
        }
        if (!FoldExists(path)) {
            CreateFold(path);
        }
    }

    void GetTempPath() {
        char path[PATH_MAX]{0};
        GetHomePath(path);
        strcat(path, "/data");
        data_ = base_type(path);
    }

private:
    base_type data_;
};


