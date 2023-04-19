objs = ThreadPool.o main.o
main:$(objs)
	g++ -pthread -o main $(objs)
%.o:%.cpp
	g++ -pthread -c $<
