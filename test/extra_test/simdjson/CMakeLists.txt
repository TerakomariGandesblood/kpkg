cmake_minimum_required(VERSION 3.18)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_cxx_compiler_flag_no_check("-fsanitize-recover=implicit-conversion")
endif()

find_package(simdjson REQUIRED)

add_executable(simdjson main.cpp)
target_link_libraries(simdjson simdjson::simdjson)

add_custom_command(
  TARGET simdjson
  POST_BUILD
  COMMAND simdjson)
