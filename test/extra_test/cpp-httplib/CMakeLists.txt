cmake_minimum_required(VERSION 3.22)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(httplib REQUIRED)

add_executable(cpp-httplib main.cpp)
target_link_libraries(cpp-httplib PRIVATE httplib::httplib)

add_custom_command(
  TARGET cpp-httplib
  POST_BUILD
  COMMAND cpp-httplib)
