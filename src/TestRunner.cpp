#include <unittest-cpp/TestRunner.h>

#include <regex>
#include <unittest-cpp/UnitTestC++.h>

using namespace std;

namespace UnitTestCpp
{

bool InSelection::operator () (const TestInfo * const test) const
{
    return !testName || (test->_details.testName == testName);
}

bool InSelection::operator () (const TestFixtureInfo * const fixture) const
{
    return !fixtureName || (fixture->Name() == fixtureName);
}

bool InSelection::operator () (const TestSuiteInfo * const suite) const
{
    return !suiteName || (suite->Name() == suiteName);
}

InSelectionFilter::InSelectionFilter(const string & filter)
    : matcherSuite()
    , matcherFixture()
    , matcherTest()
    , isValid()
{
    string filterSuite;
    string filterFixture;
    string filterTest;
    auto posFirstWildcard = filter.find('*');
    if (posFirstWildcard == string::npos)
    {
        auto posFirstDot = filter.find('.');
        if (posFirstDot != string::npos)
        {
            isValid = true;
            auto posSecondDot = filter.find('.', posFirstDot + 1);
            if (posSecondDot != string::npos)
            {
                auto posMoreDots = filter.find('.', posSecondDot + 1);
                if (posMoreDots == string::npos) {
                    // Two dots, means we have suite.fixture.test specification
                    filterSuite = filter.substr(0, posFirstDot);
                    filterFixture = filter.substr(posFirstDot + 1, posSecondDot - posFirstDot - 1);
                    filterTest = filter.substr(posSecondDot + 1);
                } else
                {
                    // Too many dots, invalid
                }
            } else
            {
                // One dot, means we have fixture.test specification, set suite to wildcard
                filterSuite = ".*";
                filterFixture = filter.substr(0, posFirstDot);
                filterTest = filter.substr(posFirstDot + 1);
            }
        } else
        {
            // Do nothing, no match possible
        }
    } else
    {
        auto posFirstDot = filter.find('.');
        if (posFirstDot != string::npos)
        {

            isValid = true;
            auto posSecondDot = filter.find('.', posFirstDot + 1);
            if (posSecondDot != string::npos)
            {

            }
        }
        auto posSecondWildcard = filter.find('*', posFirstWildcard + 1);
        string firstLiteralPart = filter.substr(0, posFirstWildcard);

    }
    matcherSuite = regex(filterSuite);
    matcherFixture = regex(filterFixture);
    matcherTest = regex(filterTest);
}

bool InSelectionFilter::operator () (const TestInfo * const test) const
{
    return regex_match(test->_details.testName, matcherTest);
}

bool InSelectionFilter::operator () (const TestFixtureInfo * const fixture) const
{
    return regex_match(fixture->Name(), matcherFixture);
}

bool InSelectionFilter::operator () (const TestSuiteInfo * const suite) const
{
    return regex_match(suite->Name(), matcherSuite);
}

bool InSelectionFilter::IsValid() const
{
    return isValid;
}

int RunAllTests()
{
    return RunSelectedTests(True());
}

int RunAllTests(ITestReporter & reporter)
{
    return RunSelectedTests(reporter, True());
}

void ListAllTests()
{
    ListSelectedTests(True());
}

void ListAllTests(ITestReporter & reporter)
{
    ListSelectedTests(reporter, True());
}

TestRunner::TestRunner(ITestReporter & reporter)
    : reporter(&reporter)
    , testResults(new TestResults(&reporter))
    , timer(new Timer())
{
}

TestRunner::~TestRunner()
{
    delete testResults;
    delete timer;
}

} // namespace UnitTestCpp
