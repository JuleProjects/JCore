set(MODULE_NAME ev_queue)
project(${PKG_NAME}_${MODULE_NAME})

add_library(${PROJECT_NAME} INTERFACE)
add_library(${ROOT_PROJECT_NAME}::${MODULE_NAME} ALIAS ${PROJECT_NAME})

set(HDRS ${ROOT_DIR}/include/${PKG_NAME}/${MODULE_NAME}.hpp)

target_sources(${PROJECT_NAME}
    PRIVATE ${HDRS}
    INTERFACE FILE_SET HEADERS BASE_DIRS ${ROOT_DIR}/include FILES ${HDRS})

target_link_libraries(${PROJECT_NAME}
    INTERFACE
    Boost::headers)

if(readerwriterqueue_FOUND)
    target_link_libraries(${PROJECT_NAME} INTERFACE readerwriterqueue::readerwriterqueue)
    target_compile_definitions(${PROJECT_NAME} INTERFACE WITH_JCORE_TS_EVENT_QUEUE)
endif()

install(TARGETS ${PROJECT_NAME} EXPORT ${ROOT_PROJECT_NAME}Targets FILE_SET HEADERS)
