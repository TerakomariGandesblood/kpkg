cmake_minimum_required(VERSION 3.19)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

find_package(magic_enum REQUIRED)
add_executable(magic-enum main.cpp)

if(KPKG_USE_LIBCXX)
  target_link_libraries(magic-enum PRIVATE ${KPKG_LIBCXX} ${KPKG_LIBCXXABI})
endif()

add_custom_command(
  TARGET magic-enum
  POST_BUILD
  COMMAND magic-enum)
