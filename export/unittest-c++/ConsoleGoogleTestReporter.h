#pragma once

#include <string>
#include <vector>
#include <unittest-c++/Console.h>
#include <unittest-c++/exports.h>
#include <unittest-c++/ITestReporter.h>
#include <unittest-c++/TestDetailedResult.h>

namespace UnitTestCpp {

class UNIT_TEST_CPP_EXPORT ConsoleGoogleTestReporter
    : public ITestReporter
{
public:
    ConsoleGoogleTestReporter()
        : _console(std::cout)
        , _results()
    {}

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

    using ResultList = std::vector<TestDetailedResult>;
    ResultList & Results()
    {
        return _results;
    }

private:
    Console _console;
    ResultList _results;
};

} // namespace UnitTestCpp
