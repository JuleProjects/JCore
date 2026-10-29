set(MODULE_NAME observer)
project(${GROUP_NAME}_${MODULE_NAME})

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
    GTest::gtest_main
    JCore::observer)

gtest_discover_tests(${PROJECT_NAME})
