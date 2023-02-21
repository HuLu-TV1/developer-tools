#I/O多路复用
##什么是网络编程
网络编程说白了就是两台设备之间进行数据交换。
通常所说的网络编程涉及到的API通常是socket、bind、listen、accept、connect等。使用以上接口就可以写一个简单的网络程序。
那么为什么还要引入I/O多路复用的概念？
若不使用I/O多路复用会出现如下问题：
1. 在不使用多线程的情况下，accpet监听到有client请求后返回一个cln_fd，然后程序read(cln_fd)->业务逻辑->write(cln_fd)。这个过程中可能会很耗时，那么这个时候有新的连接请求发生时，server端不能快速建立连接，因为此时程序在处理业务逻辑。
2. 在使用对线程情况下，我们可以将业务逻辑部份放在多线程中进行处理，但是在线程中server端调用read时，client端迟迟不写数据那么就会阻塞在read处，线程阻塞无事可做大大浪费了资源。
#Reactor
本网络库参考陈硕的muduo，使用Reactor模式。
Reactor即I/O多路复用监听事件，收到事件，根据事件的类型分配给某个进程/线程处理。
Reactor主要分为两部分Reactor与事件处理器
Reactor：负责监听事件和分发事件
事件处理器：负责具体事件的操作如read->业务逻辑->write
Reactor相关内容可以查看
https://www.zhihu.com/question/26943938/answer/1856426252
