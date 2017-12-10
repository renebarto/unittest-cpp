cmake_minimum_required(VERSION 2.8)

project(unittest-c++ CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(functions)
include(setup_pkgconfig_file_vars)
include(setup_default_options)
include(define_targets)
include(show_target_properties)

message("\n** Setting up unittest-c++ **\n")

set(LANGUAGE CXX)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)

find_package(Threads REQUIRED)

setup_version()

message(STATUS "Version: ${VERSION}")

set_build_type()
set_config_dir()

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)

set(OUTPUT_BASE_DIR ${CMAKE_SOURCE_DIR}/output)
set(CMAKE_PROGRAM_PATH ${CMAKE_INSTALL_PREFIX}/bin)
set(CMAKE_LIBRARY_PATH ${CMAKE_INSTALL_PREFIX}/lib)
set(CMAKE_INCLUDE_PATH ${CMAKE_INSTALL_PREFIX}/include)

message(STATUS "Build config directory  : ${CMAKE_BINARY_DIR}")
message(STATUS "Sytem root  directory   : ${CMAKE_SYSROOT}")
message(STATUS "Install prefix          : ${CMAKE_INSTALL_PREFIX}")
message(STATUS "CMAKE_PROGRAM_PATH      : ${CMAKE_PROGRAM_PATH}")
message(STATUS "CMAKE_LIBRARY_PATH      : ${CMAKE_LIBRARY_PATH}")
message(STATUS "CMAKE_PROGRAM_PATH      : ${CMAKE_INCLUDE_PATH}")
message(STATUS "Output base directory   : ${OUTPUT_BASE_DIR}")

setup_library_definitions()

if (NOT LOCAL_BUILD)
    add_custom_target(install-components)
    add_custom_target(uninstall-components)
endif()

setup_pkgconfig_file_vars()

setup_default_options()

option(MEASURE_COVERAGE "Determine coverage" OFF)
option(BUILD_UNIT_TESTS "Build unit tests" ON)
option(LOCAL_BUILD "Only perform local build, no install" OFF)
if ("${PLATFORM_NAME}" STREQUAL "")
    set(PLATFORM_NAME "x86_64")
endif()

if ("${TEST_DATA_ROOT}" STREQUAL "")
    if (LOCAL_BUILD)
        set(TEST_DATA_ROOT "${CMAKE_SOURCE_DIR}/testdata")
    else()
        set(TEST_DATA_ROOT "/usr/share/testdata")
    endif()
endif()

set(PACKAGE_NAME ${PROJECT_NAME})
set(TARGET_NAME ${PROJECT_NAME})
set(PACKAGE_DESCRIPTION "Unit test framework")
set(PACKAGE_VERSION_MAJOR 1)
set(PACKAGE_VERSION_MINOR 0)
set(PACKAGE_VERSION_MICRO 0)
set(PACKAGE_VERSION ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_MICRO})

set(PACKAGE_COMPILER_DEFINITIONS_PRIVATE
    UNIT_TEST_CPP_BUILD)

set(PACKAGE_COMPILER_DEFINITIONS
    ${COMPILER_DEFINITIONS}
    UNIT_TEST_CPP_DYNAMIC)

set(PACKAGE_COMPILER_OPTIONS
    ${COMPILER_OPTIONS_CXX}
    )

set(PACKAGE_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/export
    )

set(PACKAGE_LINK_OPTIONS
    ${LINKER_OPTIONS}
    ${LINK_RPATH})

set(PACKAGE_DEPENDENCIES
    )

set(PACKAGE_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${LINKER_LIBRARIES}
    ${PACKAGE_DEPENDENCIES})

file(GLOB_RECURSE PACKAGE_SOURCES src/*.cpp)
file(GLOB_RECURSE PACKAGE_INCLUDES_PUBLIC export/*.h)
file(GLOB_RECURSE PACKAGE_INCLUDES_PRIVATE include/*.h)

set(PACKAGE_BUILD_REFERENCE ${VERSION})

define_target_library_shared()

show_target_properties(${PACKAGE_NAME})

if (BUILD_UNIT_TESTS)
    add_subdirectory(test)
endif()

if (NOT LOCAL_BUILD)
    add_uninstall_target(${PACKAGE_NAME} uninstall-components)
    add_install_target(${PACKAGE_NAME} install-components)

    install_pkgconfig_file(${CMAKE_CURRENT_SOURCE_DIR}/${PACKAGE_NAME}.pc)

    install(TARGETS ${PACKAGE_NAME}
        ARCHIVE DESTINATION ${PKGCONFIG_FILE_LIB_DIR}
        LIBRARY DESTINATION ${PKGCONFIG_FILE_LIB_DIR}
        COMPONENT ${PACKAGE_NAME})
    install(DIRECTORY ${CMAKE_SOURCE_DIR}/export/${PACKAGE_NAME}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
        COMPONENT ${PACKAGE_NAME}
        FILES_MATCHING PATTERN "*.h")
endif()
