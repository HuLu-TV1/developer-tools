#ifndef __BUFFER__
#define __BUFFER__
#include "noncopyable.h"
#include "sockets_ops.h"
#include <algorithm>
#include <assert.h>
#include <string>
#include <vector>
/// A buffer class modeled after org.jboss.netty.buffer.ChannelBuffer
///
/// @code
/// +-------------------+------------------+------------------+
/// | prependable bytes |  readable bytes  |  writable bytes  |
/// |                   |     (CONTENT)    |                  |
/// +-------------------+------------------+------------------+
/// |                   |                  |                  |
/// 0      <=      readerIndex   <=   writerIndex    <=     size
/// @endcode
namespace net {
class Buffer : noncopyable {
public:
  static const size_t kCheapPrepend;
  static const size_t KInitiaSize;
  size_t ReadFd(int fd, int *SavedErrno);
  Buffer(size_t initial_size = KInitiaSize)
      : buffer_(kCheapPrepend + initial_size), read_index_(kCheapPrepend),
        write_index_(kCheapPrepend) {}
  size_t ReadableBytes() { return write_index_ - read_index_; }
  size_t WriteableBytes() { return buffer_.size() - write_index_; }
  size_t PrependableBytes() { return read_index_; }
  void Append(std::string str) { Append(str.c_str(), str.size()); }
  void Append(void *data, size_t len) {
    Append(static_cast<const char *>(data), len);
  }
  void Append(const char *data, size_t len) {
    EnsureWriteBytes(len);
    std::copy(data, data + len, BeginWrite());
    HasWrite(len);
  }
  void HasWrite(size_t len) { write_index_ += len; }

  void EnsureWriteBytes(size_t len) {
    if (WriteableBytes() < len) {
      MakeSpace(len);
    }
  }
  char *BeginWrite() { return Begin() + write_index_; }

  std::string RetrieveAllAsString() {
    return RetrieveAsString(ReadableBytes());
  }

  std::string RetrieveAsString(size_t len) {
    assert(len <= ReadableBytes());
    std::string tmp(Peek(), len);
    Retrieve(len);
    return tmp;
  }

  void Retrieve(size_t n) {
    if (ReadableBytes() > n)
      read_index_ += n;
    else {
      RetrieveAll();
    }
  }

  void RetrieveAll() {
    read_index_ = kCheapPrepend;
    write_index_ = kCheapPrepend;
  }
  const char *Peek() { return Begin() + read_index_; }

private:
  char *Begin() { return &*buffer_.begin(); }
  void MakeSpace(size_t len) {
    if ((WriteableBytes() + PrependableBytes()) >= len + kCheapPrepend) {
      size_t readable = ReadableBytes();
      std::copy(Begin() + read_index_, Begin() + write_index_,
                Begin() + kCheapPrepend);
      write_index_ = kCheapPrepend + readable;
      read_index_ = kCheapPrepend;
    } else {
      buffer_.resize(write_index_ + len);
    }
  }
  std::vector<char> buffer_;
  size_t read_index_;
  size_t write_index_;
};
} // namespace net

#endif