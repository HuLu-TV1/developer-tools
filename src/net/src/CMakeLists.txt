cmake_minimum_required(VERSION 3.10)
project(cmake_test VERSION 1.0.0 LANGUAGES C CXX)

file (GLOB_RECURSE SRC_DIR_LIST "./*.cpp")
# message( "SRC_DIR_LIST: ${SRC_DIR_LIST}")




#设置动态库路径
set(need_library ${CMAKE_SOURCE_DIR}/thirdfile/lib)
message( "need_library: ${need_library}")

add_library(net_demo ${SRC_DIR_LIST})
#设置public 代码是公开的这样依赖net_demo的文件也可以找到liblog.so
target_include_directories(net_demo PUBLIC ./include ${CMAKE_SOURCE_DIR}/thirdfile/include)
target_link_libraries(net_demo PUBLIC ${CMAKE_SOURCE_DIR}/thirdfile/lib/liblog.so)

message( "PROJECT_BINARY_DIR: ${PROJECT_BINARY_DIR}")


#install(TARGETS muduo_net DESTINATION lib)
message( "CMAKE_LIBRARY_OUTPUT_DIRECTORY: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
# set_target_properties(net_demo PROPERTIES OUTPUT_NAME "net_demo")
# install(TARGETS net_demo ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)


