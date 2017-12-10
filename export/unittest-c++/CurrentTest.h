#pragma once

#include <exception>
#include <string>
#include <unittest-c++/exports.h>
#include <unittest-c++/Strings.h>

namespace UnitTestCpp
{

class TestResults;
class TestDetails;

namespace CurrentTest
{
    UNIT_TEST_CPP_EXPORT TestResults * & Results();
    UNIT_TEST_CPP_EXPORT const TestDetails * & Details();

    UNIT_TEST_CPP_EXPORT void UnhandledException(const std::string & context, int line, const std::exception & e);
    UNIT_TEST_CPP_EXPORT void UnhandledException(const std::string & context, int line);
}

} // namespace UnitTestCpp