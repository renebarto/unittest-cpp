#pragma once

#include <string>
#include <unittest-cpp/exports.h>
#include <unittest-cpp/TestDetails.h>
#include <unittest-cpp/Strings.h>

namespace UnitTestCpp
{

class UNIT_TEST_CPP_EXPORT TestInfo
{
public:
    TestInfo(const TestInfo &) = delete;
    explicit TestInfo(const std::string & testName,
                      const std::string & fixtureName = {},
                      const std::string & suiteName = {},
                      const std::string & fileName = {}, int lineNumber = 0);
    virtual ~TestInfo();

    TestInfo & operator = (const TestInfo &) = delete;

    TestDetails _details;
};

} // namespace UnitTestCpp
