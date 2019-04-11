#include <unittest-cpp/TestResults.h>

#include <unittest-cpp/ITestReporter.h>
#include <unittest-cpp/TestDetails.h>
#include <unittest-cpp/TestRunner.h>
#include <unittest-cpp/TestFixtureInfo.h>
#include <unittest-cpp/TestSuiteInfo.h>
#include <unittest-cpp/TestInfo.h>

using namespace std;

namespace UnitTestCpp
{

TestResults::TestResults(ITestReporter * testReporter)
    : reporter(testReporter)
    , totalTestCount(0)
    , failedTestCount(0)
    , failureCount(0)
    , currentTestFailed(false)
{
}

TestResults::~TestResults()
{
}

void TestResults::OnTestSuiteStart(const TestSuiteInfo & suiteInfo)
{
    if (reporter)
        reporter->ReportTestSuiteStart(suiteInfo.Name(), CountFixtures(suiteInfo));
}

void TestResults::OnTestSuiteFinish(const TestSuiteInfo & suiteInfo, int milliSecondsElapsed)
{
    if (reporter)
        reporter->ReportTestSuiteFinish(suiteInfo.Name(), CountFixtures(suiteInfo),
                                        milliSecondsElapsed);
}

void TestResults::OnTestFixtureStart(const TestFixtureInfo & fixtureInfo)
{
    if (reporter)
        reporter->ReportTestFixtureStart(fixtureInfo.Name(), CountTests(fixtureInfo));
}

void TestResults::OnTestFixtureFinish(const TestFixtureInfo & fixtureInfo, int milliSecondsElapsed)
{
    if (reporter)
        reporter->ReportTestFixtureFinish(fixtureInfo.Name(), CountTests(fixtureInfo),
                                          milliSecondsElapsed);
}

void TestResults::OnTestStart(const TestDetails & details)
{
    ++totalTestCount;
    currentTestFailed = false;
    if (reporter)
        reporter->ReportTestStart(details);
}

void TestResults::OnTestFailure(const TestDetails & details, const string & result)
{
    ++failureCount;
    if (!currentTestFailed)
    {
        ++failedTestCount;
        currentTestFailed = true;
    }
    if (reporter)
        reporter->ReportTestFailure(details, ToString(result));
}

void TestResults::OnTestFinish(const TestDetails & details, int milliSecondsElapsed)
{
    if (reporter)
        reporter->ReportTestFinish(details, !currentTestFailed, milliSecondsElapsed);
}

void TestResults::OnTestSuiteList(const TestSuiteInfo & suiteInfo)
{
    if (reporter)
        reporter->ReportTestSuiteEntry(suiteInfo.Name());
}

void TestResults::OnTestFixtureList(const TestFixtureInfo & fixtureInfo)
{
    if (reporter)
        reporter->ReportTestFixtureEntry(fixtureInfo.Name());
}

void TestResults::OnTestList(const TestDetails & details)
{
    if (reporter)
        reporter->ReportTestEntry(details.testName);
}

int TestResults::GetTotalTestCount() const
{
    return totalTestCount;
}

int TestResults::GetPassedTestCount() const
{
    return totalTestCount - failedTestCount;
}

int TestResults::GetFailedTestCount() const
{
    return failedTestCount;
}

int TestResults::GetFailureCount() const
{
    return failureCount;
}

} // namespace UnitTestCpp
