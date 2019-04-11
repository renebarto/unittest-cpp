#include <unittest-cpp/TestRunner.h>

#include <unittest-cpp/UnitTestC++.h>

using namespace std;

namespace UnitTestCpp
{

bool InSelection::operator () (const TestInfo * const test) const
{
    return fullName.empty() || (test->_details.suiteName + "." + test->_details.fixtureName + "." + test->_details.testName == fullName);
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

template<char delimiter>
class WordDelimitedBy : public std::string
{
public:
};

template<char delimiter>
std::istream& operator >> (std::istream& is, WordDelimitedBy<delimiter> & output)
{
    std::getline(is, output, delimiter);
    return is;
}

InSelectionFilter::InSelectionFilter(const string & filter)
    : positiveMatchers()
    , negativeMatchers()
{
    std::istringstream issParts(filter);
    std::vector<std::string> parts((std::istream_iterator<WordDelimitedBy<':'>>(issParts)),
                                    std::istream_iterator<WordDelimitedBy<':'>>());
    for (auto & part : parts)
    {
        bool negative {};
        if (part.find("-") == 0)
        {
            negative = true;
            part = part.substr(1);
        }
        std::istringstream issDots(part);
        std::vector<std::string> dottedParts((std::istream_iterator<WordDelimitedBy<'.'>>(issDots)),
                                              std::istream_iterator<WordDelimitedBy<'.'>>());
        string filterSuite;
        string filterFixture;
        string filterTest;
        if (dottedParts.size() == 1)
        {
            filterSuite = "";
            filterFixture = "";
            filterTest = dottedParts[0];
        }
        else if (dottedParts.size() == 2)
        {
            if (((dottedParts[0].length() >= 1) && (dottedParts[0][0] == '*')) ||
                ((dottedParts[1].length() >= 1) && (dottedParts[1][dottedParts[1].length() - 1] == '*')))
            {
                // If only one dot, and starts or ends with wildcard, treat it as a normal pattern
                filterSuite = "";
            } else
            {
                // If only one dot, and not starting with wildcard, treat it as wildcard suite
                filterSuite = "*";
            }
            filterFixture = dottedParts[0];
            filterTest = dottedParts[1];
        }
        else
        {
            filterSuite = dottedParts[0];
            filterFixture = dottedParts[1];
            filterTest = dottedParts[2];
        }
        std::string matchString;
        if (!filterSuite.empty())
            matchString += filterSuite + "\\.";
        if (!filterFixture.empty())
            matchString += filterFixture + "\\.";
        matchString += filterTest;
        FindAndReplaceAll(matchString, "*", ".*");
        if (negative)
            negativeMatchers.emplace_back(matchString);
        else
            positiveMatchers.emplace_back(matchString);
    }
    if (positiveMatchers.empty())
        positiveMatchers.emplace_back(".*");
}

bool InSelectionFilter::operator () (const TestInfo * const test) const
{
    bool positiveMatch {};
    std::string fullName = test->_details.suiteName + "." + test->_details.fixtureName + "." + test->_details.testName;
    for (auto const & matcher : positiveMatchers)
    {
        if (regex_match(fullName, matcher.matcher))
        {
            positiveMatch = true;
            break;
        }
    }
    bool negativeMatch {};
    for (auto const & matcher : negativeMatchers)
    {
        if (regex_match(fullName, matcher.matcher))
        {
            negativeMatch  = true;
            break;
        }
    }
    return positiveMatch && !negativeMatch;
}

int CountSuites(const TestRegistry & registry)
{
    int numberOfTestSuites = 0;
    TestSuiteInfo * testSuite = registry.GetHead();
    while (testSuite)
    {
        ++numberOfTestSuites;
        testSuite = testSuite->next;
    }
    return numberOfTestSuites;
}

int CountFixtures(const TestRegistry & registry)
{
    int numberOfTestFixtures = 0;
    TestSuiteInfo * testSuite = registry.GetHead();
    while (testSuite)
    {
        numberOfTestFixtures += CountFixtures(*testSuite);
        testSuite = testSuite->next;
    }
    return numberOfTestFixtures;
}

int CountTests(const TestRegistry & registry)
{
    int numberOfTests = 0;
    TestSuiteInfo * testSuite = registry.GetHead();
    while (testSuite)
    {
        numberOfTests += CountTests(*testSuite);
        testSuite = testSuite->next;
    }
    return numberOfTests;
}

int CountFixtures(const TestSuiteInfo & testSuite)
{
    int numberOfTestFixtures = 0;
    TestFixtureInfo * testFixture = testSuite.GetHead();
    while (testFixture)
    {
        ++numberOfTestFixtures;
        testFixture = testFixture->next;
    }
    return numberOfTestFixtures;
}

int CountTests(const TestSuiteInfo & testSuite)
{
    int numberOfTests = 0;
    TestFixtureInfo * testFixture = testSuite.GetHead();
    while (testFixture)
    {
        numberOfTests += CountTests(*testFixture);
        testFixture = testFixture->next;
    }
    return numberOfTests;
}

int CountTests(const TestFixtureInfo & testFixture)
{
    int numberOfTests = 0;
    Test * test = testFixture.GetHead();
    while (test)
    {
        ++numberOfTests;
        test = test->_next;
    }
    return numberOfTests;
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
