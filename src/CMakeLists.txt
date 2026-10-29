set(MODULES
    common
    delegate
    elapsed_timer
    ev_queue
    exec_timer
    min_max_tracker
    observer
    state_machine)

add_library(${PKG_NAME} INTERFACE)
add_library(${ROOT_PROJECT_NAME}::${PKG_NAME} ALIAS ${PKG_NAME})

target_include_directories(${PKG_NAME} INTERFACE
    $<BUILD_INTERFACE:${ROOT_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(TARGETS ${PKG_NAME} EXPORT ${ROOT_PROJECT_NAME}Targets)

foreach(MODULE ${MODULES})
    add_subdirectory(${MODULE})
    target_link_libraries(${PKG_NAME} INTERFACE ${PKG_NAME}_${MODULE})
endforeach()
