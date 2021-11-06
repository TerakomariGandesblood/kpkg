find_package(Boost REQUIRED CONFIG)

add_executable(uuid main.cpp)
target_link_libraries(uuid PRIVATE ${Boost_LIBRARIES})

add_custom_command(
  TARGET uuid
  POST_BUILD
  COMMAND uuid)
