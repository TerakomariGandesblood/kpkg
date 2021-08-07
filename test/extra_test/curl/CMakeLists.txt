cmake_minimum_required(VERSION 3.19)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(CURL CONFIG REQUIRED)

add_executable(curl main.cpp)
target_link_libraries(curl PRIVATE CURL::libcurl)

add_custom_command(
  TARGET curl
  POST_BUILD
  COMMAND curl -p www.baidu.com)
