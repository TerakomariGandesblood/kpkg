cmake_minimum_required(VERSION 3.17)

project(test_library_install LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data.txt
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# https://cmake.org/cmake/help/latest/module/FindICU.html
find_package(ICU REQUIRED COMPONENTS data i18n io uc)

add_executable(conv conv.cpp)
target_link_libraries(conv ${ICU_LIBRARIES})
add_custom_command(
  TARGET conv
  POST_BUILD
  COMMAND conv)

add_executable(unicode-string unicode_string.cpp)
target_link_libraries(unicode-string ${ICU_LIBRARIES})
add_custom_command(
  TARGET unicode-string
  POST_BUILD
  COMMAND unicode-string)

add_executable(transforms transforms.cpp)
target_link_libraries(transforms ${ICU_LIBRARIES})
add_custom_command(
  TARGET transforms
  POST_BUILD
  COMMAND transforms)
