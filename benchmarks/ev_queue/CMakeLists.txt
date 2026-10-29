project(${GROUP_NAME}_ev_queue)

find_package(EnTT)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    benchmark::benchmark
    spdlog::spdlog
    JCore::ev_queue)

if(EnTT_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE EnTT::EnTT)
    target_compile_definitions(${PROJECT_NAME} PRIVATE WITH_ENTT)
endif()
