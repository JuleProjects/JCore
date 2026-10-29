set(GROUP_NAME test)

find_package(GTest REQUIRED)

include(GoogleTest)

set(TESTS
    delegate
    observer
    state_machine)

foreach(TEST ${TESTS})
    add_subdirectory(${TEST})
endforeach()
