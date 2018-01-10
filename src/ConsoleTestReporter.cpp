#include <unittest-c++/ConsoleTestReporter.h>

#include <iomanip>
#include <unittest-c++/TestDetails.h>
#include <unittest-c++/TestResults.h>

using namespace std;

namespace UnitTestCpp
{

void ConsoleTestReporter::ReportTestRunStart(int numberOfTestSuites,
                                             int numberOfTestFixtures,
                                             int numberOfTests)
{
    if (console.ShouldUseColor())
        console.SetTerminalColor(ConsoleColor::Green);

    _stream << TestRunSeparator;

    if (console.ShouldUseColor())
        console.ResetTerminalColor();

    _stream << " " << TestRunStartMessage(numberOfTestSuites, numberOfTestFixtures,
                                         numberOfTests) << endl;
}

void ConsoleTestReporter::ReportTestRunFinish(int numberOfTestSuites,
                                              int numberOfTestFixtures,
                                              int numberOfTests,
                                              int milliSecondsElapsed)
{
    if (console.ShouldUseColor())
        console.SetTerminalColor(ConsoleColor::Green);

    _stream << TestRunSeparator;

    if (console.ShouldUseColor())
        console.ResetTerminalColor();

    _stream << " " << TestRunFinishMessage(numberOfTestSuites, numberOfTestFixtures, numberOfTests,
                                          milliSecondsElapsed) << endl;
}

void ConsoleTestReporter::ReportTestRunSummary(const TestResults * results, int milliSecondsElapsed)
{
    if (console.ShouldUseColor())
    {
        if (results->GetFailureCount() > 0)
            console.SetTerminalColor(ConsoleColor::Red);
        else
            console.SetTerminalColor(ConsoleColor::Green);
    }

    _stream << TestRunSummaryMessage(results, milliSecondsElapsed) << endl;

    if (console.ShouldUseColor())
        console.ResetTerminalColor();
}

void ConsoleTestReporter::ReportTestRunOverview(const TestResults * results)
{
    _stream << TestRunOverviewMessage(results) << endl;
}

void ConsoleTestReporter::ReportTestSuiteStart(const std::string & suiteName, int numberOfTestFixtures)
{
    if (console.ShouldUseColor())
        console.SetTerminalColor(ConsoleColor::Cyan);

    _stream << TestSuiteSeparator;

    if (console.ShouldUseColor())
        console.ResetTerminalColor();

    _stream << " " << TestSuiteStartMessage(suiteName, numberOfTestFixtures) << endl;
}

void ConsoleTestReporter::ReportTestSuiteFinish(const std::string & suiteName,
                                                int numberOfTestFixtures,
                                                int milliSecondsElapsed)
{
    if (console.ShouldUseColor())
        console.SetTerminalColor(ConsoleColor::Cyan);

    _stream << TestSuiteSeparator;

    if (console.ShouldUseColor())
        console.ResetTerminalColor();

    _stream << " " << TestSuiteFinishMessage(suiteName, numberOfTestFixtures,
                                            milliSecondsElapsed) << endl;
}

void ConsoleTestReporter::ReportTestFixtureStart(const std::string & fixtureName, int numberOfTests)
{
    if (console.ShouldUseColor())
        console.SetTerminalColor(ConsoleColor::Yellow);

    _stream << TestFixtureSeparator;

    if (console.ShouldUseColor())
        console.ResetTerminalColor();

    _stream << " " << TestFixtureStartMessage(fixtureName, numberOfTests) << endl;
}

void ConsoleTestReporter::ReportTestFixtureFinish(const std::string & fixtureName,
                                                  int numberOfTests,
                                                  int milliSecondsElapsed)
{
    if (console.ShouldUseColor())
        console.SetTerminalColor(ConsoleColor::Yellow);

    _stream << TestFixtureSeparator;

    if (console.ShouldUseColor())
        console.ResetTerminalColor();

    _stream << " " << TestFixtureFinishMessage(fixtureName, numberOfTests, milliSecondsElapsed);
}

void ConsoleTestReporter::ReportTestStart(const TestDetails & details)
{
    DeferredTestReporter::ReportTestStart(details);
}

void ConsoleTestReporter::ReportTestFinish(const TestDetails & details, bool success,
                                           int milliSecondsElapsed)
{
    DeferredTestReporter::ReportTestFinish(details, success, milliSecondsElapsed);
    if (console.ShouldUseColor())
    {
        console.SetTerminalColor(success ? ConsoleColor::Green : ConsoleColor::Red);
    }

    if (success)
        _stream << TestSuccessSeparator;
    else
        _stream << TestFailSeparator;

    if (console.ShouldUseColor())
        console.ResetTerminalColor();

    _stream << " " << TestFinishMessage(details, success, milliSecondsElapsed) << endl;
}

void ConsoleTestReporter::ReportTestFailure(TestDetails const& details, const std::string & failure)
{
    DeferredTestReporter::ReportTestFailure(details, failure);
}

void ConsoleTestReporter::ReportTestSuiteEntry(const std::string & suiteName)
{
    _stream << suiteName << "." << endl;
}

void ConsoleTestReporter::ReportTestFixtureEntry(const std::string & fixtureName)
{
    _stream << "    " << fixtureName << "." << endl;
}

void ConsoleTestReporter::ReportTestEntry(const std::string & testName)
{
    _stream << "        " << testName << endl;
}

} // namespace UnitTestCpp
