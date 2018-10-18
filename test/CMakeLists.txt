find_package (Threads)
add_executable(Test test.c)
target_link_libraries(Test osl)
target_link_libraries (Test ${CMAKE_THREAD_LIBS_INIT})
