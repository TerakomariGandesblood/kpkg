cmake_minimum_required(VERSION 3.18)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

# https://microsoft.github.io/mimalloc/overrides.html
if(NOT KPKG_SANITIZER)
  message(STATUS "Use mimalloc")
  set(MIMALLOC_OBJECT "/usr/local/lib/mimalloc.o")
endif()

add_executable(mimalloc ${MIMALLOC_OBJECT} main.cpp)

add_custom_command(
  TARGET mimalloc
  POST_BUILD
  COMMAND mimalloc)
