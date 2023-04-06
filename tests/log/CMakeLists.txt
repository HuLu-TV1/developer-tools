cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(log_test)

set(LOG_DIR ${PROJECT_SOURCE_DIR}/../../src/log)
include_directories(${LOG_DIR})

set(INC test_util.h)

set(SRC test_queue.cpp)

SET(CMAKE_BUILD_TYPE "Debug") 
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

add_executable(queue_test ${SRC} ${INC})
target_link_libraries(queue_test pthread)

set(SRC test_log_sync.cpp ${LOG_DIR}/log.cpp)

SET(CMAKE_BUILD_TYPE "Debug") 
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")


add_executable(log_sync_test ${SRC} ${INC})
target_link_libraries(log_sync_test pthread)

set(SRC test_log_async.cpp ${LOG_DIR}/log.cpp)

SET(CMAKE_BUILD_TYPE "Debug") 
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")


add_executable(log_async_test ${SRC} ${INC})
target_link_libraries(log_async_test pthread)

set(SRC test_log_stdout.cpp ${LOG_DIR}/log.cpp)

SET(CMAKE_BUILD_TYPE "Debug") 
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")


add_executable(log_stdout_test ${SRC} ${INC})
target_link_libraries(log_stdout_test pthread)