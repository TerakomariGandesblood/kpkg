cmake_minimum_required(VERSION 3.19)

project(test_library_install LANGUAGES CXX)

include(../common.cmake)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data.txt
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# https://cmake.org/cmake/help/latest/module/FindICU.html
find_package(ICU REQUIRED COMPONENTS data i18n io uc)

add_executable(unicode-string unicode_string.cpp)
target_link_libraries(unicode-string PRIVATE ${ICU_LIBRARIES})

if(KPKG_USE_LIBCXX)
  target_link_libraries(unicode-string PRIVATE ${KPKG_LIBCXX} ${KPKG_LIBCXXABI})
endif()

add_custom_command(
  TARGET unicode-string
  POST_BUILD
  COMMAND unicode-string)

add_executable(transforms transforms.cpp)
target_link_libraries(transforms PRIVATE ${ICU_LIBRARIES})

if(KPKG_USE_LIBCXX)
  target_link_libraries(transforms PRIVATE ${KPKG_LIBCXX} ${KPKG_LIBCXXABI})
endif()

add_custom_command(
  TARGET transforms
  POST_BUILD
  COMMAND transforms)
