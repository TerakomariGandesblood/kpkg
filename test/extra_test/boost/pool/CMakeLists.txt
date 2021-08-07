find_package(Boost CONFIG REQUIRED)

add_executable(pool main.cpp)

add_custom_command(
  TARGET pool
  POST_BUILD
  COMMAND pool)
