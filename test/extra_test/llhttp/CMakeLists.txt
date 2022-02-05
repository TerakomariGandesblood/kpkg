cmake_minimum_required(VERSION 3.22)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(llhttp REQUIRED)

add_executable(llhttp main.cpp)
target_link_libraries(llhttp PRIVATE llhttp::llhttp)

add_custom_command(
  TARGET llhttp
  POST_BUILD
  COMMAND llhttp)
