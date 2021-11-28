cmake_minimum_required(VERSION 3.22)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(zstd REQUIRED)

add_executable(zstd main.cpp)
target_link_libraries(zstd zstd::libzstd_static)

add_custom_command(
  TARGET zstd
  POST_BUILD
  COMMAND zstd zstd
  COMMAND zstd zstd.zst)
