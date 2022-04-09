cmake_minimum_required(VERSION 3.18)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/SourceHanSansSC-Bold.ttf
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

find_package(klib REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(libwoff2dec REQUIRED IMPORTED_TARGET libwoff2dec)
pkg_check_modules(libwoff2enc REQUIRED IMPORTED_TARGET libwoff2enc)

add_executable(woff2 main.cpp)
target_link_libraries(woff2 PRIVATE klib::klib PkgConfig::libwoff2dec
                                    PkgConfig::libwoff2enc)

add_custom_command(
  TARGET woff2
  POST_BUILD
  COMMAND woff2)
