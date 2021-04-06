include(AddCXXCompilerFlag)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------------------------------------------------------------------------------------
# Coverage
# ---------------------------------------------------------------------------------------
if(KPKG_BUILD_COVERAGE OR KPKG_BUILD_ALL)
  if(CMAKE_COMPILER_IS_GNUCXX)
    add_cxx_compiler_flag("--coverage")
  else()
    add_cxx_compiler_flag("-fprofile-instr-generate")
    add_cxx_compiler_flag("-fcoverage-mapping")
  endif()
endif()

# ---------------------------------------------------------------------------------------
# libc++
# ---------------------------------------------------------------------------------------
if(KPKG_USE_LIBCXX)
  if(CMAKE_COMPILER_IS_GNUCXX)
    message(FATAL_ERROR "GCC does not support libc++")
  endif()

  message(STATUS "Use libc++")
  add_cxx_compiler_flag("-stdlib=libc++")

  if((CMAKE_BUILD_TYPE STREQUAL "Debug") OR (CMAKE_BUILD_TYPE STREQUAL
                                             "RelWithDebInfo"))
    add_cxx_compiler_flag("-fstandalone-debug")
  endif()
endif()

# ---------------------------------------------------------------------------------------
# lld
# ---------------------------------------------------------------------------------------
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  message(STATUS "Use lld")
  add_link_options("-fuse-ld=lld")
endif()

# ---------------------------------------------------------------------------------------
# Sanitizer
# ---------------------------------------------------------------------------------------
if(KPKG_SANITIZER)
  if(NOT (KPKG_SANITIZER STREQUAL "Thread"))
    add_cxx_compiler_flag("-fno-omit-frame-pointer")
  endif()

  macro(append_address_sanitizer_flags)
    add_cxx_compiler_flag("-fsanitize=address")
    add_cxx_compiler_flag("-fsanitize-address-use-after-scope")
    add_cxx_compiler_flag("-fno-optimize-sibling-calls")
  endmacro()

  macro(append_undefined_sanitizer_flags)
    add_cxx_compiler_flag("-fsanitize=undefined")
    add_cxx_compiler_flag("-fno-sanitize-recover=all")

    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
      add_cxx_compiler_flag("-fsanitize=float-divide-by-zero")
      add_cxx_compiler_flag("-fsanitize=unsigned-integer-overflow")
      add_cxx_compiler_flag("-fsanitize=implicit-conversion")
      add_cxx_compiler_flag("-fsanitize=local-bounds")
      add_cxx_compiler_flag("-fsanitize=nullability")
      add_cxx_compiler_flag("-fsanitize-recover=unsigned-integer-overflow")
    endif()
  endmacro()

  if(KPKG_SANITIZER STREQUAL "Address")
    message(STATUS "Build with AddressSanitizer")
    append_address_sanitizer_flags()
  elseif(KPKG_SANITIZER STREQUAL "Thread")
    message(STATUS "Build with ThreadSanitizer")
    add_cxx_compiler_flag("-fsanitize=thread")
  elseif(KPKG_SANITIZER STREQUAL "Memory")
    if(CMAKE_COMPILER_IS_GNUCXX)
      message(FATAL_ERROR "GCC does not support MemorySanitizer")
    endif()

    message(STATUS "Build with MemorySanitizer")

    add_cxx_compiler_flag("-fsanitize=memory")
    add_cxx_compiler_flag("-fsanitize-memory-track-origins")
    add_cxx_compiler_flag("-fsanitize-memory-use-after-dtor")
    add_cxx_compiler_flag("-fno-optimize-sibling-calls")
  elseif(KPKG_SANITIZER STREQUAL "Undefined")
    message(STATUS "Build with UndefinedSanitizer")
    append_undefined_sanitizer_flags()
  elseif(KPKG_SANITIZER STREQUAL "Address;Undefined")
    message(STATUS "Build with AddressSanitizer and UndefinedSanitizer")
    append_address_sanitizer_flags()
    append_undefined_sanitizer_flags()
  else()
    message(FATAL_ERROR "The Sanitizer is not supported: ${KPKG_SANITIZER}")
  endif()
endif()

# ---------------------------------------------------------------------------------------
# Warning
# ---------------------------------------------------------------------------------------
add_cxx_compiler_flag("-Wall")
add_cxx_compiler_flag("-Wextra")
add_cxx_compiler_flag("-Wpedantic")
add_cxx_compiler_flag("-Werror")

 if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
   add_cxx_compiler_flag("-Weverything")
   add_cxx_compiler_flag("-Wno-c++98-compat-pedantic")
   #add_cxx_compiler_flag("-Wno-disabled-macro-expansion")
   add_cxx_compiler_flag("-Wno-missing-prototypes")
 endif()
