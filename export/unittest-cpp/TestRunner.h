#pragma once

#include <regex>
#include <unittest-cpp/exports.h>
#include <unittest-cpp/ITestReporter.h>
#include <unittest-cpp/TestRegistry.h>

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
    virtual bool operator () (const TestFixtureInfo * const ) const = 0;
    virtual bool operator () (const TestSuiteInfo * const ) const = 0;
    virtual bool IsValid() const = 0;
};

struct True : public Selector
{
    bool operator () (const TestInfo * const ) const override { return true; }
    bool operator () (const TestFixtureInfo * const ) const override { return true; }
    bool operator () (const TestSuiteInfo * const ) const override { return true; }
    virtual bool IsValid() const override { return true; }
};

struct UNIT_TEST_CPP_EXPORT InSelection : public Selector
{
    InSelection(const char * suiteName, const char * fixtureName, const char * testName)
        : suiteName(suiteName)
        , fixtureName(fixtureName)
        , testName(testName)
    {
    }
    bool operator () (const TestInfo * const test) const override;
    bool operator () (const TestFixtureInfo * const fixture) const override;
    bool operator () (const TestSuiteInfo * const suite) const override;
    virtual bool IsValid() const override { return true; }

private:
    const char * suiteName;
    const char * fixtureName;
    const char * testName;
};

struct UNIT_TEST_CPP_EXPORT InSelectionFilter : public Selector
{
    InSelectionFilter(const std::string & filter);
    bool operator () (const TestInfo * const test) const override;
    bool operator () (const TestFixtureInfo * const fixture) const override;
    bool operator () (const TestSuiteInfo * const suite) const override;
    virtual bool IsValid() const override;

private:
    std::regex matcherSuite;
    std::regex matcherFixture;
    std::regex matcherTest;
    bool isValid;
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
int TestRunner::RunTestsIf(const TestRegistry & registry,
                           const Predicate & predicate,
                           int maxTestTimeInMs)
{
    Start(predicate);

    TestSuiteInfo * curTestSuite = registry.GetHead();

    while (curTestSuite)
    {
        if (predicate(curTestSuite))
            curTestSuite->RunIf(predicate, maxTestTimeInMs, testResults);

        curTestSuite = curTestSuite->next;
    }

    return Finish(predicate);
}

template <class Predicate>
void TestRunner::ListTestsIf(const TestRegistry & registry,
                             const Predicate & predicate)
{
    TestSuiteInfo * curTestSuite = registry.GetHead();

    while (curTestSuite)
    {
        if (predicate(curTestSuite))
            curTestSuite->ListIf(predicate, testResults);

        curTestSuite = curTestSuite->next;
    }
}

template <class Predicate>
void TestRunner::Start(const Predicate & predicate) const
{
    TestRegistry & registry = Test::GetTestRegistry();
    int numberOfTestSuites = registry.CountSuitesIf(predicate);
    int numberOfTestFixtures = registry.CountFixturesIf(predicate);
    int numberOfTests = registry.CountTestsIf(predicate);
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
    int numberOfTestSuites = registry.CountSuitesIf(predicate);
    int numberOfTestFixtures = registry.CountFixturesIf(predicate);
    int numberOfTests = registry.CountTestsIf(predicate);
    reporter->ReportTestRunFinish(numberOfTestSuites, numberOfTestFixtures, numberOfTests,
                                  milliSecondsElapsed);

    return testResults->GetFailureCount();
}

} // namespace UnitTestCpp
