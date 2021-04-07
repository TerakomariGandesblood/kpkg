find_package(Boost CONFIG REQUIRED)
add_definitions(-DBOOST_ASIO_NO_DEPRECATED -DBOOST_ASIO_STANDALONE)
add_executable(asio main.cpp)
target_link_libraries(asio ${Boost_LIBRARIES})
