find_package(Boost CONFIG REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

add_definitions(-DBOOST_ASIO_NO_DEPRECATED -DBOOST_ASIO_STANDALONE)
add_executable(asio main.cpp)
target_link_libraries(asio ${CMAKE_THREAD_LIBS_INIT})

if(KPKG_USE_LIBCXX)
  target_link_libraries(asio PRIVATE ${KPKG_LIBCXX} ${KPKG_LIBCXXABI})
endif()

add_custom_command(
  TARGET asio
  POST_BUILD
  COMMAND asio)
