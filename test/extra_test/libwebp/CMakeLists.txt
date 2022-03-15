cmake_minimum_required(VERSION 3.18)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(PkgConfig REQUIRED)
pkg_check_modules(libwebp REQUIRED IMPORTED_TARGET libwebp)

add_executable(libwebp main.cpp)
target_link_libraries(libwebp PkgConfig::libwebp)

add_custom_command(
  TARGET libwebp
  POST_BUILD
  COMMAND libwebp)
