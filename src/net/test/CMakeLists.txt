message( "BOOSTTEST_LIBRARY: ${BOOSTTEST_LIBRARY}")
add_executable(net_server server.cpp)
target_link_libraries(net_server net_demo )
#################


add_executable(thread_pool_unittest thread_pool_unittest.cpp)
target_link_libraries(thread_pool_unittest net_demo )

#################
if(BOOSTTEST_LIBRARY)
add_executable(buffer_unittest buffer_unittest.cpp)
target_link_libraries(buffer_unittest net_demo boost_unit_test_framework)
add_test(NAME buffer_unittest COMMAND buffer_unittest)
endif()
#################