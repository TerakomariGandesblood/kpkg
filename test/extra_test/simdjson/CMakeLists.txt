cmake_minimum_required(VERSION 3.20)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(simdjson REQUIRED)

add_executable(simdjson main.cpp)
target_link_libraries(simdjson simdjson::simdjson)

add_custom_command(
  TARGET simdjson
  POST_BUILD
  COMMAND simdjson)
