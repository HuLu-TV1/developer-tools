#include "cfile.h"
#include "fileManager.h"

CFile::CFile(const file_event_handlers& event_handlers)
    : event_handlers_(event_handlers)
{}

CFile::~CFile() {
    Close();
}

void CFile::Open(const std::string& fname, bool truncate) {
    Close();
    filename_ = fname;
    if (event_handlers_.before_open) event_handlers_.before_open(filename_);
    FileManager::createFile(fname);
    if (truncate) {
        std::FILE *tmp;
        tmp = ::fopen(fname.c_str(), "wb");
        std::fclose(tmp);
    }
    fd_ = ::fopen(fname.c_str(), "ab");
    if (!fd_) {
        if (event_handlers_.after_open) {
            event_handlers_.after_open(filename_, fd_);
        }
    }
}

void CFile::Reopen(bool truncate) {
    Open(filename_, truncate);
}

void CFile::Flush() {
    std::fflush(fd_);
}

void CFile::Sync() {
    ::fsync(fileno(fd_));
}

void CFile::Close() {
    if (fd_ != nullptr) {
       if (event_handlers_.before_close)
        {
            event_handlers_.before_close(filename_, fd_);
        }
        std::fclose(fd_);
        fd_ = nullptr;

        if (event_handlers_.after_close)
        {
            event_handlers_.after_close(filename_);
        }
    }
}

void CFile::Write(void* data, size_t len)
{
    std::fwrite((char*)data, 1, len, fd_);
}

size_t CFile::Size() const {
    if (fd_ == nullptr) return 0;
    int fd = ::fileno(fd_);
    struct stat64 st;
    if (::fstat64(fd, &st) == 0) {
        return static_cast<size_t>(st.st_size);
    }
    return 0;
}

const std::string& CFile::Filename() const {
    return filename_;
}