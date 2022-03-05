cmake_minimum_required(VERSION 3.22)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/SourceHanSansSC-Bold.ttf
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

find_package(klib REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(libwoff2dec REQUIRED IMPORTED_TARGET libwoff2dec)
pkg_check_modules(libwoff2enc REQUIRED IMPORTED_TARGET libwoff2enc)
pkg_check_modules(libwoff2common REQUIRED IMPORTED_TARGET libwoff2common)

pkg_check_modules(libbrotlidec REQUIRED IMPORTED_TARGET libbrotlidec)
pkg_check_modules(libbrotlienc REQUIRED IMPORTED_TARGET libbrotlienc)
pkg_check_modules(libbrotlicommon REQUIRED IMPORTED_TARGET libbrotlicommon)

add_executable(woff2 main.cpp)
target_link_libraries(
  woff2
  PRIVATE klib::klib
          PkgConfig::libwoff2dec
          PkgConfig::libwoff2enc
          PkgConfig::libwoff2common
          PkgConfig::libbrotlidec
          PkgConfig::libbrotlienc
          PkgConfig::libbrotlicommon)

add_custom_command(
  TARGET woff2
  POST_BUILD
  COMMAND woff2)
