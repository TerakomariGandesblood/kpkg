cmake_minimum_required(VERSION 3.22)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_cxx_compiler_flag_no_check("-fsanitize-recover=implicit-conversion")
endif()

find_package(indicators REQUIRED)

add_executable(indicators main.cpp)
target_link_libraries(indicators PRIVATE indicators::indicators)

add_custom_command(
  TARGET indicators
  POST_BUILD
  COMMAND indicators)
