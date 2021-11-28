cmake_minimum_required(VERSION 3.22)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

if(NOT (${CMAKE_BUILD_TYPE} STREQUAL "Debug"))
  add_definitions(-DDBG_MACRO_DISABLE -DDBG_MACRO_NO_WARNING)
endif()

add_executable(dbg-macro main.cpp)

add_custom_command(
  TARGET dbg-macro
  POST_BUILD
  COMMAND dbg-macro)
