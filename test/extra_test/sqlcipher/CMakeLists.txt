cmake_minimum_required(VERSION 3.20)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(openssl REQUIRED IMPORTED_TARGET openssl)
add_definitions(-DOPENSSL_NO_DEPRECATED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(sqlcipher REQUIRED IMPORTED_TARGET sqlcipher)

add_definitions(-DSQLITE_HAS_CODEC)

add_executable(sqlcipher main.cpp)
target_link_libraries(sqlcipher PRIVATE PkgConfig::sqlcipher PkgConfig::openssl
                                        ${CMAKE_THREAD_LIBS_INIT})

add_custom_command(
  TARGET sqlcipher
  POST_BUILD
  COMMAND sqlcipher)
