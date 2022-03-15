cmake_minimum_required(VERSION 3.18)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(Microsoft.GSL REQUIRED)

add_executable(gsl main.cpp)
target_link_libraries(gsl PRIVATE Microsoft.GSL::GSL)

add_custom_command(
  TARGET gsl
  POST_BUILD
  COMMAND gsl)
