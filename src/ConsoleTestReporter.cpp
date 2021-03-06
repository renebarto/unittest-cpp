#include <unittest-cpp/ConsoleTestReporter.h>

#include <iomanip>
#include <unittest-cpp/Console.h>
#include <unittest-cpp/TestDetails.h>
#include <unittest-cpp/TestResults.h>

using namespace std;

namespace UnitTestCpp
{

ConsoleTestReporter::ConsoleTestReporter(bool useColor)
    : StreamTestReporter(std::cout)
    , _console(std::make_shared<Console>())
{
    _console->ShouldUseColor(useColor);
}

void ConsoleTestReporter::ReportTestRunStart(int numberOfTestSuites,
                                             int numberOfTestFixtures,
                                             int numberOfTests)
{
    if (_console->ShouldUseColor())
        _console->SetTerminalColor(ConsoleColor::Green);

    _stream << TestRunSeparator;

    if (_console->ShouldUseColor())
        _console->ResetTerminalColor();

    _stream << " " << TestRunStartMessage(numberOfTestSuites, numberOfTestFixtures,
                                          numberOfTests) << endl;
}

void ConsoleTestReporter::ReportTestRunFinish(int numberOfTestSuites,
                                              int numberOfTestFixtures,
                                              int numberOfTests,
                                              int milliSecondsElapsed)
{
    if (_console->ShouldUseColor())
        _console->SetTerminalColor(ConsoleColor::Green);

    _stream << TestRunSeparator;

    if (_console->ShouldUseColor())
        _console->ResetTerminalColor();

    _stream << " " << TestRunFinishMessage(numberOfTestSuites, numberOfTestFixtures, numberOfTests,
                                          milliSecondsElapsed) << endl;
}

void ConsoleTestReporter::ReportTestRunSummary(const TestResults * results, int milliSecondsElapsed)
{
    if (_console->ShouldUseColor())
    {
        if (results->GetFailureCount() > 0)
            _console->SetTerminalColor(ConsoleColor::Red);
        else
            _console->SetTerminalColor(ConsoleColor::Green);
    }

    _stream << TestRunSummaryMessage(results, milliSecondsElapsed) << endl;

    if (_console->ShouldUseColor())
        _console->ResetTerminalColor();
}

void ConsoleTestReporter::ReportTestRunOverview(const TestResults * results)
{
    _stream << TestRunOverviewMessage(results) << endl;
}

void ConsoleTestReporter::ReportTestSuiteStart(const std::string & suiteName, int numberOfTestFixtures)
{
    if (_console->ShouldUseColor())
        _console->SetTerminalColor(ConsoleColor::Cyan);

    _stream << TestSuiteSeparator;

    if (_console->ShouldUseColor())
        _console->ResetTerminalColor();

    _stream << " " << TestSuiteStartMessage(suiteName, numberOfTestFixtures) << endl;
}

void ConsoleTestReporter::ReportTestSuiteFinish(const std::string & suiteName,
                                                int numberOfTestFixtures,
                                                int milliSecondsElapsed)
{
    if (_console->ShouldUseColor())
        _console->SetTerminalColor(ConsoleColor::Cyan);

    _stream << TestSuiteSeparator;

    if (_console->ShouldUseColor())
        _console->ResetTerminalColor();

    _stream << " " << TestSuiteFinishMessage(suiteName, numberOfTestFixtures,
                                            milliSecondsElapsed) << endl;
}

void ConsoleTestReporter::ReportTestFixtureStart(const std::string & fixtureName, int numberOfTests)
{
    if (_console->ShouldUseColor())
        _console->SetTerminalColor(ConsoleColor::Yellow);

    _stream << TestFixtureSeparator;

    if (_console->ShouldUseColor())
        _console->ResetTerminalColor();

    _stream << " " << TestFixtureStartMessage(fixtureName, numberOfTests) << endl;
}

void ConsoleTestReporter::ReportTestFixtureFinish(const std::string & fixtureName,
                                                  int numberOfTests,
                                                  int milliSecondsElapsed)
{
    if (_console->ShouldUseColor())
        _console->SetTerminalColor(ConsoleColor::Yellow);

    _stream << TestFixtureSeparator;

    if (_console->ShouldUseColor())
        _console->ResetTerminalColor();

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
    if (_console->ShouldUseColor())
    {
        _console->SetTerminalColor(success ? ConsoleColor::Green : ConsoleColor::Red);
    }

    if (success)
        _stream << TestSuccessSeparator;
    else
        _stream << TestFailSeparator;

    if (_console->ShouldUseColor())
        _console->ResetTerminalColor();

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
