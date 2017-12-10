#include <unittest-c++/TestDetails.h>

using namespace std;

namespace UnitTestCpp
{

TestDetails::TestDetails(const std::string & testName,
                         const std::string & fixtureName,
                         const std::string & suiteName,
                         const std::string & fileName, int lineNumber)
    : suiteName(suiteName)
    , fixtureName(fixtureName)
    , testName(testName)
    , fileName(fileName)
    , lineNumber(lineNumber)
{
}

TestDetails::TestDetails(const TestDetails & details, int lineNumber)
    : suiteName(details.suiteName)
    , fixtureName(details.fixtureName)
    , testName(details.testName)
    , fileName(details.fileName)
    , lineNumber(lineNumber)
{
}

} // namespace UnitTestCpp
