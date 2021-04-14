cmake_minimum_required(VERSION 3.19)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

# https://cmake.org/cmake/help/latest/module/FindProtobuf.html
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

find_package(Protobuf REQUIRED)

add_executable(protobuf main.cpp addressbook.proto)
target_link_libraries(protobuf PRIVATE ${Protobuf_LIBRARIES})

if(KPKG_USE_LIBCXX)
  target_link_libraries(protobuf PRIVATE ${KPKG_LIBCXX} ${KPKG_LIBCXXABI})
endif()

protobuf_generate(TARGET protobuf)
# TODO add_custom_command( TARGET protobuf POST_BUILD COMMAND protobuf)
