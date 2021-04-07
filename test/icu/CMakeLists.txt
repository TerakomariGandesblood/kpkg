# https://cmake.org/cmake/help/latest/module/FindICU.html
find_package(ICU REQUIRED COMPONENTS uc io i18n)
add_executable(icu main.cpp)
target_link_libraries(icu ${ICU_LIBRARIES})
