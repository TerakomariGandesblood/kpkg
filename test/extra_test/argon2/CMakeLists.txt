cmake_minimum_required(VERSION 3.20)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(PkgConfig REQUIRED)
pkg_check_modules(libargon2 REQUIRED IMPORTED_TARGET libargon2)

add_executable(argon2 main.cpp)
target_link_libraries(argon2 PRIVATE PkgConfig::libargon2)

add_custom_command(
  TARGET argon2
  POST_BUILD
  COMMAND argon2)
