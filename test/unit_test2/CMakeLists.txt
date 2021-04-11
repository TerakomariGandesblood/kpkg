file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/LICENSE.tar.gz
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/zlib-v1.2.11.tar.gz
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB_RECURSE TEST_SRC CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

find_package(Catch2 REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(catch ${TEST_SRC})
target_link_libraries(catch PRIVATE ${LIBRARY} Catch2::Catch2WithMain
                                    OpenSSL::Crypto)

#include(Catch)
#catch_discover_tests(catch REPORTER compact)
