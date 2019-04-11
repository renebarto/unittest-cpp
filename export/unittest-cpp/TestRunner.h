#pragma once

#include <regex>
#include <unittest-cpp/exports.h>
#include <unittest-cpp/ITestReporter.h>
#include <unittest-cpp/TestRegistry.h>

#include <iostream>

namespace UnitTestCpp
{

class ITestReporter;
class TestInfo;
class TestFixtureInfo;
class TestResults;
class TestSuiteInfo;

struct Selector
{
    virtual bool operator () (const TestInfo * const ) const = 0;
};

struct True : public Selector
{
    bool operator () (const TestInfo * const ) const override { return true; }
};

struct UNIT_TEST_CPP_EXPORT InSelection : public Selector
{
    InSelection(const char * suiteName, const char * fixtureName, const char * testName)
        : fullName(std::string(suiteName) + "." + fixtureName + "." + testName)
    {
    }
    virtual ~InSelection() {}
    bool operator () (const TestInfo * const test) const override;

private:
    std::string fullName;
};

struct UNIT_TEST_CPP_EXPORT FilterMatcher
{
    FilterMatcher(const std::string & filter)
            : matcher(filter)
    {
        std::cout << "Matcher " << filter << std::endl;
    }
    std::regex matcher;
};

struct UNIT_TEST_CPP_EXPORT InSelectionFilter : public Selector
{
    InSelectionFilter(const std::string & filter);
    virtual ~InSelectionFilter() {}
    bool operator () (const TestInfo * const test) const override;

private:
    std::vector<FilterMatcher> positiveMatchers;
    std::vector<FilterMatcher> negativeMatchers;
};

class UNIT_TEST_CPP_EXPORT TestRunner
{
public:
    TestRunner(const TestRunner &) = delete;
    explicit TestRunner(ITestReporter & reporter);
    ~TestRunner();

    TestRunner & operator = (const TestRunner &) = delete;

    template <class Predicate>
    int RunTestsIf(const TestRegistry & registry, const Predicate & predicate, int maxTestTimeInMs);
    template <class Predicate>
    void ListTestsIf(const TestRegistry & registry, const Predicate & predicate);

private:
    ITestReporter * reporter;
    TestResults * testResults;
    Timer * timer;

    template <class Predicate>
    void Start(const Predicate & predicate) const;

    template <class Predicate>
    int Finish(const Predicate & predicate) const;
};

template <class Predicate>
void RunIf(const TestSuiteInfo & testSuite, const Predicate & predicate, int const maxTestTimeInMs, TestResults * testResults);

int UNIT_TEST_CPP_EXPORT CountSuites(const TestRegistry & registry);
int UNIT_TEST_CPP_EXPORT CountFixtures(const TestRegistry & registry);
int UNIT_TEST_CPP_EXPORT CountTests(const TestRegistry & registry);
int UNIT_TEST_CPP_EXPORT CountFixtures(const TestSuiteInfo & testSuite);
int UNIT_TEST_CPP_EXPORT CountTests(const TestSuiteInfo & testSuite);
int UNIT_TEST_CPP_EXPORT CountTests(const TestFixtureInfo & testFixture);

template <typename Predicate>
int CountSuitesIf(const TestRegistry & registry, const Predicate & predicate);
template <typename Predicate>
int CountFixturesIf(const TestRegistry & registry, const Predicate & predicate);
template <typename Predicate>
int CountTestsIf(const TestRegistry & registry, const Predicate & predicate);
template <typename Predicate>
int CountFixturesIf(const TestSuiteInfo & testSuite, const Predicate & predicate);
template <typename Predicate>
int CountTestsIf(const TestSuiteInfo & testSuite, const Predicate & predicate);
template <typename Predicate>
int CountTestsIf(const TestFixtureInfo & testFixture, const Predicate & predicate);

template <class Predicate>
int TestRunner::RunTestsIf(const TestRegistry & registry,
                           const Predicate & predicate,
                           int maxTestTimeInMs)
{
    Start(predicate);

    TestSuiteInfo * curTestSuite = registry.GetHead();

    while (curTestSuite)
    {
        RunIf(*curTestSuite, predicate, maxTestTimeInMs, testResults);
        curTestSuite = curTestSuite->next;
    }

    return Finish(predicate);
}

template <class Predicate>
void RunIf(const TestSuiteInfo & testSuite, const Predicate & predicate, int const maxTestTimeInMs, TestResults * testResults)
{
    if (CountTestsIf(testSuite, predicate) <= 0)
        return;

    Timer testTimerSuite;
    testTimerSuite.Start();

    testResults->OnTestSuiteStart(testSuite);

    TestFixtureInfo * testFixture = testSuite.GetHead();
    while (testFixture)
    {
        if (CountTestsIf(*testFixture, predicate) > 0)
        {
            Timer testTimerFixture;
            testTimerFixture.Start();

            testResults->OnTestFixtureStart(*testFixture);

            Test * test = testFixture->GetHead();
            while (test)
            {
                if (predicate(test))
                    test->Run(maxTestTimeInMs, testResults);
                test = test->_next;
            }

            testResults->OnTestFixtureFinish(*testFixture, testTimerFixture.GetTimeInMilliSeconds());
        }

        testFixture = testFixture->next;
    }

    testResults->OnTestSuiteFinish(testSuite, testTimerSuite.GetTimeInMilliSeconds());
}

template <class Predicate>
void TestRunner::ListTestsIf(const TestRegistry & registry,
                             const Predicate & predicate)
{
    TestSuiteInfo * curTestSuite = registry.GetHead();

    while (curTestSuite)
    {
        if (CountTestsIf(*curTestSuite, predicate) > 0)
        {
            testResults->OnTestSuiteList(*curTestSuite);

            TestFixtureInfo * testFixture = curTestSuite->GetHead();
            while (testFixture)
            {
                if (CountTestsIf(*testFixture, predicate) > 0)
                {
                    testResults->OnTestFixtureList(*testFixture);

                    Test * test = testFixture->GetHead();
                    while (test)
                    {
                        if (predicate(test))
                            test->List(testResults);
                        test = test->_next;
                    }
                }

                testFixture = testFixture->next;
            }
        }
        curTestSuite = curTestSuite->next;
    }
}

template <typename Predicate>
int CountSuitesIf(const TestRegistry & registry, const Predicate & predicate)
{
    int numberOfTestSuites = 0;
    TestSuiteInfo * testSuite = registry.GetHead();
    while (testSuite)
    {
        if (CountFixturesIf(*testSuite, predicate) > 0)
            ++numberOfTestSuites;
        testSuite = testSuite->next;
    }
    return numberOfTestSuites;
}

template <typename Predicate>
int CountFixturesIf(const TestRegistry & registry, const Predicate & predicate)
{
    int numberOfTestFixtures = 0;
    TestSuiteInfo * testSuite = registry.GetHead();
    while (testSuite)
    {
        numberOfTestFixtures += CountFixturesIf(*testSuite, predicate);
        testSuite = testSuite->next;
    }
    return numberOfTestFixtures;
}

template <typename Predicate>
int CountTestsIf(const TestRegistry & registry, const Predicate & predicate)
{
    int numberOfTests = 0;
    TestSuiteInfo * testSuite = registry.GetHead();
    while (testSuite)
    {
        numberOfTests += CountTestsIf(*testSuite, predicate);
        testSuite = testSuite->next;
    }
    return numberOfTests;
}

template <typename Predicate>
int CountFixturesIf(const TestSuiteInfo & testSuite, const Predicate & predicate)
{
    int numberOfTestFixtures = 0;
    TestFixtureInfo * testFixture = testSuite.GetHead();
    while (testFixture)
    {
        if (CountTestsIf(*testFixture, predicate) > 0)
            numberOfTestFixtures++;
        testFixture = testFixture->next;
    }
    return numberOfTestFixtures;
}

template <typename Predicate>
int CountTestsIf(const TestSuiteInfo & testSuite, const Predicate & predicate)
{
    int numberOfTests = 0;
    TestFixtureInfo * testFixture = testSuite.GetHead();
    while (testFixture)
    {
        numberOfTests += CountTestsIf(*testFixture, predicate);
        testFixture = testFixture->next;
    }
    return numberOfTests;
}

template <typename Predicate>
int CountTestsIf(const TestFixtureInfo & testFixture, const Predicate & predicate)
{
    int numberOfTests = 0;
    Test * test = testFixture.GetHead();
    while (test)
    {
        if (predicate(test))
            numberOfTests++;
        test = test->_next;
    }
    return numberOfTests;
}

template <class Predicate>
void TestRunner::Start(const Predicate & predicate) const
{
    TestRegistry & registry = Test::GetTestRegistry();
    int numberOfTestSuites = CountSuitesIf(registry, predicate);
    int numberOfTestFixtures = CountFixturesIf(registry, predicate);
    int numberOfTests = CountTestsIf(registry, predicate);
    this->reporter->ReportTestRunStart(numberOfTestSuites, numberOfTestFixtures, numberOfTests);
    timer->Start();
}

template <class Predicate>
int TestRunner::Finish(const Predicate & predicate) const
{
    int const milliSecondsElapsed = timer->GetTimeInMilliSeconds();
    reporter->ReportTestRunSummary(testResults, milliSecondsElapsed);
    reporter->ReportTestRunOverview(testResults);

    TestRegistry & registry = Test::GetTestRegistry();
    int numberOfTestSuites = CountSuitesIf(registry, predicate);
    int numberOfTestFixtures = CountFixturesIf(registry, predicate);
    int numberOfTests = CountTestsIf(registry, predicate);
    reporter->ReportTestRunFinish(numberOfTestSuites, numberOfTestFixtures, numberOfTests,
                                  milliSecondsElapsed);

    return testResults->GetFailureCount();
}

} // namespace UnitTestCpp
