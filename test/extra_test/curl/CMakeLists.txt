cmake_minimum_required(VERSION 3.18)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(PkgConfig REQUIRED)
pkg_check_modules(libcurl REQUIRED IMPORTED_TARGET libcurl)
add_definitions(-DCURL_NO_OLDIES)

add_executable(curl main.cpp)
target_link_libraries(curl PRIVATE PkgConfig::libcurl)

add_custom_command(
  TARGET curl
  POST_BUILD
  COMMAND curl -p https://www.baidu.com/)
