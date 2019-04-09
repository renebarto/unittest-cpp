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

static void FindAndReplaceAll(std::string & str, std::string searchStr, std::string replaceStr)
{
    // Get the first occurrence
    size_t pos = str.find(searchStr);

    // Repeat till end is reached
    while( pos != std::string::npos)
    {
        // Replace this occurrence of Sub String
        str.replace(pos, searchStr.size(), replaceStr);
        // Get the next occurrence from the current position
        pos = str.find(searchStr, pos + replaceStr.size());
    }
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
            // One dot
            // Match on either suite or test, depending on position of any wildcard
            auto posFirstWildcard = filter.find('*');
            if (posFirstWildcard == string::npos)
            {
                // No wildcard, use pattern *.fixture.test
                filterSuite = "*";
                filterFixture = filter.substr(0, posFirstDot);
                filterTest = filter.substr(posFirstDot + 1);
            } else if (posFirstWildcard == 0)
            {
                // Left side wildcard
                filterSuite = "*";
                filterTest = filter.substr(posFirstDot + 1);
                if (posFirstDot == 1)
                {
                    // Pattern *.*.text
                    filterFixture = "*";
                } else
                {
                    // Pattern *.*fixture.test
                    filterFixture = "*" + filter.substr(0, posFirstDot);
                }
            } else if (posFirstWildcard == filter.length() - 1)
            {
                // Right side wildcard
                filterSuite = filter.substr(0, posFirstDot);
                filterTest = "*";
                if (posFirstDot == filter.length() - 2)
                {
                    // Pattern suite.fixture.*
                    filterFixture = "*";
                } else
                {
                    // Pattern suite.fixture*.*
                    filterFixture = filter.substr(posFirstDot + 1) + "*";
                }
            } else
            {
                if (posFirstWildcard < posFirstDot)
                {
                    // Wildcard in middle, use pattern suite*.*.test or suite.*.*test
                    filterSuite = filter.substr(0, posFirstDot);
                    filterFixture = "*";
                    filterTest = filter.substr(posFirstDot + 1);
                }
            }
        }
    } else
    {
        // Match on either suite or test, depending on position of any wildcard
        auto posFirstWildcard = filter.find('*');
        if (posFirstWildcard == string::npos)
        {
            // No asterisk, no match possible
        } else
        {
            isValid = true;
            filterSuite = filter.substr(0, posFirstWildcard) + "*";
            filterFixture = "*";
            filterTest = "*" + filter.substr(posFirstWildcard + 1);
        }
    }
    FindAndReplaceAll(filterSuite, "*", ".*");
    FindAndReplaceAll(filterFixture, "*", ".*");
    FindAndReplaceAll(filterTest, "*", ".*");
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
