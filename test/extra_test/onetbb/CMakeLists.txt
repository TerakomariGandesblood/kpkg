cmake_minimum_required(VERSION 3.20)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)
# FIXME
add_cxx_compiler_flag_no_check("-fsanitize-recover=implicit-conversion")

find_package(TBB REQUIRED)

add_executable(onetbb main.cpp)
target_link_libraries(onetbb PRIVATE TBB::tbb)

add_custom_command(
  TARGET onetbb
  POST_BUILD
  COMMAND onetbb)
