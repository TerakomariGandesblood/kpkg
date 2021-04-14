cmake_minimum_required(VERSION 3.19)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(fmt REQUIRED)
add_executable(fmt main.cpp)
target_link_libraries(fmt PRIVATE fmt::fmt)

if(KPKG_USE_LIBCXX)
  target_link_libraries(fmt PRIVATE ${KPKG_LIBCXX} ${KPKG_LIBCXXABI})
endif()

add_custom_command(
  TARGET fmt
  POST_BUILD
  COMMAND fmt)
