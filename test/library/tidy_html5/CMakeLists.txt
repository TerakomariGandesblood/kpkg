cmake_minimum_required(VERSION 3.19)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

add_executable(tidy-html5 main.cpp)
target_link_libraries(tidy-html5 PRIVATE tidys)

add_custom_command(
  TARGET tidy-html5
  POST_BUILD
  COMMAND tidy-html5)
