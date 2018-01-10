#include <unittest-c++/DeferredTestReporter.h>

#include <unittest-c++/TestDetails.h>

#include <unittest-c++/Unused.h>

using namespace std;

namespace UnitTestCpp
{

void DeferredTestReporter::ReportTestRunStart(int UNUSED(numberOfTestSuites),
                                              int UNUSED(numberOfTestFixtures),
                                              int UNUSED(numberOfTests))
{
}

void DeferredTestReporter::ReportTestRunFinish(int UNUSED(numberOfTestSuites),
                                               int UNUSED(numberOfTestFixtures),
                                               int UNUSED(numberOfTests),
                                               int UNUSED(milliSecondsElapsed))
{
}

void DeferredTestReporter::ReportTestRunSummary(const TestResults * UNUSED(_results),
                                                int UNUSED(milliSecondsElapsed))
{
}

void DeferredTestReporter::ReportTestRunOverview(const TestResults * UNUSED(_results))
{
}

void DeferredTestReporter::ReportTestSuiteStart(const std::string & UNUSED(suiteName),
                                                int UNUSED(numberOfTestFixtures))
{
}

void DeferredTestReporter::ReportTestSuiteFinish(const std::string & UNUSED(suiteName),
                                                 int UNUSED(numberOfTests),
                                                 int UNUSED(milliSecondsElapsed))
{
}

void DeferredTestReporter::ReportTestFixtureStart(const std::string & UNUSED(fixtureName),
                                                  int UNUSED(numberOfTests))
{
}

void DeferredTestReporter::ReportTestFixtureFinish(const std::string & UNUSED(fixtureName),
                                                   int UNUSED(numberOfTests),
                                                   int UNUSED(milliSecondsElapsed))
{
}

void DeferredTestReporter::ReportTestStart(const TestDetails & details)
{
    _results.push_back(TestDetailedResult(details));
}

void DeferredTestReporter::ReportTestFinish(const TestDetails & UNUSED(details),
                                            bool UNUSED(success),
                                            int milliSecondsElapsed)
{
    TestDetailedResult & result = _results.back();
    result.MilliSecondsElapsed(milliSecondsElapsed);
}

void DeferredTestReporter::ReportTestFailure(const TestDetails & details, const std::string & failure)
{
    TestDetailedResult & result = _results.back();
    result.AddFailure(TestDetailedResult::Failure(details.lineNumber, failure));
}

void DeferredTestReporter::ReportTestSuiteEntry(const std::string & UNUSED(suiteName))
{
}

void DeferredTestReporter::ReportTestFixtureEntry(const std::string & UNUSED(fixtureName))
{
}

void DeferredTestReporter::ReportTestEntry(const std::string & UNUSED(testName))
{
}


DeferredTestReporter::ResultList & DeferredTestReporter::Results()
{
    return _results;
}

} // namespace UnitTestCpp
