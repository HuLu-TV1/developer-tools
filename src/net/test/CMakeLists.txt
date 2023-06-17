message( "BOOSTTEST_LIBRARY: ${BOOSTTEST_LIBRARY}")
# include_directories(../src/include)
# add_executable(server_unittest server_unittest.cpp)
# target_link_libraries(server_unittest net_demo )
#################


add_executable(thread_pool_unittest thread_pool_unittest.cpp)
target_link_libraries(thread_pool_unittest net_demo )

#################

# add_executable(server_echo_unittest server_echo_unittest.cpp)
# target_link_libraries(server_echo_unittest net_demo )
#################

# add_executable(client_unittest client_unittest.cpp)
# target_link_libraries(client_unittest net_demo )

if(BOOSTTEST_LIBRARY)
add_executable(buffer_unittest buffer_unittest.cpp)
target_link_libraries(buffer_unittest net_demo boost_unit_test_framework)
add_test(NAME buffer_unittest COMMAND buffer_unittest)
endif()
#################