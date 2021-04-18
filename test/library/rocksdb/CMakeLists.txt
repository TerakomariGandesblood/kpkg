cmake_minimum_required(VERSION 3.19)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

find_package(RocksDB REQUIRED)

add_executable(rocksdb main.cpp)
target_link_libraries(rocksdb PRIVATE RocksDB::rocksdb ${CMAKE_DL_LIBS}
                                      ${CMAKE_THREAD_LIBS_INIT})

if(KPKG_USE_LIBCXX)
  target_link_libraries(rocksdb PRIVATE ${KPKG_LIBCXX} ${KPKG_LIBCXXABI})
endif()

add_custom_command(
  TARGET rocksdb
  POST_BUILD
  COMMAND rocksdb)
