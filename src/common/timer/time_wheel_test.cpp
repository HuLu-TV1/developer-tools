#include "time_wheel.h"
#include <functional>
#include <signal.h>
#include <unistd.h>
TimeWheel time_wheel;

void test_basic_function() {
  TimerNode *tm_node1 = new TimerNode(115);
  TimerNode *tm_node2 = new TimerNode(120);
  TimerNode *tm_node3 = new TimerNode(1345);
  TimerNode *tm_node4 = new TimerNode(11234);
  TimerNode *tm_node5 = new TimerNode(15512);
  TimerNode *tm_node6 = new TimerNode(0);

  std::cout << "**********AddTimer*********" << std::endl;
  time_wheel.AddTimer(tm_node1);
  time_wheel.AddTimer(tm_node2);
  time_wheel.AddTimer(tm_node3);
  time_wheel.AddTimer(tm_node4);
  time_wheel.AddTimer(tm_node5);
  time_wheel.AddTimer(tm_node6);
  time_wheel.Show();

  std::cout << "**********ModTimer*********" << std::endl;

  time_wheel.ModTimer(tm_node1, 10);
  time_wheel.ModTimer(tm_node2, 1);
  time_wheel.ModTimer(tm_node6, 60);
  time_wheel.ModTimer(tm_node5, 76);
  time_wheel.Show();

  std::cout << "**********DelTimer*********" << std::endl;
  time_wheel.DelTimer(tm_node1);
  time_wheel.DelTimer(tm_node2);
  time_wheel.DelTimer(tm_node6);
  time_wheel.DelTimer(tm_node5);
  time_wheel.Show();
}

void print_time(int timeout) {
  std::cout << "****************print_time************" << std::endl;
  std::cout << "timeout " << timeout << std::endl;
  time_t now_time = time(nullptr);
  printTime(now_time);
}

void handle_alarm(int signal) {
  time_wheel.Tick();
  alarm(time_wheel.kSi);
}

void test_tick() {
  signal(SIGALRM, handle_alarm);
  alarm(time_wheel.kSi);
  time_t now_time = time(nullptr);
  TimerNode *tm_node1 = new TimerNode(1);
  printTime(now_time + 1);
  tm_node1->cb_func_ = std::bind(print_time, tm_node1->timeout_);

  TimerNode *tm_node2 = new TimerNode(5);
  printTime(now_time + 5);
  tm_node2->cb_func_ = std::bind(print_time, tm_node2->timeout_);

  TimerNode *tm_node3 = new TimerNode(10);
  printTime(now_time + 10);
  tm_node3->cb_func_ = std::bind(print_time, tm_node3->timeout_);

  std::cout << "**********AddTimer*********" << std::endl;
  time_wheel.AddTimer(tm_node1);
  time_wheel.AddTimer(tm_node2);
  time_wheel.AddTimer(tm_node3);
  time_wheel.Show();
  std::cout << "**********test_tick*********" << std::endl;
   while(1){}
}
int main() {
  test_tick();
  return 0;
}