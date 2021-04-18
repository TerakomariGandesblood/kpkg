find_package(Boost CONFIG REQUIRED)

add_executable(string-algo main.cpp)

if(KPKG_USE_LIBCXX)
  target_link_libraries(string-algo PRIVATE ${KPKG_LIBCXX} ${KPKG_LIBCXXABI})
endif()

add_custom_command(
  TARGET string-algo
  POST_BUILD
  COMMAND string-algo)
