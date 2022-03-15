cmake_minimum_required(VERSION 3.18)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(CLI11 REQUIRED)

add_executable(cli11 main.cpp)
target_link_libraries(cli11 PRIVATE CLI11::CLI11)

add_custom_command(
  TARGET cli11
  POST_BUILD
  COMMAND cli11 -h)
