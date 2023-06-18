#ifndef _BASE_NONCOPYABLE_H
#define _BASE_NONCOPYABLE_H

class noncopyable {
public:
  noncopyable(const noncopyable &) = delete;
  void operator=(const noncopyable &) = delete;

protected:
  noncopyable() = default;
  ~noncopyable() = default;
};

#endif // MUDUO_BASE_NONCOPYABLE_H
