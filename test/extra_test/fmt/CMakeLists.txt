cmake_minimum_required(VERSION 3.22)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(fmt REQUIRED)

add_executable(fmt main.cpp)
target_link_libraries(fmt PRIVATE fmt::fmt)

add_custom_command(
  TARGET fmt
  POST_BUILD
  COMMAND fmt)
