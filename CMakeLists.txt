cmake_minimum_required(VERSION 3.28)
project(JCore VERSION 1.0 LANGUAGES CXX)

set(ROOT_PROJECT_NAME ${PROJECT_NAME})
set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
string(TOLOWER ${PROJECT_NAME} PKG_NAME)

# CXX
if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

add_compile_options(
    "$<$<COMPILE_LANG_AND_ID:CXX,GNU,Clang>:-Wall;-Wextra;-Wpedantic>"
    "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/W4>")

add_compile_definitions(
    "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING>")

# Find packages
find_package(Boost CONFIG REQUIRED)
find_package(readerwriterqueue)
find_package(spdlog REQUIRED)

include(GNUInstallDirs)
include(FetchContent)

if(NOT cmake_utils_POPULATED)
    FetchContent_Declare(
        cmake_utils
        GIT_REPOSITORY "git@github.com:JuleProjects/cmake-utils.git"
        GIT_TAG main)
    FetchContent_MakeAvailable(cmake_utils)
endif()
list(APPEND CMAKE_MODULE_PATH "${cmake_utils_SOURCE_DIR}/modules")

include(FetchContentUtils)

add_subdirectory(src)

if(PROJECT_IS_TOP_LEVEL)
    add_subdirectory(benchmarks)
    enable_testing()
    add_subdirectory(tests)
endif()

include(CMakePackageConfigHelpers)

# Generate ${ROOT_PROJECT_NAME}Config.cmake
configure_package_config_file(
    Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${ROOT_PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/${PKG_NAME})

# Generate ${ROOT_PROJECT_NAME}ConfigVersion.cmake
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${ROOT_PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion)

# Install package configuration
install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${ROOT_PROJECT_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${ROOT_PROJECT_NAME}ConfigVersion.cmake
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/${PKG_NAME})

# Install targets
install(
    EXPORT ${ROOT_PROJECT_NAME}Targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PKG_NAME}
    NAMESPACE ${ROOT_PROJECT_NAME}::)

# Exports ${ROOT_PROJECT_NAME}Targets.cmake
export(EXPORT ${ROOT_PROJECT_NAME}Targets FILE ${ROOT_PROJECT_NAME}Targets.cmake)
