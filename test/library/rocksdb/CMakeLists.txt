cmake_minimum_required(VERSION 3.19)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(RocksDB REQUIRED)
add_executable(rocksdb main.cpp)
target_link_libraries(rocksdb RocksDB::rocksdb)

if(KPKG_USE_LIBCXX)
  target_link_libraries(rocksdb PRIVATE ${KPKG_LIBCXX} ${KPKG_LIBCXXABI})
endif()

add_custom_command(
  TARGET rocksdb
  POST_BUILD
  COMMAND rocksdb)
