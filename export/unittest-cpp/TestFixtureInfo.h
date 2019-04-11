#pragma once

#include <string>
#include <unittest-cpp/exports.h>
#include <unittest-cpp/Test.h>
#include <unittest-cpp/TestResults.h>
#include <unittest-cpp/Timer.h>

namespace UnitTestCpp
{

class TestInfo;

class UNIT_TEST_CPP_EXPORT TestFixtureInfo
{
public:
    TestFixtureInfo() = delete;
    TestFixtureInfo(const std::string & fixtureName);
    TestFixtureInfo(const TestFixtureInfo &) = delete;

    TestFixtureInfo & operator = (const TestFixtureInfo &) = delete;

    void AddTest(Test * test);
    Test * GetHead() const;

    const std::string & Name() const;
    TestFixtureInfo * next;

private:
    Test * head;
    Test * tail;
    std::string fixtureName;
};

} // namespace UnitTestCpp
