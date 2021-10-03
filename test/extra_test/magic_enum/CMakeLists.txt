cmake_minimum_required(VERSION 3.20)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(magic_enum REQUIRED)

add_executable(magic-enum main.cpp)

add_custom_command(
  TARGET magic-enum
  POST_BUILD
  COMMAND magic-enum)
