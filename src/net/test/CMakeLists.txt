#需要在根目录下添加，表示开启项目的测试功能
enable_testing()

include_directories(../src/include)
add_executable(server_unittest server_unittest.cpp)
target_link_libraries(server_unittest net_demo )
#################


add_executable(thread_pool_unittest thread_pool_unittest.cpp)
target_link_libraries(thread_pool_unittest net_demo )

#################

add_executable(server_echo_unittest server_echo_unittest.cpp)
target_link_libraries(server_echo_unittest net_demo )
#################

# add_executable(client_unittest client_unittest.cpp)
# target_link_libraries(client_unittest net_demo )

if(BOOSTTEST_LIBRARY)
add_executable(buffer_unittest buffer_unittest.cpp)
# target_link_libraries(boost_unit_test_framework)
target_link_libraries(buffer_unittest  PUBLIC net_demo  ${CMAKE_SOURCE_DIR}/thirdfile/lib/libboost_unit_test_framework.so)
add_test(NAME buffer_unittest COMMAND buffer_unittest)
endif()
#################