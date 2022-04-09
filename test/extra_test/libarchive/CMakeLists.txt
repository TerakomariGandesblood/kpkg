cmake_minimum_required(VERSION 3.18)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(PkgConfig REQUIRED)
pkg_check_modules(libarchive REQUIRED IMPORTED_TARGET libarchive)

add_executable(libarchive main.cpp)
target_link_libraries(libarchive PRIVATE PkgConfig::libarchive)

add_custom_command(
  TARGET libarchive
  POST_BUILD
  COMMAND libarchive libarchive libarchive.tar.gz
  COMMAND libarchive libarchive.tar.gz)
