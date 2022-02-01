cmake_minimum_required(VERSION 3.22)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(PkgConfig REQUIRED)
pkg_check_modules(libcurl REQUIRED IMPORTED_TARGET libcurl)
pkg_check_modules(libbrotlicommon REQUIRED IMPORTED_TARGET libbrotlicommon)
add_definitions(-DCURL_NO_OLDIES)

add_executable(curl main.cpp)
target_link_libraries(
  curl PRIVATE PkgConfig::libcurl PkgConfig::libbrotlicommon
               /usr/local/lib/libunistring.a)

add_custom_command(
  TARGET curl
  POST_BUILD
  COMMAND curl -p https://www.baidu.com/)
