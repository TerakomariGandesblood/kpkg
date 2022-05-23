cmake_minimum_required(VERSION 3.22)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(re2 REQUIRED)

add_executable(re2 main.cpp)
target_link_libraries(re2 re2::re2)

add_custom_command(
  TARGET re2
  POST_BUILD
  COMMAND re2)
