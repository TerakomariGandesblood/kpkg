cmake_minimum_required(VERSION 3.18)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(nameof REQUIRED)

add_executable(nameof main.cpp)
target_link_libraries(nameof nameof::nameof)

add_custom_command(
  TARGET nameof
  POST_BUILD
  COMMAND nameof)
