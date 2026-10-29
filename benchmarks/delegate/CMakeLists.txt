project(${GROUP_NAME}_delegate)

find_package(EnTT)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    benchmark::benchmark
    JCore::delegate)

if(EnTT_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE EnTT::EnTT)
    target_compile_definitions(${PROJECT_NAME} PUBLIC WITH_ENTT)
endif()
