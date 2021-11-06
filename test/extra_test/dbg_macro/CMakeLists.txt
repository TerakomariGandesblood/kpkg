cmake_minimum_required(VERSION 3.20)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

add_executable(dbg-macro main.cpp)

add_custom_command(
  TARGET dbg-macro
  POST_BUILD
  COMMAND dbg-macro)
