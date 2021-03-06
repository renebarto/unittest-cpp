#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unittest-cpp/exports.h>
#include <unittest-cpp/ITestReporter.h>
#include <unittest-cpp/TestDetailedResult.h>

namespace UnitTestCpp {

class Console;

class UNIT_TEST_CPP_EXPORT ConsoleGoogleTestReporter
    : public ITestReporter
{
public:
    ConsoleGoogleTestReporter(bool useColor);

private:
    virtual void ReportTestRunStart(int numberOfTestSuites,
                                    int numberOfTestFixtures,
                                    int numberOfTests) override;

    virtual void ReportTestRunFinish(int numberOfTestSuites,
                                     int numberOfTestFixtures,
                                     int numberOfTests,
                                     int milliSecondsElapsed) override;

    virtual void ReportTestRunSummary(const TestResults * results, int milliSecondsElapsed) override;

    virtual void ReportTestRunOverview(const TestResults * results) override;

    virtual void ReportTestSuiteStart(const std::string & suiteName,
                                      int numberOfTestFixtures) override;

    virtual void ReportTestSuiteFinish(const std::string & suiteName,
                                       int numberOfTestFixtures,
                                       int milliSecondsElapsed) override;

    virtual void ReportTestFixtureStart(const std::string & fixtureName,
                                        int numberOfTests) override;

    virtual void ReportTestFixtureFinish(const std::string & fixtureName,
                                         int numberOfTests,
                                         int milliSecondsElapsed) override;

    virtual void ReportTestStart(const TestDetails & test) override;

    virtual void ReportTestFinish(const TestDetails & test, bool success,
                                  int milliSecondsElapsed) override;

    virtual void ReportTestFailure(const TestDetails & test, const std::string & failure) override;

    virtual void ReportTestSuiteEntry(const std::string & suiteName) override;
    virtual void ReportTestFixtureEntry(const std::string & fixtureName) override;
    virtual void ReportTestEntry(const std::string & testName) override;

    using ResultList = std::vector<TestDetailedResult>;
    ResultList & Results()
    {
        return _results;
    }

private:
    std::shared_ptr<Console> _console;
    ResultList _results;
};

} // namespace UnitTestCpp
