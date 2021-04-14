find_package(Boost CONFIG REQUIRED COMPONENTS program_options)
add_executable(program-options main.cpp)
target_link_libraries(program-options PRIVATE ${Boost_LIBRARIES})

if(KPKG_USE_LIBCXX)
  target_link_libraries(program-options PRIVATE ${KPKG_LIBCXX}
                                                ${KPKG_LIBCXXABI})
endif()

add_custom_command(
  TARGET program-options
  POST_BUILD
  COMMAND program-options -h)
