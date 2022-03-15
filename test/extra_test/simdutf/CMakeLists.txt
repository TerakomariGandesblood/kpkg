cmake_minimum_required(VERSION 3.18)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(simdutf REQUIRED)

add_executable(simdutf main.cpp)
target_link_libraries(simdutf simdutf::simdutf)

add_custom_command(
  TARGET simdutf
  POST_BUILD
  COMMAND simdutf)
