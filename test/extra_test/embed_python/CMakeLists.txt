cmake_minimum_required(VERSION 3.20)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

# https://cmake.org/cmake/help/latest/module/FindPython.html
find_package(Python COMPONENTS Development.Embed)
add_definitions(-DPY_SSIZE_T_CLEAN)

add_executable(embed_python main.cpp)
target_link_libraries(embed_python PRIVATE Python::Python)

add_custom_command(
  TARGET embed_python
  POST_BUILD
  COMMAND embed_python)
