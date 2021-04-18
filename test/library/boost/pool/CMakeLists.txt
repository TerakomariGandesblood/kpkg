find_package(Boost CONFIG REQUIRED)

add_executable(pool main.cpp)

if(KPKG_USE_LIBCXX)
  target_link_libraries(pool PRIVATE ${KPKG_LIBCXX} ${KPKG_LIBCXXABI})
endif()

add_custom_command(
  TARGET pool
  POST_BUILD
  COMMAND pool)
