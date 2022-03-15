cmake_minimum_required(VERSION 3.18)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(scope_guard REQUIRED)

add_executable(scope_guard main.cpp)
target_link_libraries(scope_guard scope_guard::scope_guard)

add_custom_command(
  TARGET scope_guard
  POST_BUILD
  COMMAND scope_guard)
