
#include "common.h"

class CAPI CFile {
public:
    CFile() = default;
    explicit CFile(const file_event_handlers& event_handlers);
    CFile(const CFile&) = delete;
    CFile& operator=(const CFile&) = delete;
    ~CFile();

    void Open(const std::string& fname, bool truncate = false);
    void Reopen(bool truncate);
    void Flush();
    void Sync();
    void Close();
    void Write(void* data, size_t len);
    size_t Size() const;
    const std::string& Filename() const;

private:
    std::FILE *fd_{nullptr};
    std::string filename_;
    file_event_handlers event_handlers_;
};