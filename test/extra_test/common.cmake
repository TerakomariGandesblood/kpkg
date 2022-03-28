function(add_cxx_compiler_flag_no_check FLAG)
  set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} ${FLAG}"
      PARENT_SCOPE)
endfunction()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_cxx_compiler_flag_no_check("-Wall")
add_cxx_compiler_flag_no_check("-Wextra")
add_cxx_compiler_flag_no_check("-Wpedantic")
add_cxx_compiler_flag_no_check("-Werror")

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  add_cxx_compiler_flag_no_check("-Wno-error=unused-command-line-argument")
endif()

# FIXME
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  execute_process(
    COMMAND ld.mold --version
    OUTPUT_VARIABLE LINKER_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  message(STATUS "Linker: ${LINKER_VERSION}")

  add_link_options("-fuse-ld=mold")
else()
  if((${CMAKE_BUILD_TYPE} STREQUAL "Debug") OR (${CMAKE_BUILD_TYPE} STREQUAL
                                                "RelWithDebInfo"))
    execute_process(
      COMMAND ld.lld --version
      OUTPUT_VARIABLE LINKER_VERSION
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    message(STATUS "Linker: ${LINKER_VERSION}")

    add_link_options("-fuse-ld=lld")
  else()
    execute_process(
      COMMAND ld.bfd --version
      OUTPUT_VARIABLE LINKER_VERSION
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    string(REPLACE "\n" ";" LINKER_VERSION ${LINKER_VERSION})
    list(GET LINKER_VERSION 0 LINKER_VERSION)
    message(STATUS "Linker: ${LINKER_VERSION}")

    add_link_options("-fuse-ld=bfd")
  endif()
endif()

add_cxx_compiler_flag_no_check("-pipe")

add_cxx_compiler_flag_no_check("-maes")
add_cxx_compiler_flag_no_check("-march=haswell")

add_link_options("-static-libgcc")
add_link_options("-static-libstdc++")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(((${CMAKE_BUILD_TYPE} STREQUAL "Release") OR (${CMAKE_BUILD_TYPE} STREQUAL
                                                 "MinSizeRel"))
   AND NOT KPKG_SANITIZER)
  include(CheckIPOSupported)
  check_ipo_supported(
    RESULT LTO_SUPPORTED
    OUTPUT ERROR
    LANGUAGES CXX)

  if(LTO_SUPPORTED)
    message(STATUS "Link time optimization: enabled")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
  else()
    message(FATAL_ERROR "Link time optimization not supported: ${ERROR}")
  endif()
else()
  message(STATUS "Link time optimization: disable")
endif()

if((${CMAKE_BUILD_TYPE} STREQUAL "Release") OR (${CMAKE_BUILD_TYPE} STREQUAL
                                                "MinSizeRel"))
  message(STATUS "Discard symbols and other data from object files")
  add_cxx_compiler_flag_no_check("-s")
endif()

if(KPKG_SANITIZER)
  message(STATUS "Build with AddressSanitizer and UndefinedSanitizer")

  add_cxx_compiler_flag_no_check("-fsanitize=address")
  add_cxx_compiler_flag_no_check("-fsanitize-address-use-after-scope")
  add_cxx_compiler_flag_no_check("-fno-omit-frame-pointer")
  add_cxx_compiler_flag_no_check("-fno-optimize-sibling-calls")

  add_cxx_compiler_flag_no_check("-fsanitize=undefined")
  add_cxx_compiler_flag_no_check("-fno-sanitize-recover=all")

  if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_cxx_compiler_flag_no_check("-fsanitize=float-divide-by-zero")
    add_cxx_compiler_flag_no_check("-fsanitize=unsigned-integer-overflow")
    add_cxx_compiler_flag_no_check("-fsanitize=implicit-conversion")
    add_cxx_compiler_flag_no_check("-fsanitize=local-bounds")
    add_cxx_compiler_flag_no_check("-fsanitize=nullability")
    add_cxx_compiler_flag_no_check(
      "-fsanitize-recover=unsigned-integer-overflow")
  endif()

  if(CMAKE_COMPILER_IS_GNUCXX)
    add_cxx_compiler_flag_no_check("-static-libasan")
    add_cxx_compiler_flag_no_check("-static-liblsan")
    add_cxx_compiler_flag_no_check("-static-libubsan")
  else()
    add_cxx_compiler_flag_no_check("-static-libsan")
  endif()
endif()

include_directories(../)
