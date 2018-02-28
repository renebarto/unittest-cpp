#pragma once

#include <string>
#include <unittest-cpp/exports.h>
#include <unittest-cpp/Strings.h>

namespace UnitTestCpp
{

class AssertException;

class UNIT_TEST_CPP_EXPORT TestDetails
{
public:
    TestDetails(const std::string & testName,
                const std::string & fixtureName,
                const std::string & suiteName,
                const AssertException & e);
    TestDetails(const std::string & testName,
                const std::string & fixtureName,
                const std::string & suiteName,
                const std::string & fileName, int lineNumber);
    TestDetails(const TestDetails & details, int lineNumber);

    const std::string suiteName;
    const std::string fixtureName;
    const std::string testName;
    const std::string fileName;
    const int lineNumber;
};

} // namespace UnitTestCpp
