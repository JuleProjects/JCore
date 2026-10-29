set(MODULE_NAME state_machine)
project(${GROUP_NAME}_${MODULE_NAME})

add_executable(${PROJECT_NAME}
    main.cpp
    event.hpp
    state_a.cpp state_a.hpp
    state_b.cpp state_b.hpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
    GTest::gtest_main
    spdlog::spdlog
    JCore::jcore)

gtest_discover_tests(${PROJECT_NAME})
