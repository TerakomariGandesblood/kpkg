cmake_minimum_required(VERSION 3.22)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(PkgConfig REQUIRED)
pkg_check_modules(opencc REQUIRED IMPORTED_TARGET opencc)

add_executable(opencc main.cpp)
target_link_libraries(opencc PRIVATE PkgConfig::opencc)

add_custom_command(
  TARGET opencc
  POST_BUILD
  COMMAND opencc)
