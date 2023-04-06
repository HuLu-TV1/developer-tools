cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(log)

set(INC log.h log_queue.h)

set(SRC log.cpp)

add_library(log SHARED ${SRC} ${INC})
target_link_libraries(log pthread)
