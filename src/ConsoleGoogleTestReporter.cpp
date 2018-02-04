#include <unittest-cpp/ConsoleGoogleTestReporter.h>

#include <iomanip>
#include <unittest-cpp/TestDetails.h>
#include <unittest-cpp/TestResults.h>
#include <unittest-cpp/TestDetailedResult.h>

using namespace std;

namespace UnitTestCpp
{

static const std::string TestRunSeparator =     "[==========]";
static const std::string TestSuiteSeparator =   "";
static const std::string TestFixtureSeparator = "[----------]";
static const std::string TestStartSeparator =   "[ RUN      ]";
static const std::string TestSuccessSeparator = "[       OK ]";
static const std::string TestPassSeparator =    "[  PASSED  ]";
static const std::string TestFailSeparator =    "[  FAILED  ]";

static std::string TestLiteral(int numberOfTests)
{
    return (numberOfTests == 1) ? "test" : "tests";
}

static std::string TestFixtureLiteral(int numberOfTestFixtures)
{
    return (numberOfTestFixtures == 1) ? "test case" : "test cases";
}

static std::string TestSuiteName(std::string name)
{
    return (!name.empty()) ? name : "DefaultSuite";
}

static std::string TestFixtureName(std::string name)
{
    return (!name.empty()) ? name : "DefaultFixture";
}

static std::string TestName(const std::string & UNUSED(suiteName),
                            const std::string & fixtureName,
                            const std::string & testName)
{
    basic_ostringstream<char> stream;
    if (!fixtureName.empty())
        stream << fixtureName;
    else
        stream << "DefaultFixture";
    stream  << "." << testName;
    return stream.str();
}

void PrintHeader(Console & console, const std::string & header, ConsoleColor color)
{
    if (console.ShouldUseColor())
        console.SetTerminalColor(color);

    console << header;

    if (console.ShouldUseColor())
        console.ResetTerminalColor();
}

void ConsoleGoogleTestReporter::ReportTestRunStart(int UNUSED(numberOfTestSuites),
                                                   int numberOfTestFixtures,
                                                   int numberOfTests)
{
    _console << "Running main() from gtest_main.cc" << endl;
    PrintHeader(_console, ::UnitTestCpp::TestRunSeparator, ConsoleColor::Green);

    _console << " Running " << numberOfTests << " " << TestLiteral(numberOfTests)
            << " from " << numberOfTestFixtures << " " << TestFixtureLiteral(numberOfTestFixtures) << "." << endl;

    PrintHeader(_console, ::UnitTestCpp::TestFixtureSeparator, ConsoleColor::Green);

    _console << " Global test environment set-up." << endl;
}

void ConsoleGoogleTestReporter::ReportTestRunFinish(int UNUSED(numberOfTestSuites),
                                                    int UNUSED(numberOfTestFixtures),
                                                    int UNUSED(numberOfTests),
                                                    int UNUSED(milliSecondsElapsed))
{
}

void ConsoleGoogleTestReporter::ReportTestRunSummary(const TestResults * results, int milliSecondsElapsed)
{
    PrintHeader(_console, ::UnitTestCpp::TestFixtureSeparator, ConsoleColor::Green);

    _console << " Global test environment tear-down" << endl;

    PrintHeader(_console, ::UnitTestCpp::TestRunSeparator, ConsoleColor::Green);

    _console << " " << results->GetTotalTestCount() << " "
             << TestLiteral(results->GetTotalTestCount()) << " ran. ("
             << milliSecondsElapsed << " ms total)" << endl;

    PrintHeader(_console, ::UnitTestCpp::TestPassSeparator, ConsoleColor::Green);

    _console << " " << results->GetPassedTestCount() << " "
             << TestLiteral(results->GetPassedTestCount()) << "." << endl;

    if (results->GetFailureCount() > 0)
    {
        PrintHeader(_console, ::UnitTestCpp::TestFailSeparator, ConsoleColor::Red);

        _console << " " << results->GetFailedTestCount() << " "
                 << TestLiteral(results->GetFailedTestCount()) << ", listed below:" << endl;
    }
}

void ConsoleGoogleTestReporter::ReportTestRunOverview(const TestResults * results)
{
    if (results->GetFailedTestCount() > 0)
    {
        for (auto const & testResult : Results())
        {
            if (testResult.Failed())
            {
                PrintHeader(_console, ::UnitTestCpp::TestFailSeparator, ConsoleColor::Red);

                _console << " " << TestName(testResult.suiteName, testResult.fixtureName, testResult.testName) << endl;
            }
        }
        _console << endl << results->GetFailedTestCount() << " FAILED "
                         << ToUpper(TestLiteral(results->GetFailedTestCount())) << endl;
    }
}

void ConsoleGoogleTestReporter::ReportTestSuiteStart(const std::string & suiteName, int numberOfTestFixtures)
{
    if (!::UnitTestCpp::TestSuiteSeparator.empty())
    {
        PrintHeader(_console, ::UnitTestCpp::TestSuiteSeparator, ConsoleColor::Cyan);

        _console << " " << TestSuiteName(suiteName) << " (" << numberOfTestFixtures << " "
                 << TestFixtureLiteral(numberOfTestFixtures) << ")" << endl;
    }
}

void ConsoleGoogleTestReporter::ReportTestSuiteFinish(const std::string & suiteName,
                                                      int numberOfTestFixtures,
                                                      int milliSecondsElapsed)
{
    if (!::UnitTestCpp::TestSuiteSeparator.empty())
    {
        PrintHeader(_console, ::UnitTestCpp::TestSuiteSeparator, ConsoleColor::Cyan);

        _console << " " << numberOfTestFixtures << " " << TestFixtureLiteral(numberOfTestFixtures)
                 << " from " << TestSuiteName(suiteName)
                 << " (" << milliSecondsElapsed << " ms total)" << endl;
    }
}

void ConsoleGoogleTestReporter::ReportTestFixtureStart(const std::string & fixtureName, int numberOfTests)
{
    if (!::UnitTestCpp::TestFixtureSeparator.empty())
    {
        PrintHeader(_console, ::UnitTestCpp::TestFixtureSeparator, ConsoleColor::Yellow);

        _console << " " << numberOfTests << " " << TestLiteral(numberOfTests)
                 << " from " << TestFixtureName(fixtureName) << endl;
    }
}

void ConsoleGoogleTestReporter::ReportTestFixtureFinish(const std::string & fixtureName,
                                                        int numberOfTests,
                                                        int milliSecondsElapsed)
{
    if (!::UnitTestCpp::TestFixtureSeparator.empty())
    {
        PrintHeader(_console, ::UnitTestCpp::TestFixtureSeparator, ConsoleColor::Yellow);

        _console << " " << numberOfTests << " " << TestLiteral(numberOfTests)
                 << " from " << TestFixtureName(fixtureName)
                 << " (" << milliSecondsElapsed << " ms total)" << endl;
    }
    _console << endl;
}

void ConsoleGoogleTestReporter::ReportTestStart(const TestDetails & details)
{
    if (!::UnitTestCpp::TestStartSeparator.empty())
    {
        PrintHeader(_console, ::UnitTestCpp::TestStartSeparator, ConsoleColor::Yellow);

        _console << " " << TestName(details.suiteName, details.fixtureName, details.testName) << endl;
    }
}

void ConsoleGoogleTestReporter::ReportTestFinish(const TestDetails & details, bool success,
                                                 int milliSecondsElapsed)
{
    if (success)
        PrintHeader(_console, ::UnitTestCpp::TestSuccessSeparator, ConsoleColor::Green);
    else
        PrintHeader(_console, ::UnitTestCpp::TestFailSeparator, ConsoleColor::Red);

    _console << " " << TestName(details.suiteName, details.fixtureName, details.testName)
             << " (" << milliSecondsElapsed << " ms)" << endl;
}

void ConsoleGoogleTestReporter::ReportTestFailure(TestDetails const& details, const std::string & failure)
{
    TestDetailedResult detailedResult(details);
    detailedResult.AddFailure(TestDetailedResult::Failure(details.lineNumber, failure));
    _console << details.fileName << ":" << details.lineNumber << ": Failure" << endl;
    _console << failure << endl;
    _results.push_back(detailedResult);
}

void ConsoleGoogleTestReporter::ReportTestSuiteEntry(const std::string & UNUSED(suiteName))
{
}

void ConsoleGoogleTestReporter::ReportTestFixtureEntry(const std::string & fixtureName)
{
    _console << TestFixtureName(fixtureName) << "." << endl;
}

void ConsoleGoogleTestReporter::ReportTestEntry(const std::string & testName)
{
    _console << "  " << testName << endl;
}

} // namespace UnitTestCpp
