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

    template <class Predicate> void RunIf(const Predicate & predicate, int const maxTestTimeInMs, TestResults * testResults);
    template <class Predicate> void ListIf(const Predicate & predicate, TestResults * testResults);

    int CountTests();
    template <typename Predicate> int CountTestsIf(Predicate predicate);

private:
    Test * head;
    Test * tail;
    std::string fixtureName;
};

template <class Predicate> void TestFixtureInfo::RunIf(const Predicate & predicate, int const maxTestTimeInMs, TestResults * testResults)
{
    Timer testTimer;
    testTimer.Start();

    testResults->OnTestFixtureStart(this);

    Test * test = this->GetHead();
    while (test)
    {
        if (predicate(test))
            test->Run(maxTestTimeInMs, testResults);
        test = test->_next;
    }

    testResults->OnTestFixtureFinish(this, testTimer.GetTimeInMilliSeconds());
}

template <class Predicate> void TestFixtureInfo::ListIf(const Predicate & predicate, TestResults * testResults)
{
    testResults->OnTestFixtureList(this);

    Test * test = this->GetHead();
    while (test)
    {
        if (predicate(test))
            test->List(testResults);
        test = test->_next;
    }
}

template <typename Predicate> int TestFixtureInfo::CountTestsIf(Predicate predicate)
{
    int numberOfTests = 0;
    Test * test = this->GetHead();
    while (test)
    {
        if (predicate(test))
            numberOfTests++;
        test = test->_next;
    }
    return numberOfTests;
}

} // namespace UnitTestCpp