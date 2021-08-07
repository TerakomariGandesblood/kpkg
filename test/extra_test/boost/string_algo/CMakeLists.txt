find_package(Boost CONFIG REQUIRED)

add_executable(string-algo main.cpp)

add_custom_command(
  TARGET string-algo
  POST_BUILD
  COMMAND string-algo)
