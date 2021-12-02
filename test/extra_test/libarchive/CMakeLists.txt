cmake_minimum_required(VERSION 3.22)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(libarchive REQUIRED IMPORTED_TARGET libarchive)
pkg_check_modules(zlib REQUIRED IMPORTED_TARGET zlib)
pkg_check_modules(openssl REQUIRED IMPORTED_TARGET openssl)
pkg_check_modules(libzstd REQUIRED IMPORTED_TARGET libzstd)
add_definitions(-DOPENSSL_NO_DEPRECATED)

add_executable(libarchive main.cpp)
target_link_libraries(
  libarchive PRIVATE PkgConfig::libarchive PkgConfig::libzstd PkgConfig::zlib
                     PkgConfig::openssl ${CMAKE_THREAD_LIBS_INIT})

add_custom_command(
  TARGET libarchive
  POST_BUILD
  COMMAND libarchive libarchive libarchive.tar.gz
  COMMAND libarchive libarchive.tar.gz)
