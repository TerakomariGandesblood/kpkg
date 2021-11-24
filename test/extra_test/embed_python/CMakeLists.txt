cmake_minimum_required(VERSION 3.20)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

# FIXME
if(CMAKE_COMPILER_IS_GNUCXX)
  add_cxx_compiler_flag_no_check("-Wno-error=maybe-uninitialized")
endif()
if(KPKG_SANITIZER)
  add_definitions(-DKPKG_SANITIZER)
endif()

# https://cmake.org/cmake/help/latest/module/FindPython.html
set(Python_USE_STATIC_LIBS TRUE)
find_package(Python COMPONENTS Development.Embed)
add_definitions(-DPY_SSIZE_T_CLEAN)

add_executable(embed_python main.cpp)
target_link_libraries(embed_python PRIVATE Python::Python)

add_custom_command(
  TARGET embed_python
  POST_BUILD
  COMMAND embed_python)
