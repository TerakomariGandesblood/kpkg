find_package(Boost CONFIG REQUIRED)
add_executable(algorithm main.cpp)
add_custom_command(
  TARGET algorithm
  POST_BUILD
  COMMAND algorithm)
