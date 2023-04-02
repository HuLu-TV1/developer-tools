cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(log)

set(INC log.h log_impl.h)

set(SRC log_impl.cpp)

add_library(log SHARED ${SRC} ${INC})
