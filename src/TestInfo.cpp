#include <unittest-cpp/TestInfo.h>

using namespace std;

namespace UnitTestCpp
{

TestInfo::TestInfo(const std::string & testName,
                   const std::string & fixtureName,
                   const std::string & suiteName,
                   const std::string & fileName, int lineNumber)
    : _details(testName, fixtureName, suiteName, fileName, lineNumber)
{
}

TestInfo::~TestInfo()
{
}

} // namespace UnitTestCpp
