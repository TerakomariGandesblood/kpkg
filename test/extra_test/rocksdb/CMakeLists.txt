cmake_minimum_required(VERSION 3.20)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

find_package(RocksDB REQUIRED)

add_executable(rocksdb main.cpp)
target_link_libraries(rocksdb PRIVATE RocksDB::rocksdb ${CMAKE_DL_LIBS}
                                      ${CMAKE_THREAD_LIBS_INIT})

add_custom_command(
  TARGET rocksdb
  POST_BUILD
  COMMAND rocksdb)
