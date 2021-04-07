# https://cmake.org/cmake/help/latest/module/FindBoost.html
find_package(Boost CONFIG REQUIRED COMPONENTS json)
add_executable(json json.cpp)
target_link_libraries(json ${Boost_LIBRARIES})
