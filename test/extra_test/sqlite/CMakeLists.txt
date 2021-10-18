cmake_minimum_required(VERSION 3.20)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(sqlite3 REQUIRED IMPORTED_TARGET sqlite3)

add_executable(sqlite main.cpp)
target_link_libraries(sqlite PRIVATE PkgConfig::sqlite3 ${CMAKE_DL_LIBS}
                                     ${CMAKE_THREAD_LIBS_INIT})

add_custom_command(
  TARGET sqlite
  POST_BUILD
  COMMAND sqlite)
