cmake_minimum_required(VERSION 3.18)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/zlib-v1.2.11.tar.gz
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

find_package(fmt REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(openssl REQUIRED IMPORTED_TARGET openssl)

add_executable(openssl main.cpp)
target_link_libraries(openssl PRIVATE PkgConfig::openssl fmt::fmt
                                      ${CMAKE_THREAD_LIBS_INIT})

add_custom_command(
  TARGET openssl
  POST_BUILD
  COMMAND openssl)
