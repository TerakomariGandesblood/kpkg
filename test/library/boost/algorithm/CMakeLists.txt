find_package(Boost CONFIG REQUIRED)
add_executable(algorithm main.cpp)

if(KPKG_USE_LIBCXX)
  target_link_libraries(algorithm PRIVATE ${KPKG_LIBCXX} ${KPKG_LIBCXXABI})
endif()

add_custom_command(
  TARGET algorithm
  POST_BUILD
  COMMAND algorithm)
