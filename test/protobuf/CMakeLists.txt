cmake_minimum_required(VERSION 3.17)

project(test_library_install LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

# https://cmake.org/cmake/help/latest/module/FindProtobuf.html
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

find_package(Protobuf REQUIRED)

add_executable(protobuf main.cpp addressbook.proto)
target_link_libraries(protobuf ${Protobuf_LIBRARIES})
protobuf_generate(TARGET protobuf)
# TODO add_custom_command( TARGET protobuf POST_BUILD COMMAND protobuf)
