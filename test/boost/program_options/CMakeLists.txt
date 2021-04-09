find_package(Boost CONFIG REQUIRED COMPONENTS program_options)
add_executable(program-options main.cpp)
target_link_libraries(program-options ${Boost_LIBRARIES})
add_custom_command(
  TARGET program-options
  POST_BUILD
  COMMAND program-options -h)
