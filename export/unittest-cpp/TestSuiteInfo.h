#pragma once

#include <string>
#include <unittest-cpp/exports.h>
#include <unittest-cpp/TestFixtureInfo.h>
#include <unittest-cpp/TestResults.h>

namespace UnitTestCpp
{

class UNIT_TEST_CPP_EXPORT TestSuiteInfo
{
public:
    TestSuiteInfo() = delete;
    TestSuiteInfo(const TestSuiteInfo &) = delete;
    TestSuiteInfo(const std::string & suiteName);
    virtual ~TestSuiteInfo();

    TestSuiteInfo & operator = (const TestSuiteInfo &) = delete;

    TestFixtureInfo * GetTestFixture(const std::string & fixtureName);

    TestFixtureInfo * GetHead() const;

    const std::string & Name() const;
    TestSuiteInfo * next;

private:
    TestFixtureInfo * head;
    TestFixtureInfo * tail;
    std::string suiteName;

    void AddFixture(TestFixtureInfo * testFixture);
};

} // namespace UnitTestCpp
