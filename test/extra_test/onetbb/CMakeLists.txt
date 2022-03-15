cmake_minimum_required(VERSION 3.18)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_cxx_compiler_flag_no_check("-fsanitize-recover=implicit-conversion")
else()
  add_cxx_compiler_flag_no_check("-fsanitize-recover=alignment")
endif()

find_package(TBB REQUIRED)
if(NOT KPKG_SANITIZER)
  message(STATUS "Use mimalloc")
  set(MIMALLOC_OBJECT "/usr/local/lib/mimalloc.o")
endif()

add_executable(onetbb ${MIMALLOC_OBJECT} main.cpp)
target_link_libraries(onetbb PRIVATE TBB::tbb)

add_custom_command(
  TARGET onetbb
  POST_BUILD
  COMMAND onetbb)
