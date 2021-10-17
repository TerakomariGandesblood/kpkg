set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

find_package(Boost CONFIG REQUIRED)
add_definitions(-DBOOST_ASIO_NO_DEPRECATED -DBOOST_ASIO_STANDALONE)

add_executable(asio main.cpp)
target_link_libraries(asio PRIVATE ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

add_custom_command(
  TARGET asio
  POST_BUILD
  COMMAND asio)
