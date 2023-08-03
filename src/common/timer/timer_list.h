#ifndef __TIME_LIST__
#define __TIME_LIST__
#include <cstring>
#include <functional>
#include <iostream>
#include <time.h>
#include <unistd.h>

void printTime(time_t t) {
  struct tm timeinfo;
  localtime_r(&t, &timeinfo); // localtime_r 函数是线程安全版本的 localtime

  char buffer[80];
  strftime(buffer, sizeof(buffer), "%H:%M:%S", &timeinfo);
  std::cout << "时分秒： " << buffer << std::endl;
}

class TimerNode;
// struct ClientData {
//   int socket_;
//   TimerNode *time_node_;
// };
using Func = std::function<void()>;
class TimerNode {
public:
  TimerNode()
      : pre_(nullptr), next_(nullptr), cb_func_() {}

public:
  Func cb_func_;
  time_t expired_;
  TimerNode *pre_;
  TimerNode *next_;
//   ClientData *client_data_;
  int socket_;
};
class Timer_list {
public:
  Timer_list() {
    head_ = (TimerNode *)malloc(sizeof(TimerNode));
    tail_ = (TimerNode *)malloc(sizeof(TimerNode));
    head_->next_ = tail_;
    head_->pre_ = nullptr;
    tail_->next_ = nullptr;
    tail_->pre_ = head_;
  }
  ~Timer_list() {
    TimerNode *tmp = head_->next_;
    while (tmp!=tail_) {
      TimerNode *delete_node = tmp;
      tmp = tmp->next_;
      free(delete_node);
      delete_node = nullptr;
    }
    free(head_);
    free(tail_);
  }
  void Add_Timer(TimerNode *current_time_node) {
    if (head_->next_ == nullptr) {
      head_->next_ = current_time_node;
      current_time_node->pre_ = head_;
      tail_->pre_ = current_time_node;
      current_time_node->next_ = tail_;
      return;
    }
    TimerNode *tmp = head_->next_;
    while (tmp != tail_) {
      if (tmp->expired_ > current_time_node->expired_) {
        current_time_node->next_ = tmp;
        current_time_node->pre_ = tmp->pre_;
        tmp->pre_->next_ = current_time_node;
        tmp->pre_ = current_time_node;
        return;
      }
      tmp = tmp->next_;
    }
    if (tmp == tail_) {
      current_time_node->next_ = tail_;
      current_time_node->pre_ = tail_->pre_;
      tail_->pre_->next_ = current_time_node;
      tail_->pre_ = current_time_node;
    }
  }

  bool Del_Timer(TimerNode *current_time_node) {
    current_time_node->pre_->next_ = current_time_node->next_;
    current_time_node->next_->pre_ = current_time_node->pre_;
    free(current_time_node);
    current_time_node = nullptr;
    return false;
  }

  void Show() {
    // std::cout<<"head_->expired"<<head_->expired_<<std::endl;
    // std::cout<<"&head_"<<&head_<<std::endl;
    TimerNode *tmp = head_->next_;
    while (tmp != tail_) {
      std::cout << "tmp->expired" << tmp->expired_ << std::endl;
      tmp = tmp->next_;
      //std::cout << "&tmp" << &tmp << std::endl;
    }
    // std::cout<<"tail_->expired"<<tail_->expired_<<std::endl;
    // std::cout<<"&tail_"<<&tail_<<std::endl;
  }
  bool Mod_Timer(TimerNode *current_time_node) {
    if(current_time_node==nullptr) {
        std::cout<<"current node has deleted"<<std::endl;
    }
    current_time_node->next_->pre_ = current_time_node->pre_;
    current_time_node->pre_->next_ = current_time_node->next_;
    Add_Timer(current_time_node);
    return false;
  }
  void Tick() {
    time_t now_time = time(nullptr);
    // printTime(now_time);
    TimerNode *tmp = head_->next_;
    TimerNode *next = nullptr;
    // printTime(tmp->expired_);
    while (head_->next_ != tail_) {
      if (tmp->expired_ > now_time) {
        break;
      }
      next = tmp->next_;
      tmp->cb_func_();
      tmp->pre_->next_ = tmp->next_;
      tmp->next_->pre_ = tmp->pre_;
      free(tmp);
      tmp = next;
    }
  }

public:
  TimerNode *head_;
  TimerNode *tail_;
};
#endif