cmake_minimum_required(VERSION 3.20)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)
# FIXME
if(KPKG_SANITIZER)
  add_definitions(-DKPKG_SANITIZER)
endif()

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/SourceHanSansSC-Bold.otf"
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# https://cmake.org/cmake/help/latest/module/FindPython.html
find_package(Python COMPONENTS Development.Embed)
add_definitions(-DPY_SSIZE_T_CLEAN)

add_executable(embed_python main.cpp)
target_link_libraries(embed_python PRIVATE Python::Python)

add_custom_command(
  TARGET embed_python
  POST_BUILD
  COMMAND embed_python)
