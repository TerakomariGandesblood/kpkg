cmake_minimum_required(VERSION 3.17)

# ---------------------------------------------------------------------------------------
# Start kpkg project
# ---------------------------------------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(
  kpkg
  VERSION 0.1.0
  LANGUAGES ASM CXX)

message(STATUS "Build ${PROJECT_NAME}: ${PROJECT_VERSION}")

# ---------------------------------------------------------------------------------------
# Set default build to release
# ---------------------------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      "Release"
      CACHE STRING "Choose Release, Debug, MinSizeRel or RelWithDebInfo" FORCE)
endif()
message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})

# ---------------------------------------------------------------------------------------
# Set variables
# ---------------------------------------------------------------------------------------
set(LIBRARY lib${PROJECT_NAME})
set(EXECUTABLE ${PROJECT_NAME})
set(TESTS_EXECUTABLE catch)

set(KPKG_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(KPKG_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB_RECURSE LIBRARY_SRC CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE EXECUTABLE_SRC CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/tool/*.cpp")

# ---------------------------------------------------------------------------------------
# Include
# ---------------------------------------------------------------------------------------
include(GNUInstallDirs)

include(Options)
include(Check)
include(ClangTidy)
include(CompilerOptions)
include(Format)
include(LTO)

find_package(Boost CONFIG REQUIRED COMPONENTS json program_options)
find_package(CURL CONFIG REQUIRED)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(ZLIB REQUIRED)
find_package(LibArchive REQUIRED)

# ---------------------------------------------------------------------------------------
# Build port
# ---------------------------------------------------------------------------------------
message(STATUS "Build port")

file(GLOB_RECURSE PORT_SRC CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/port.s")
add_library(port STATIC ${PORT_SRC})
target_compile_options(port PRIVATE "-Wno-unused-command-line-argument")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/port/port.json"
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# ---------------------------------------------------------------------------------------
# Build executable
# ---------------------------------------------------------------------------------------
add_library(${LIBRARY} STATIC ${LIBRARY_SRC})
target_include_directories(${LIBRARY}
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(
  ${LIBRARY}
  PRIVATE port
          CURL::libcurl
          fmt::fmt
          spdlog::spdlog
          ${LibArchive_LIBRARIES}
          ${ZLIB_LIBRARIES}
          ${Boost_LIBRARIES})
set_target_properties(${LIBRARY} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}
                                            DEBUG_POSTFIX d)

add_executable(${EXECUTABLE} ${EXECUTABLE_SRC})
target_link_libraries(${EXECUTABLE} PRIVATE ${LIBRARY})

# ---------------------------------------------------------------------------------------
# Install target
# ---------------------------------------------------------------------------------------
if(KPKG_INSTALL)
  message(STATUS "Generating install")
  include(Install)
endif()

add_subdirectory(test)
