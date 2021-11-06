cmake_minimum_required(VERSION 3.20)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(gsl-lite REQUIRED)

add_executable(gsl-lite main.cpp)
target_link_libraries(gsl-lite PRIVATE gsl::gsl-lite-v1)

add_custom_command(
  TARGET gsl-lite
  POST_BUILD
  COMMAND gsl-lite)
