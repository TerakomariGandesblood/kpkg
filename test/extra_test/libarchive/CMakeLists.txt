cmake_minimum_required(VERSION 3.22)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(PkgConfig REQUIRED)
pkg_check_modules(libarchive REQUIRED IMPORTED_TARGET libarchive)
pkg_check_modules(zlib REQUIRED IMPORTED_TARGET zlib)

add_executable(libarchive main.cpp)
target_link_libraries(libarchive PRIVATE PkgConfig::libarchive PkgConfig::zlib)

add_custom_command(
  TARGET libarchive
  POST_BUILD
  COMMAND libarchive libarchive libarchive.tar.gz
  COMMAND libarchive libarchive.tar.gz)
