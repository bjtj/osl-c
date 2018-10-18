cmake_minimum_required(VERSION 3.5)

project(osl LANGUAGES C VERSION "1.0.0")

enable_testing()

add_library(osl SHARED
  src/date.c
  src/date.h
  src/file.c
  src/file.h
  src/list.c
  src/list.h
  src/mutex.c
  src/mutex.h
  src/osl.c
  src/osl.h
  src/string_buffer.c
  src/string_buffer.h
  src/platform.h
  src/semaphore.c
  src/semaphore.h
  src/thread.c
  src/thread.h)

# properties
set_target_properties(osl PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(osl PROPERTIES SOVERSION 1)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(osl PRIVATE .)

include(GNUInstallDirs)
set(DIR_HEADERS)
list(APPEND DIR_HEADERS
  src/date.h
  src/file.h
  src/list.h
  src/mutex.h
  src/osl.h
  src/string_buffer.h
  src/platform.h
  src/semaphore.h
  src/thread.h
  )
set_target_properties(osl PROPERTIES PUBLIC_HEADER "${DIR_HEADERS}")
install(TARGETS osl
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/osl)

# pkgconfig
set(VERSION "${PROJECT_VERSION}")
set(prefix "${CMAKE_INSTALL_PREFIX}")
set(exec_prefix "\${prefix}")
set(libdir "\${exec_prefix}/${CMAKE_INSTALL_LIBDIR}")
set(includedir "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}")
configure_file(osl.pc.in osl.pc @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/osl.pc" DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig")
