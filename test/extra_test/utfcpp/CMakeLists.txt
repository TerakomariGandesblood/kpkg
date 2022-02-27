cmake_minimum_required(VERSION 3.22)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)
# FIXME
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_cxx_compiler_flag_no_check("-fsanitize-recover=implicit-conversion")
endif()

find_package(utf8cpp REQUIRED)

add_executable(utfcpp main.cpp)
target_link_libraries(utfcpp PRIVATE utf8cpp)

add_custom_command(
  TARGET utfcpp
  POST_BUILD
  COMMAND utfcpp)
