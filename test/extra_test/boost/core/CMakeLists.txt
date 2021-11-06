find_package(Boost REQUIRED CONFIG)

add_executable(core main.cpp)
target_link_libraries(core PRIVATE ${Boost_LIBRARIES})

add_custom_command(
  TARGET core
  POST_BUILD
  COMMAND core)
