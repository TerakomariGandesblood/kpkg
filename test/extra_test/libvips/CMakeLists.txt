cmake_minimum_required(VERSION 3.18)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/avatar.jpg
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

find_package(klib REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(vips-cpp REQUIRED IMPORTED_TARGET vips-cpp)

add_executable(libvips main.cpp)
target_link_libraries(libvips PRIVATE klib::klib PkgConfig::vips-cpp)

add_custom_command(
  TARGET libvips
  POST_BUILD
  COMMAND libvips)
