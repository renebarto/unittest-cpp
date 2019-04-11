#pragma once

#include <string>
#include <vector>
#include <unittest-cpp/exports.h>
#include <unittest-cpp/Strings.h>

namespace UnitTestCpp
{

class ITestReporter;
class TestDetails;
class TestSuiteInfo;
class TestFixtureInfo;
class TestInfo;

class UNIT_TEST_CPP_EXPORT TestResults
{
public:
    TestResults(const TestResults &) = delete;
    explicit TestResults(ITestReporter* reporter = nullptr);
    virtual ~TestResults();

    TestResults & operator = (const TestResults &) = delete;

    void OnTestSuiteStart(const TestSuiteInfo & suiteInfo);
    void OnTestSuiteFinish(const TestSuiteInfo & suiteInfo, int milliSecondsElapsed);
    void OnTestFixtureStart(const TestFixtureInfo & fixtureInfo);
    void OnTestFixtureFinish(const TestFixtureInfo & fixtureInfo, int milliSecondsElapsed);
    void OnTestStart(const TestDetails & details);
    void OnTestFailure(const TestDetails & details, const std::string & message);
    void OnTestFinish(const TestDetails & details, int milliSecondsElapsed);

    void OnTestSuiteList(const TestSuiteInfo & suiteInfo);
    void OnTestFixtureList(const TestFixtureInfo & fixtureInfo);
    void OnTestList(const TestDetails & details);

    int GetTotalTestCount() const;
    int GetPassedTestCount() const;
    int GetFailedTestCount() const;
    int GetFailureCount() const;

private:
    ITestReporter * reporter;
    int totalTestCount;
    int failedTestCount;
    int failureCount;
    bool currentTestFailed;
};

} // namespace UnitTestCpp
