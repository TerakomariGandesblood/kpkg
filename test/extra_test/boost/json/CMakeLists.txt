set(Boost_USE_STATIC_LIBS ON)
find_package(Boost CONFIG REQUIRED COMPONENTS json)
# FIXME add_definitions(-DBOOST_JSON_STANDALONE)

add_executable(json main.cpp)
target_link_libraries(json PRIVATE ${Boost_LIBRARIES})

add_custom_command(
  TARGET json
  POST_BUILD
  COMMAND json)
