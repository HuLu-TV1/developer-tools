#include "timer_list.h"
#include <functional>
#include <signal.h>
#include <unistd.h>

const int KTimeOut = 5;

Timer_list timer_list;
void handle_alarm(int signal) {
  timer_list.Tick();
  alarm(KTimeOut);
}

void test() {
  TimerNode *timer_1 = new TimerNode();
  timer_1->expired_ = 1;

  TimerNode *timer_2 = new TimerNode();
  timer_2->expired_ = 2;

  TimerNode *timer_5 = new TimerNode();
  timer_5->expired_ = 5;

  TimerNode *timer_3 = new TimerNode();
  timer_3->expired_ = 3;

  TimerNode *timer_6 = new TimerNode();
  timer_6->expired_ = 6;

  TimerNode *timer_4 = new TimerNode();
  timer_4->expired_ = 4;
  TimerNode *timer_7 = new TimerNode();
  timer_4->expired_ = 7;

  std::cout << "****************Add_Timer************" << std::endl;

  timer_list.Add_Timer(timer_2);
  timer_list.Add_Timer(timer_1);
  timer_list.Add_Timer(timer_3);
  timer_list.Add_Timer(timer_4);
  timer_list.Add_Timer(timer_5);
  timer_list.Add_Timer(timer_6);

  timer_list.Show();
  std::cout << "****************Del_Timer************" << std::endl;
  timer_list.Del_Timer(timer_1);
  timer_list.Del_Timer(timer_2);
  // timer_list.Del_Timer(timer_3);//需要执行mod测试
  timer_list.Del_Timer(timer_4);
  timer_list.Del_Timer(timer_5);
  timer_list.Del_Timer(timer_6);
  // timer_list.Del_Timer(timer_7);
  timer_list.Show();
  std::cout << "****************Mod_Timer************" << std::endl;
  timer_3->expired_ = 10;
  timer_list.Mod_Timer(timer_3);
  timer_list.Show();
}

void print_time(int socket) {
  std::cout << "****************print_time************" << std::endl;
  std::cout << "socket " << socket << std::endl;
  time_t now_time = time(nullptr);
  printTime(now_time);
}

void test_tick() {

  std::cout << "****************test_tick************" << std::endl;
  signal(SIGALRM, handle_alarm);
  alarm(KTimeOut);

  time_t now_time = time(nullptr);
  printTime(now_time);
  TimerNode *timer_1 = new TimerNode();
  timer_1->expired_ = now_time + KTimeOut;
  printTime(timer_1->expired_);
  timer_1->socket_ = 1;
  timer_1->cb_func_ = std::bind(print_time, timer_1->socket_);

  TimerNode *timer_2 = new TimerNode();
  timer_2->expired_ = timer_2->expired_ = now_time + 2 * KTimeOut;
  printTime(timer_2->expired_);
  timer_2->socket_ = 2;
  timer_2->cb_func_ = std::bind(print_time, timer_2->socket_);

  TimerNode *timer_5 = new TimerNode();
  timer_5->expired_ = timer_5->expired_ = now_time + 5 * KTimeOut;
  printTime(timer_5->expired_);
  timer_5->socket_ = 5;
  timer_5->cb_func_ = std::bind(print_time, timer_5->socket_);

  timer_list.Add_Timer(timer_1);
  timer_list.Add_Timer(timer_2);
  timer_list.Add_Timer(timer_5);

  while (1) {
  }
}
int main() {
  //test();
  // test_tick();
  return 0;
}