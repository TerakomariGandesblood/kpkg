cmake_minimum_required(VERSION 3.18)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(PkgConfig REQUIRED)
pkg_check_modules(sqlcipher REQUIRED IMPORTED_TARGET sqlcipher)
add_definitions(-DSQLITE_HAS_CODEC)

add_executable(sqlcipher main.cpp)
target_link_libraries(sqlcipher PRIVATE PkgConfig::sqlcipher)

add_custom_command(
  TARGET sqlcipher
  POST_BUILD
  COMMAND sqlcipher)
