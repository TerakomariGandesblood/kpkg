cmake_minimum_required(VERSION 3.18)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(PkgConfig REQUIRED)
pkg_check_modules(libzstd REQUIRED IMPORTED_TARGET libzstd)

add_executable(zstd main.cpp)
target_link_libraries(zstd PkgConfig::libzstd)

add_custom_command(
  TARGET zstd
  POST_BUILD
  COMMAND zstd zstd
  COMMAND zstd zstd.zst)
