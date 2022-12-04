cmake_minimum_required(VERSION 3.5)

project(osl LANGUAGES C VERSION "1.0.0")

include(CTest)

enable_testing()
add_subdirectory(test)

file(GLOB OSL_C_SOURCES src/*.c)

add_library(osl SHARED ${OSL_C_SOURCES})

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

file(GLOB OSL_C_HEADERS src/*.h)

include(GNUInstallDirs)
set(DIR_HEADERS)
list(APPEND DIR_HEADERS ${OSL_C_HEADERS})
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
