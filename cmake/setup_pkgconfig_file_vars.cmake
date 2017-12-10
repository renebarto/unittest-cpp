macro(setup_pkgconfig_file_vars)
    set(PKGCONFIG_FILE_PREFIX ${CMAKE_INSTALL_PREFIX})
    set(PKGCONFIG_FILE_EXEC_PREFIX ${PKGCONFIG_FILE_PREFIX})
    set(PKGCONFIG_FILE_BIN_DIR ${PKGCONFIG_FILE_EXEC_PREFIX}/bin)
    set(PKGCONFIG_FILE_LIB_DIR ${PKGCONFIG_FILE_EXEC_PREFIX}/lib)
    set(PKGCONFIG_FILE_INCLUDE_DIR ${PKGCONFIG_FILE_PREFIX}/include)
    set(PKGCONFIG_FILE_DATA_ROOT_DIR ${PKGCONFIG_FILE_PREFIX}/share)
    set(PKGCONFIG_FILE_DATA_DIR ${PKGCONFIG_FILE_DATA_ROOT_DIR})
    set(PKGCONFIG_FILE_MAN_DIR ${PKGCONFIG_FILE_DATA_ROOT_DIR}/man)
    set(PKGCONFIG_FILE_INFO_DIR ${PKGCONFIG_FILE_DATA_ROOT_DIR}/info)
    set(PKGCONFIG_FILE_PKG_CONFIG_DIR ${PKGCONFIG_FILE_DATA_ROOT_DIR}/pkgconfig)
    if(CMAKE_VERBOSE_MAKEFILE)
        message(STATUS "PKGCONFIG PREFIX        : ${PKGCONFIG_FILE_PREFIX}")
        message(STATUS "PKGCONFIG EXEC_PREFIX   : ${PKGCONFIG_FILE_EXEC_PREFIX}")
        message(STATUS "PKGCONFIG BIN_DIR       : ${PKGCONFIG_FILE_BIN_DIR}")
        message(STATUS "PKGCONFIG LIB_DIR       : ${PKGCONFIG_FILE_LIB_DIR}")
        message(STATUS "PKGCONFIG INCLUDE_DIR   : ${PKGCONFIG_FILE_INCLUDE_DIR}")
        message(STATUS "PKGCONFIG DATA_ROOT_DIR : ${PKGCONFIG_FILE_DATA_ROOT_DIR}")
        message(STATUS "PKGCONFIG DATA_DIR      : ${PKGCONFIG_FILE_DATA_DIR}")
        message(STATUS "PKGCONFIG MAN_DIR       : ${PKGCONFIG_FILE_MAN_DIR}")
        message(STATUS "PKGCONFIG INFO_DIR      : ${PKGCONFIG_FILE_INFO_DIR}")
        message(STATUS "PKGCONFIG PKG_CONFIG_DIR: ${PKGCONFIG_FILE_PKG_CONFIG_DIR}")
    endif()
endmacro()
