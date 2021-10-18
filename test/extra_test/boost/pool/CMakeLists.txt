find_package(Boost REQUIRED CONFIG)

add_executable(pool main.cpp)
target_link_libraries(pool PRIVATE ${Boost_LIBRARIES})

add_custom_command(
  TARGET pool
  POST_BUILD
  COMMAND pool)
