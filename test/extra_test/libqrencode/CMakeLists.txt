cmake_minimum_required(VERSION 3.18)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(PkgConfig REQUIRED)
pkg_check_modules(libqrencode REQUIRED IMPORTED_TARGET libqrencode)

add_executable(libqrencode main.cpp)
target_link_libraries(libqrencode PRIVATE PkgConfig::libqrencode)

add_custom_command(
  TARGET libqrencode
  POST_BUILD
  COMMAND libqrencode)
