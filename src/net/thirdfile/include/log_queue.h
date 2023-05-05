#pragma once

#include <vector>
#include <mutex>
#include <condition_variable>
#include <cassert>
#include <stdexcept>

template<typename T>
class BufferQueue {
public:
    BufferQueue(): array_(new std::vector<T>[capacity_]()) {}
    explicit BufferQueue(int max_size) {
            if (max_size <= 0) throw std::invalid_argument("max_size is illegal!");
            capacity_ = max_size;
            array_ = new std::vector<T>();
            array_->reserve(capacity_);
        }
    BufferQueue(const BufferQueue& other) {
        std::lock_guard<std::mutex> lock(mutex_);
        first_ = other.first_;
        last_ = other.last_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        array_ = new std::vector<T>();
        array_->reserve(capacity_);
        std::copy(other.array_->begin(), other.array_->end(), array_->begin());
    } 

    BufferQueue& operator=(const BufferQueue& other) {
        std::lock_guard<std::mutex> lock(mutex_);
        first_ = other.first_;
        last_ = other.last_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        std::vector<T> *tmp = array_;
        array_ = new std::vector<T>();
        array_->reserve(capacity_);
        std::copy(other.array_->begin(), other.array_->end(), array_->begin());
        delete tmp;
        tmp = nullptr;
    }

    ~BufferQueue() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (array_) {
            delete array_;
            array_ = nullptr;
        }
    }

    void Clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        first_ = -1;
        last_ = -1;
        size_ = 0;
    }

    int GetSize() const {
        std::lock_guard<std::mutex> lock(mutex_);
        assert(size_ >= 0 && size_ <= capacity_);
        return size_;
    }

    bool IsFull() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return size_ == capacity_ ? true : false;
    }

    bool IsEmpty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return size_ == 0 ? true : false;
    }

    int GetCapacity() const {
        return capacity_;
    }

    bool Front(T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (size_ == 0) return false;
        value = (*array_)[first_+1];
        return true;
    }

    bool Back(T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (size_ == 0) return false;
        value = (*array_)[last_];
        return true;
    }

    bool Push(const T& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (size_ >= capacity_) {
            cond_.notify_all();
            return false;
        }
        last_ = (last_ + 1) % capacity_;
        (*array_)[last_] = value;
        size_++;
        cond_.notify_all();
        return true;
    }

    bool Pop(T& value) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (size_ <= 0) {
            cond_.wait(lock);
            if (size_ <= 0) return false;
        }
        first_ = (first_ + 1) % capacity_;
        value = (*array_)[first_];
        size_--;
        return true;
    }

    bool Pop(T& value, int ms_timeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (size_ <= 0) {
            if (cond_.wait_for(lock, std::chrono::milliseconds(ms_timeout)) == std::cv_status::timeout)
                return false;
        }
        if (size_ <= 0) return false;
        first_ = (first_ + 1) % capacity_;
        value = (*array_)[first_];
        size_--;
        return true;
    }

    void notify() {
        cond_.notify_all();
    }

private:
    mutable std::mutex mutex_;
    std::condition_variable cond_;
    std::vector<T>* array_;
    int first_ = -1;
    int last_ = -1;
    int size_ = 0;
    int capacity_ = 1000;
};