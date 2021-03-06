include_directories(${PROJECT_SOURCE_DIR}/src)

find_package (Threads)

add_library(hello SHARED hello.c)

find_program( MEMORYCHECK_COMMAND valgrind)
set( MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --leak-check=full")
set( MEMORYCHECK_SUPPRESSIONS_FILE "${PROJECT_SOURCE_DIR}/valgrind_suppress.txt")

# https://stackoverflow.com/a/14307434/5676460
set(MYLIBS osl ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
file(GLOB TEST_SOURCES test_*.c)
foreach( testsourcefile ${TEST_SOURCES} )

  # https://stackoverflow.com/a/50738319/5676460
  # https://cmake.org/cmake/help/latest/command/get_filename_component.html
  # get_filename_component( testname ${testsourcefile} NAME_WLE )

  # message("test source file: ${testsourcefile}")
  string(REGEX REPLACE ".+/(test.*)\.c" "\\1" testname ${testsourcefile})
  # message("test name: ${testname}")
  
  add_executable( ${testname} ${testsourcefile} )
  target_link_libraries( ${testname} ${MYLIBS} )
  # https://stackoverflow.com/a/61381440/5676460
  if (MSVC)
    # warning level 4 and all warnings as errors
    target_compile_options(${testname} PRIVATE /W4 /WX)
  else()
    # lots of warnings and all warnings as errors
    target_compile_options(${testname} PRIVATE -Wall -Wextra -pedantic -Werror)
  endif()
  add_test( NAME ${testname} COMMAND ${testname} )
endforeach( testsourcefile ${TEST_SOURCES} )
