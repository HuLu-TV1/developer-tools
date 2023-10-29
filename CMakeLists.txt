cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(developer-tools LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(WORK_SPACE ${PROJECT_SOURCE_DIR})

set(COMMON_INC "include/common")

set(SRC
"include/common/common.h"
"include/common/cfile_name.h" 
"include/common/cfile.h"
"include/common/fileManager.h"
"include/common/log_queue.h"
"include/common/log.h"
"include/common/lrucache.hpp"
"include/common/ThreadPool.h"
"include/common/time_wheel.h"
"include/common/timer_list.h"
"src/common/cfile.cpp"
"src/common/fileManager.cpp" 
"src/common/log.cpp"
"src/common/ThreadPool.cpp"
)


add_library(developer-tools SHARED ${SRC})
target_include_directories(developer-tools PRIVATE ${COMMON_INC})
target_link_options(developer-tools PRIVATE -w -g -fstack-protector -fPIC)
target_link_libraries(developer-tools PRIVATE pthread)
