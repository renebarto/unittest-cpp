#pragma once

#include <unittest-cpp/PlatformDefines.h>

#if defined(WIN_MSVC) || defined(WIN_MINGW)
    #if defined(UNIT_TEST_CPP_DYNAMIC)
        #if defined(UNIT_TEST_CPP_BUILD)
//            #pragma message("unittest-cpp Windows DLL export")
            #define UNIT_TEST_CPP_EXPORT __declspec(dllexport)
        #else // !defined(UNIT_TEST_CPP_BUILD)
//            #pragma message("unittest-cpp Windows DLL import")
            #define UNIT_TEST_CPP_EXPORT __declspec(dllimport)
        #endif // defined(UNIT_TEST_CPP_BUILD)
    #else // !defined(UNIT_TEST_CPP_DYNAMIC)
//        #pragma message("unittest-cpp Windows Lib")
        #define UNIT_TEST_CPP_EXPORT
    #endif // defined(UNIT_TEST_CPP_DYNAMIC)
#elif defined(LINUX) | defined(DARWIN)
    #define UNIT_TEST_CPP_EXPORT
#else
    #error "Unknown platform"
#endif
