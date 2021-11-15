# ---------------------------------------------------------------------------------------
# CMake version
# ---------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.20)
message(STATUS "CMake version: ${CMAKE_VERSION}")

# ---------------------------------------------------------------------------------------
# Start kpkg project
# ---------------------------------------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(Version)
kpkg_extract_version()

project(
  kpkg
  VERSION ${KPKG_VERSION}
  LANGUAGES ASM CXX)

message(STATUS "${PROJECT_NAME} version: ${KPKG_VERSION}")

# ---------------------------------------------------------------------------------------
# Set default build to release
# ---------------------------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      "Release"
      CACHE STRING "Choose Debug, Release, RelWithDebInfo or MinSizeRel" FORCE)
endif()
message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})

# ---------------------------------------------------------------------------------------
# Set variables
# ---------------------------------------------------------------------------------------
set(LIBRARY lib${PROJECT_NAME})
set(EXECUTABLE ${PROJECT_NAME})
set(TEST_EXECUTABLE ${EXECUTABLE}-test)

set(KPKG_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(KPKG_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

# https://cmake.org/cmake/help/latest/command/file.html#glob-recurse
file(GLOB_RECURSE LIBRARY_SRC CONFIGURE_DEPENDS "${KPKG_SOURCE_DIR}/src/*.cpp")

# ---------------------------------------------------------------------------------------
# Include CMake module
# ---------------------------------------------------------------------------------------
include(CTest)
include(GNUInstallDirs)

include(Options)
include(Check)
include(ClangTidy)
include(Format)
include(CompilerOptions)

# ---------------------------------------------------------------------------------------
# Find package
# ---------------------------------------------------------------------------------------
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(klib REQUIRED)
find_package(CLI11 REQUIRED)
find_package(semver REQUIRED)
find_package(simdjson REQUIRED)
find_package(Boost 1.75.0 REQUIRED CONFIG)

set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

find_package(PkgConfig REQUIRED)

pkg_check_modules(zlib REQUIRED IMPORTED_TARGET zlib)
pkg_check_modules(openssl REQUIRED IMPORTED_TARGET openssl)
pkg_check_modules(libaria2 REQUIRED IMPORTED_TARGET libaria2)

add_definitions(-DOPENSSL_NO_DEPRECATED)

# ---------------------------------------------------------------------------------------
# Build library json file
# ---------------------------------------------------------------------------------------
file(COPY "${KPKG_SOURCE_DIR}/library/library.json"
     DESTINATION ${KPKG_BINARY_DIR})

add_library(library_json STATIC "${KPKG_SOURCE_DIR}/src/library.s")
target_compile_options(library_json PRIVATE "-Wno-unused-command-line-argument")

# ---------------------------------------------------------------------------------------
# Build static library
# ---------------------------------------------------------------------------------------
add_library(${LIBRARY} STATIC ${LIBRARY_SRC})

target_include_directories(${LIBRARY} PUBLIC "${KPKG_SOURCE_DIR}/include")
target_link_libraries(
  ${LIBRARY}
  PRIVATE library_json
          fmt::fmt
          spdlog::spdlog
          semver::semver
          simdjson::simdjson
          ${Boost_LIBRARIES}
          klib::klib
          PkgConfig::libaria2
          PkgConfig::zlib
          PkgConfig::openssl
          ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(${LIBRARY} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

# ---------------------------------------------------------------------------------------
# Build executable
# ---------------------------------------------------------------------------------------
add_subdirectory(tool)

# ---------------------------------------------------------------------------------------
# Build test
# ---------------------------------------------------------------------------------------
if(BUILD_TESTING AND KPKG_BUILD_TEST)
  message(STATUS "Build test")
  add_subdirectory(test)
endif()

# ---------------------------------------------------------------------------------------
# Install target
# ---------------------------------------------------------------------------------------
include(Install)

# https://gitlab.kitware.com/cmake/community/-/wikis/FAQ#can-i-do-make-uninstall-with-cmake
configure_file("${KPKG_SOURCE_DIR}/cmake/CMakeUninstall.cmake.in"
               "${KPKG_BINARY_DIR}/CMakeUninstall.cmake" IMMEDIATE @ONLY)
add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P
                                    ${KPKG_BINARY_DIR}/CMakeUninstall.cmake)
