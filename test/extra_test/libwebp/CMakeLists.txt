cmake_minimum_required(VERSION 3.22)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(WebP REQUIRED)

add_executable(libwebp main.cpp)
target_link_libraries(libwebp WebP::webp)

add_custom_command(
  TARGET libwebp
  POST_BUILD
  COMMAND libwebp)
