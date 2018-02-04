#include <unittest-cpp/StreamTestReporter.h>

#include <cstdio>
#include <sstream>
#include <iomanip>
#include <unittest-cpp/TestDetails.h>
#include <unittest-cpp/TestDetailedResult.h>
#include <unittest-cpp/TestResults.h>

#include <unittest-cpp/Unused.h>

using namespace std;

namespace UnitTestCpp
{

const std::string StreamTestReporter::TestRunSeparator =     "[===========]";
const std::string StreamTestReporter::TestSuiteSeparator =   "[   SUITE   ]";
const std::string StreamTestReporter::TestFixtureSeparator = "[  FIXTURE  ]";
const std::string StreamTestReporter::TestSuccessSeparator = "[ SUCCEEDED ]";
const std::string StreamTestReporter::TestFailSeparator =    "[  FAILED   ]";

void StreamTestReporter::ReportTestRunStart(int numberOfTestSuites,
                                            int numberOfTestFixtures,
                                            int numberOfTests)
{
    _stream << TestRunSeparator << " "
            << TestRunStartMessage(numberOfTestSuites, numberOfTestFixtures, numberOfTests) << endl;
}

void StreamTestReporter::ReportTestRunFinish(int numberOfTestSuites,
                                             int numberOfTestFixtures,
                                             int numberOfTests,
                                             int milliSecondsElapsed)
{
    _stream << TestRunSeparator << " "
            << TestRunFinishMessage(numberOfTestSuites, numberOfTestFixtures, numberOfTests,
                                    milliSecondsElapsed) << endl;
}

void StreamTestReporter::ReportTestRunSummary(const TestResults * results, int milliSecondsElapsed)
{
    _stream << TestRunSummaryMessage(results, milliSecondsElapsed) << endl;
}

void StreamTestReporter::ReportTestRunOverview(const TestResults * results)
{
    _stream << TestRunOverviewMessage(results) << endl;
}

void StreamTestReporter::ReportTestSuiteStart(const std::string & suiteName,
                                              int numberOfTestFixtures)
{
    _stream << TestSuiteSeparator << " "
            << TestSuiteStartMessage(suiteName, numberOfTestFixtures) << endl;
}

void StreamTestReporter::ReportTestSuiteFinish(const std::string & suiteName,
                                               int numberOfTestFixtures,
                                               int milliSecondsElapsed)
{
    _stream << TestSuiteSeparator << " "
            << TestSuiteFinishMessage(suiteName, numberOfTestFixtures, milliSecondsElapsed) << endl;
}

void StreamTestReporter::ReportTestFixtureStart(const std::string & fixtureName,
                                                int numberOfTests)
{
    _stream << TestFixtureSeparator << " "
            << TestFixtureStartMessage(fixtureName, numberOfTests) << endl;
}

void StreamTestReporter::ReportTestFixtureFinish(const std::string & fixtureName,
                                                 int numberOfTests,
                                                 int milliSecondsElapsed)
{
    _stream << TestFixtureSeparator << " "
			<< TestFixtureFinishMessage(fixtureName, numberOfTests, milliSecondsElapsed) << endl;
}

void StreamTestReporter::ReportTestStart(const TestDetails & details)
{
    DeferredTestReporter::ReportTestStart(details);
}

void StreamTestReporter::ReportTestFinish(const TestDetails & details, bool success,
                                          int milliSecondsElapsed)
{
    DeferredTestReporter::ReportTestFinish(details, success, milliSecondsElapsed);
    if (success)
        _stream << TestSuccessSeparator;
    else
        _stream << TestFailSeparator;

    _stream << " " << TestFinishMessage(details, success, milliSecondsElapsed) << endl;
}

void StreamTestReporter::ReportTestFailure(const TestDetails & details, const std::string & failure)
{
    DeferredTestReporter::ReportTestFailure(details, failure);
}

static std::string TestLiteral(int numberOfTests)
{
    return (numberOfTests == 1) ? "test" : "tests";
}

static std::string TestFailureLiteral(int numberOfTestFailures)
{
    return (numberOfTestFailures == 1) ? "failure" : "failures";
}

static std::string TestFixtureLiteral(int numberOfTestFixtures)
{
    return (numberOfTestFixtures == 1) ? "test fixture" : "test fixtures";
}

static std::string TestSuiteLiteral(int numberOfTestSuites)
{
    return (numberOfTestSuites == 1) ? "test suite" : "test suites";
}

static std::string TestSuiteName(std::string name)
{
    return (!name.empty()) ? name : "DefaultSuite";
}

static std::string TestFixtureName(std::string name)
{
    return (!name.empty()) ? name : "DefaultFixture";
}

static std::string TestName(const std::string & suiteName,
                            const std::string & fixtureName,
                            const std::string & testName)
{
    basic_ostringstream<char> stream;
    if (!suiteName.empty())
        stream << suiteName << "::";
    if (!fixtureName.empty())
        stream << fixtureName << "::";
    stream << testName;
    return stream.str();
}

std::string StreamTestReporter::TestRunStartMessage(int numberOfTestSuites,
                                                    int numberOfTestFixtures,
                                                    int numberOfTests)
{
    basic_ostringstream<char> stream;
    stream << "Running " << numberOfTests << " " << TestLiteral(numberOfTests)
           << " from " << numberOfTestFixtures << " " << TestFixtureLiteral(numberOfTestFixtures)
           << " in " << numberOfTestSuites << " " << TestSuiteLiteral(numberOfTestSuites) << " .";
    return stream.str();
}

std::string StreamTestReporter::TestRunFinishMessage(int numberOfTestSuites,
                                                      int numberOfTestFixtures,
                                                      int numberOfTests,
                                                      int milliSecondsElapsed)
{
    basic_ostringstream<char> stream;
    stream << numberOfTests << " " << TestLiteral(numberOfTests)
           << " from " << numberOfTestFixtures << " " << TestFixtureLiteral(numberOfTestFixtures)
           << " in " << numberOfTestSuites << " " << TestSuiteLiteral(numberOfTestSuites)
           << " ran. (" << milliSecondsElapsed << " ms total)";
    return stream.str();
}

std::string StreamTestReporter::TestRunSummaryMessage(const TestResults * results,
                                                 int milliSecondsElapsed)
{
    basic_ostringstream<char> stream;

    if (results->GetFailureCount() > 0)
        stream << "FAILURE: " << results->GetFailedTestCount()
               << " out of " << results->GetTotalTestCount()
               << " tests failed (" << results->GetFailureCount() << " "
               << TestFailureLiteral(results->GetFailureCount()) << ")." << endl;
    else
        stream << "Success: " << results->GetTotalTestCount() << " "
               << TestLiteral(results->GetTotalTestCount()) << " passed." << endl;

    stream << "Test time: " << milliSecondsElapsed << " ms." << endl;

    return stream.str();
}

std::string StreamTestReporter::TestRunOverviewMessage(const TestResults * results)
{
    basic_ostringstream<char> stream;

    if (results->GetFailureCount() > 0)
    {
        stream << "Failures:" << endl;
        for (auto testResult : Results())
        {
            if (testResult.Failed())
            {
                for (auto failure : testResult.Failures())
                    stream << testResult.fileName << ":" << failure.first << " : Failure in "
                           << TestName(testResult.suiteName, testResult.fixtureName,
                                       testResult.testName) << ": "
                           << failure.second << endl;
            }
        }
    }
    else
        stream << "No failures" << endl;

    return stream.str();
}

std::string StreamTestReporter::TestSuiteStartMessage(const std::string & suiteName,
                                                       int numberOfTestFixtures)
{
    basic_ostringstream<char> stream;
    stream << TestSuiteName(suiteName) << " (" << numberOfTestFixtures << " "
           << TestFixtureLiteral(numberOfTestFixtures) << ")";
    return stream.str();
}

std::string StreamTestReporter::TestSuiteFinishMessage(const std::string & suiteName,
                                                        int numberOfTestFixtures,
                                                        int milliSecondsElapsed)
{
    basic_ostringstream<char> stream;
    stream << numberOfTestFixtures << " " << TestFixtureLiteral(numberOfTestFixtures)
           << " from " << TestSuiteName(suiteName)
           << " (" << milliSecondsElapsed << " ms total)" << endl;
    return stream.str();
}

std::string StreamTestReporter::TestFixtureStartMessage(const std::string & fixtureName, int numberOfTests)
{
    basic_ostringstream<char> stream;
    stream << TestFixtureName(fixtureName)
           << " (" << numberOfTests << " " << TestLiteral(numberOfTests) << ")";
    return stream.str();
}

std::string StreamTestReporter::TestFixtureFinishMessage(const std::string & fixtureName, int numberOfTests,
                                                          int milliSecondsElapsed)
{
    basic_ostringstream<char> stream;
    stream << numberOfTests << " " << TestLiteral(numberOfTests)
           << " from " << TestFixtureName(fixtureName)
           << " (" << milliSecondsElapsed << " ms total)" << endl;
    return stream.str();
}

std::string StreamTestReporter::TestStartMessage(const std::string & fixtureName, const std::string & testName)
{
    basic_ostringstream<char> stream;
    stream << TestName("", fixtureName, testName);
    return stream.str();
}

std::string StreamTestReporter::TestFinishMessage(const TestDetails & details,
                                                   bool UNUSED(success),
                                                   int milliSecondsElapsed)
{
    basic_ostringstream<char> stream;
    stream << TestName(details.suiteName, details.fixtureName, details.testName)
           << " (" << milliSecondsElapsed << " ms)";
    return stream.str();
}

} // namespace UnitTestCpp
