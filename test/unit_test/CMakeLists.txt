file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/LICENSE.tar.gz
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/zlib-v1.2.11.tar.gz
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB_RECURSE TEST_SRC CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

find_package(GTest REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(${TEST_EXECUTABLE} ${TEST_SRC})
target_link_libraries(${TEST_EXECUTABLE} PRIVATE ${LIBRARY} GTest::gtest_main
                                                 OpenSSL::Crypto)

# https://cmake.org/cmake/help/latest/module/GoogleTest.html#command:gtest_discover_tests
gtest_discover_tests(${TEST_EXECUTABLE} DISCOVERY_MODE PRE_TEST)
