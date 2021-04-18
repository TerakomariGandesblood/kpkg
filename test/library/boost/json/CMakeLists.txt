set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.75.0 CONFIG REQUIRED COMPONENTS json)
# FIXME add_definitions(-DBOOST_JSON_STANDALONE)

add_executable(json main.cpp)
target_link_libraries(json PRIVATE ${Boost_LIBRARIES})

if(KPKG_USE_LIBCXX)
  target_link_libraries(json PRIVATE ${KPKG_LIBCXX} ${KPKG_LIBCXXABI})
endif()

add_custom_command(
  TARGET json
  POST_BUILD
  COMMAND json)
