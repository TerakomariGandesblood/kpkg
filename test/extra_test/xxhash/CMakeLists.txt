cmake_minimum_required(VERSION 3.18)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(xxHash REQUIRED)

add_executable(xxhash main.cpp)
target_link_libraries(xxhash xxHash::xxhash)

add_custom_command(
  TARGET xxhash
  POST_BUILD
  COMMAND xxhash)
