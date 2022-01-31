cmake_minimum_required(VERSION 3.22)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(zlib REQUIRED IMPORTED_TARGET zlib)
pkg_check_modules(openssl REQUIRED IMPORTED_TARGET openssl)
pkg_check_modules(libaria2 REQUIRED IMPORTED_TARGET libaria2)
# FIXME pkg_check_modules(libcares REQUIRED IMPORTED_TARGET libcares)

add_definitions(-DOPENSSL_NO_DEPRECATED)

add_executable(aria2 main.cpp)
target_link_libraries(
  aria2
  PRIVATE PkgConfig::libaria2 PkgConfig::zlib PkgConfig::openssl
          # PkgConfig::libcares
          ${CMAKE_THREAD_LIBS_INIT})

add_custom_command(
  TARGET aria2
  POST_BUILD
  COMMAND aria2 https://dldir1.qq.com/qqfile/qq/PCQQ9.5.6/QQ9.5.6.28129.exe)
