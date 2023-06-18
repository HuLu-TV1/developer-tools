#include <buffer.h>
#include <sys/uio.h>

#include <errno.h>
using namespace net;
const size_t Buffer::kCheapPrepend = 8;
const size_t Buffer::KInitiaSize = 1024;
size_t Buffer::ReadFd(int fd, int *SavedErrno) {
  char extra_buf[65536];
  struct iovec vec[2];
  const size_t writable = WriteableBytes();
  vec[0].iov_base = Begin() + write_index_;
  vec[0].iov_len = writable;
  vec[1].iov_base = extra_buf;
  vec[2].iov_len = sizeof(extra_buf);
  const size_t iovcnt = (writable < sizeof extra_buf) ? 2 : 1;
  const ssize_t n = readv(fd, vec, iovcnt);
  if (n < 0) {
    *SavedErrno = errno;
  } else if (implicit_cast<size_t>(n) <= writable) {
    write_index_ += n;
  } else {
    write_index_ = buffer_.size();
    Append(extra_buf, n - writable);
  }
  return n;
}