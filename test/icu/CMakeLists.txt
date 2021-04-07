file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data.txt
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# https://cmake.org/cmake/help/latest/module/FindICU.html
find_package(ICU REQUIRED COMPONENTS data i18n io uc)

add_executable(conv conv.cpp)
target_link_libraries(conv ${ICU_LIBRARIES})

add_executable(unicode-string unicode_string.cpp)
target_link_libraries(unicode-string ${ICU_LIBRARIES})

add_executable(transforms transforms.cpp)
target_link_libraries(transforms ${ICU_LIBRARIES})
