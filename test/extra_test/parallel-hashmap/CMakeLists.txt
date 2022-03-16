cmake_minimum_required(VERSION 3.18)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

add_executable(parallel-hashmap main.cpp)

add_custom_command(
  TARGET parallel-hashmap
  POST_BUILD
  COMMAND parallel-hashmap)
