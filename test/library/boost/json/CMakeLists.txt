find_package(Boost 1.75.0 CONFIG REQUIRED COMPONENTS json)
# FIXME add_definitions(-DBOOST_JSON_STANDALONE)
add_executable(json main.cpp)
target_link_libraries(json ${Boost_LIBRARIES})
add_custom_command(
  TARGET json
  POST_BUILD
  COMMAND json)
