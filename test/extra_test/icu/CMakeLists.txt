cmake_minimum_required(VERSION 3.22)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)
# FIXME
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_cxx_compiler_flag_no_check("-fsanitize-recover=implicit-conversion")
endif()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data.txt
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

find_package(PkgConfig REQUIRED)
pkg_check_modules(icu-i18n REQUIRED IMPORTED_TARGET icu-i18n)
pkg_check_modules(icu-io REQUIRED IMPORTED_TARGET icu-io)
add_definitions(-DU_HIDE_INTERNAL_API -DU_HIDE_OBSOLETE_API
                -DU_HIDE_DEPRECATED_API)

add_executable(unicode-string unicode_string.cpp)
target_link_libraries(unicode-string PRIVATE PkgConfig::icu-i18n)

add_custom_command(
  TARGET unicode-string
  POST_BUILD
  COMMAND unicode-string)

add_executable(transforms transforms.cpp)
target_link_libraries(transforms PRIVATE PkgConfig::icu-i18n PkgConfig::icu-io)

add_custom_command(
  TARGET transforms
  POST_BUILD
  COMMAND transforms)
