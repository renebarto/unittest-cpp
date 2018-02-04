#include <unittest-cpp/CurrentTest.h>

#include <cstddef>
#include <sstream>
#include <unittest-cpp/PrintValue.h>
#include <unittest-cpp/TestDetails.h>
#include <unittest-cpp/TestResults.h>

namespace UnitTestCpp
{

TestResults * & CurrentTest::Results()
{
    static TestResults * testResults = nullptr;
    return testResults;
}

const TestDetails * & CurrentTest::Details()
{
    static const TestDetails * testDetails = nullptr;
    return testDetails;
}

void CurrentTest::UnhandledException(const std::string & context, int line, const std::exception & e)
{
    std::ostringstream stream;
    stream << std::endl << "  Exception: " << e.what();
    Results()->OnTestFailure(TestDetails(*Details(), line),
                             "Unhandled exception in " + context + ": " + stream.str());
}

void CurrentTest::UnhandledException(const std::string & context, int line)
{
    Results()->OnTestFailure(TestDetails(*Details(), line),
                             "Unhandled exception in " + context);
}

} // namespace UnitTestCpp
