cmake_minimum_required(VERSION 3.18)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/avatar.jpg
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

find_package(klib REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(vips REQUIRED IMPORTED_TARGET vips)
pkg_check_modules(vips-cpp REQUIRED IMPORTED_TARGET vips-cpp)
pkg_check_modules(gio-2.0 REQUIRED IMPORTED_TARGET gio-2.0)
pkg_check_modules(expat REQUIRED IMPORTED_TARGET expat)
pkg_check_modules(libffi REQUIRED IMPORTED_TARGET libffi)
pkg_check_modules(libspng REQUIRED IMPORTED_TARGET libspng)
pkg_check_modules(libjpeg REQUIRED IMPORTED_TARGET libjpeg)
pkg_check_modules(libwebp REQUIRED IMPORTED_TARGET libwebp)
pkg_check_modules(libwebpmux REQUIRED IMPORTED_TARGET libwebpmux)
pkg_check_modules(libwebpdemux REQUIRED IMPORTED_TARGET libwebpdemux)
pkg_check_modules(blkid REQUIRED IMPORTED_TARGET blkid)

add_executable(libvips main.cpp)
target_link_libraries(
  libvips
  PRIVATE klib::klib
          PkgConfig::vips
          PkgConfig::vips-cpp
          PkgConfig::gio-2.0
          PkgConfig::expat
          PkgConfig::libffi
          PkgConfig::libspng
          PkgConfig::libjpeg
          PkgConfig::libwebp
          PkgConfig::libwebpmux
          PkgConfig::libwebpdemux
          PkgConfig::blkid)

add_custom_command(
  TARGET libvips
  POST_BUILD
  COMMAND libvips)
