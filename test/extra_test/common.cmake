function(add_cxx_compiler_flag_no_check FLAG)
  set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} ${FLAG}"
      PARENT_SCOPE)
endfunction()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_link_options("-static-libstdc++")
add_link_options("-static-libgcc")

add_cxx_compiler_flag_no_check("-Wall")
add_cxx_compiler_flag_no_check("-Wextra")
add_cxx_compiler_flag_no_check("-Wpedantic")
add_cxx_compiler_flag_no_check("-Werror")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

if(KPKG_SANITIZER)
  message(STATUS "Build with AddressSanitizer and UndefinedSanitizer")
  add_cxx_compiler_flag_no_check("-fno-omit-frame-pointer")

  add_cxx_compiler_flag_no_check("-fsanitize=address")
  add_cxx_compiler_flag_no_check("-fsanitize-address-use-after-scope")
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
    # FIXME
    add_cxx_compiler_flag_no_check("-fsanitize-recover=implicit-conversion")
  endif()
endif()

include_directories(../)
