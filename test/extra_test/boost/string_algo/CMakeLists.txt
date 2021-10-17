find_package(Boost CONFIG REQUIRED)

add_executable(string-algo main.cpp)
target_link_libraries(string-algo PRIVATE ${Boost_LIBRARIES})

add_custom_command(
  TARGET string-algo
  POST_BUILD
  COMMAND string-algo)
