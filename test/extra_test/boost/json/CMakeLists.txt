find_package(Boost REQUIRED COMPONENTS json CONFIG)

add_executable(json main.cpp)
target_link_libraries(json PRIVATE ${Boost_LIBRARIES})

add_custom_command(
  TARGET json
  POST_BUILD
  COMMAND json)
