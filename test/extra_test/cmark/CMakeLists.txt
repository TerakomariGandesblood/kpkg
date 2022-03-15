cmake_minimum_required(VERSION 3.18)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(cmark REQUIRED)

add_executable(cmark main.cpp)
target_link_libraries(cmark PRIVATE cmark::cmark_static)

add_custom_command(
  TARGET cmark
  POST_BUILD
  COMMAND cmark)
