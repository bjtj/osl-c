cmake_minimum_required(VERSION 3.5)

project(osl LANGUAGES C VERSION "1.0.0")

include(CTest)

enable_testing()
add_subdirectory(test)

add_library(osl SHARED
  src/argparse.c
  src/argparse.h
  src/cache.c
  src/cache.h
  src/data.h
  src/date.c
  src/date.h
  src/environ.c
  src/environ.h
  src/event.c
  src/event.h
  src/file.c
  src/file.h
  src/hash.c
  src/hash.h
  src/heap.c
  src/heap.h
  src/inet_address.c
  src/inet_address.h
  src/library.c
  src/library.h
  src/list.c
  src/list.h
  src/log.c
  src/log.h
  src/looper.c
  src/looper.h
  src/mutex.c
  src/mutex.h
  src/network.c
  src/network.h
  src/osl.c
  src/osl.h
  src/pathname.c
  src/pathname.h
  src/platform.h
  src/process.c
  src/process.h
  src/properties.c
  src/properties.h
  src/selector.c
  src/selector.h
  src/sem.c
  src/sem.h
  src/socket.c
  src/socket.h
  src/str.c
  src/str.h
  src/stream.c
  src/stream.h
  src/string_buffer.c
  src/string_buffer.h
  src/string_stream.c
  src/string_stream.h
  src/thread.c
  src/thread.h
  src/url.c
  src/url.h)

target_link_libraries(osl ${CMAKE_DL_LIBS})

# https://stackoverflow.com/a/61381440/5676460
if (MSVC)
    # warning level 4 and all warnings as errors
    target_compile_options(osl PRIVATE /W4 /WX)
else()
    # lots of warnings and all warnings as errors
    target_compile_options(osl PRIVATE -Wall -Wextra -pedantic -Werror -Wno-missing-field-initializers)
endif()


# properties
set_target_properties(osl PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(osl PROPERTIES SOVERSION 1)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(osl PRIVATE .)

include(GNUInstallDirs)
set(DIR_HEADERS)
list(APPEND DIR_HEADERS
  src/argparse.h
  src/cache.h
  src/data.h
  src/date.h
  src/environ.h
  src/event.h
  src/file.h
  src/hash.h
  src/heap.h
  src/inet_address.h
  src/library.h
  src/list.h
  src/log.h
  src/looper.h
  src/mutex.h
  src/network.h
  src/osl.h
  src/pathname.h
  src/platform.h
  src/process.h
  src/properties.h
  src/selector.h
  src/sem.h
  src/socket.h
  src/str.h
  src/stream.h
  src/string_buffer.h
  src/string_stream.h
  src/thread.h
  src/url.h)
set_target_properties(osl PROPERTIES PUBLIC_HEADER "${DIR_HEADERS}")
install(TARGETS osl
  EXPORT osltargets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/osl)

if (MSVC)
add_custom_command(TARGET osl POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:osl> $<TARGET_FILE_DIR:hello>) 
endif()

# pkgconfig
set(VERSION "${PROJECT_VERSION}")
set(prefix "${CMAKE_INSTALL_PREFIX}")
set(exec_prefix "\${prefix}")
set(libdir "\${exec_prefix}/${CMAKE_INSTALL_LIBDIR}")
set(includedir "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}")
configure_file(osl.pc.in osl.pc @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/osl.pc" DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig")
