cmake_minimum_required(VERSION 3.20)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

# https://cmake.org/cmake/help/latest/module/FindSQLite3.html
find_package(SQLite3 REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

add_executable(sqlite main.cpp)
target_link_libraries(sqlite PRIVATE ${SQLite3_LIBRARIES} ${CMAKE_DL_LIBS}
                                     ${CMAKE_THREAD_LIBS_INIT})

add_custom_command(
  TARGET sqlite
  POST_BUILD
  COMMAND sqlite)
