#pragma once

#include <string>
#include <unittest-cpp/exports.h>
#include <unittest-cpp/TestInfo.h>

namespace UnitTestCpp
{

class TestRegistry;
class TestResults;

class UNIT_TEST_CPP_EXPORT Test : public TestInfo
{
public:
    Test(const Test &) = delete;
    explicit Test(const std::string & testName,
                  const std::string & fixtureName = {},
                  const std::string & suiteName = {},
                  const std::string & fileName = {}, int lineNumber = 0);
    virtual ~Test();

    Test & operator = (const Test &) = delete;

    void Run();

    Test * _next;

    static TestRegistry & GetTestRegistry();

    void Run(int const maxTestTimeInMs, TestResults * testResults);
    void List(TestResults * testResults);

    virtual void RunImpl() const;
};

} // namespace UnitTestCpp
