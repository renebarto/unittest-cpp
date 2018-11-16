project(unittest-cpp.test CXX)

include(setup_target_properties_executable)
include(show_target_properties)

message("\n** Setting up unittest-cpp.test **\n")

set(PACKAGE_NAME ${PROJECT_NAME})
set(TARGET_NAME ${PROJECT_NAME})
set(PACKAGE_DESCRIPTION "unittest-cpp test application")

set(PACKAGE_COMPILER_DEFINITIONS_PRIVATE
    )

set(PACKAGE_COMPILER_DEFINITIONS
    ${COMPILER_DEFINITIONS})

set(PACKAGE_COMPILER_OPTIONS
    ${COMPILER_OPTIONS_CXX}
    ${PLATFORM_LIBRARIES_STATIC})

set(PACKAGE_INCLUDE_DIRS
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

set(PACKAGE_LINK_OPTIONS
    ${LINKER_OPTIONS}
    ${LINK_RPATH})

set(PACKAGE_DEPENDENCIES
    unittest-cpp)

set(PACKAGE_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
    ${LINKER_LIBRARIES}
    ${PACKAGE_DEPENDENCIES})

file(GLOB PACKAGE_SOURCES src/*.cpp)
file(GLOB PACKAGE_SOURCES_TEST src/test/*.cpp)
file(GLOB_RECURSE PACKAGE_INCLUDES_PUBLIC include/*.h)
set(PACKAGE_INCLUDES_PUBLIC_PRIVATE
    )
list(APPEND PACKAGE_SOURCES ${PACKAGE_SOURCES_TEST})

set(PACKAGE_BUILD_REFERENCE ${VERSION})

define_target_executable()

show_target_properties(${PACKAGE_NAME})

if (NOT LOCAL_BUILD)
    add_install_target(${PACKAGE_NAME} install-components)
    add_uninstall_target(${PACKAGE_NAME} uninstall-components)

    install(TARGETS ${PACKAGE_NAME}
        EXPORT unittest-cpp
        RUNTIME DESTINATION ${PKGCONFIG_FILE_BIN_DIR}
        COMPONENT ${PACKAGE_NAME})
endif()
