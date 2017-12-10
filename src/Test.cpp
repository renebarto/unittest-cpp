#include <unittest-c++/Test.h>

#include <unittest-c++/CurrentTest.h>
#include <unittest-c++/ExecuteTest.h>
#include <unittest-c++/TestRegistry.h>

#ifdef UNITTEST_POSIX
#include <unittest-c++/SignalTranslator.h>
#endif

using namespace std;

namespace UnitTestCpp
{

TestRegistry & Test::GetTestRegistry()
{
    static TestRegistry s_registry;
    return s_registry;
}

Test::Test(const string & testName,
           const string & fixtureName,
           const string & suiteName,
           const string & fileName, int lineNumber)
    : TestInfo(ToString(testName),
			   ToString(fixtureName),
               ToString(suiteName),
               ToString(fileName), lineNumber)
    , _next(nullptr)
{
}

Test::~Test()
{
}

void Test::Run(int const UNUSED(maxTestTimeInMs),
               TestResults * testResults)
{
    CurrentTest::Results() = testResults;

    Timer testTimer;
    testTimer.Start();

    testResults->OnTestStart(_details);

    Run();

    testResults->OnTestFinish(_details, testTimer.GetTimeInMilliSeconds());
}

void Test::Run()
{
    ExecuteTest(*this, _details);
}

void Test::RunImpl() const
{
}

} // namespace UnitTestCpp
