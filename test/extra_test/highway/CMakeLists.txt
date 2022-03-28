cmake_minimum_required(VERSION 3.18)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(klib REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(libhwy REQUIRED IMPORTED_TARGET libhwy)

add_executable(highway main.cpp)
target_link_libraries(highway PRIVATE klib::klib PkgConfig::libhwy)

add_custom_command(
  TARGET highway
  POST_BUILD
  COMMAND highway)
