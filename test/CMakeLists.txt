
include_directories(${PROJECT_SOURCE_DIR}/src)

find_package (Threads)

add_library(hello SHARED hello.c)

# test_osl
add_executable(test_osl test_osl.c)
target_link_libraries(test_osl osl ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_osl COMMAND test_osl)

# test_thread
add_executable(test_thread test_thread.c)
target_link_libraries(test_thread osl ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_thread COMMAND test_thread)

# test_list
add_executable(test_list test_list.c)
target_link_libraries(test_list osl ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_list COMMAND test_list)

# test_date
add_executable(test_date test_date.c)
target_link_libraries(test_date osl ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_date COMMAND test_date)

# test_string
add_executable(test_string test_string.c)
target_link_libraries(test_string osl ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_string COMMAND test_string)

# test_string_buffer
add_executable(test_string_buffer test_string_buffer.c)
target_link_libraries(test_string_buffer osl ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_string_buffer COMMAND test_string_buffer)

# test_stream
add_executable(test_stream test_stream.c)
target_link_libraries(test_stream osl ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_stream COMMAND test_stream)

# test_string_stream
add_executable(test_string_stream test_string_stream.c)
target_link_libraries(test_string_stream osl ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_string_stream COMMAND test_string_stream)

# test_pathname
add_executable(test_pathname test_pathname.c)
target_link_libraries(test_pathname osl ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_pathname COMMAND test_pathname)

# test_mem
add_executable(test_mem test_mem.c)
target_link_libraries(test_mem osl ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_mem COMMAND test_mem)

# test_network
add_executable(test_network test_network.c)
target_link_libraries(test_network osl ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_network COMMAND test_network)

# test_echo_server
add_executable(test_echo_server test_echo_server.c)
target_link_libraries(test_echo_server osl ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_echo_server COMMAND test_echo_server)

# test_datagram_socket
add_executable(test_datagram_socket test_datagram_socket.c)
target_link_libraries(test_datagram_socket osl ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_datagram_socket COMMAND test_datagram_socket)

# test_multicast_socket
add_executable(test_multicast_socket test_multicast_socket.c)
target_link_libraries(test_multicast_socket osl ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_multicast_socket COMMAND test_multicast_socket)

# test_library
add_executable(test_library test_library.c)
target_link_libraries(test_library osl ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_library COMMAND test_library)

# test_log
add_executable(test_log test_log.c)
target_link_libraries(test_log osl ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_log COMMAND test_log)

# test_process
add_executable(test_process test_process.c)
target_link_libraries(test_process osl ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_process COMMAND test_process)

# test_url
add_executable(test_url test_url.c)
target_link_libraries(test_url osl ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_url COMMAND test_url)

# test_arguments
add_executable(test_arguments test_arguments.c)
target_link_libraries(test_arguments osl ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_arguments COMMAND test_arguments)

# test_looper
add_executable(test_looper test_looper.c)
target_link_libraries(test_looper osl ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_looper COMMAND test_looper)

# test_environ
add_executable(test_environ test_environ.c)
target_link_libraries(test_environ osl ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_environ COMMAND test_environ)

# test_heap
add_executable(test_heap test_heap.c)
target_link_libraries(test_heap osl ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_heap COMMAND test_heap)

# test_properties
add_executable(test_properties test_properties.c)
target_link_libraries(test_properties osl ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_properties COMMAND test_properties)

# test_cache
add_executable(test_cache test_cache.c)
target_link_libraries(test_cache osl ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_cache COMMAND test_cache)


