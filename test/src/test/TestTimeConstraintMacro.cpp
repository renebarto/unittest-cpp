#include <unittest-cpp/UnitTestC++.h>
#include "unittest-cpp/RecordingReporter.h"
#include "unittest-cpp/ScopedCurrentTest.h"

using namespace std;

namespace UnitTestCpp
{

namespace _Test
{

TEST_SUITE(unit_test_cpp)
{

TEST (TimeConstraintMacroQualifiesNamespace)
{
    // If this compiles without a "using namespace UnitTestCpp;", all is well.
    UNITTEST_TIME_CONSTRAINT(1);
}

TEST (TimeConstraintMacroUsesCorrectInfo)
{
    int testLine = 0;
    RecordingReporter reporter;
    {
        UnitTestCpp::TestResults testResults(&reporter);
        ScopedCurrentTest scopedResults(testResults);

        UNITTEST_TIME_CONSTRAINT(10); testLine = __LINE__;
        UnitTestCpp::TimeHelpers::SleepMilliSeconds(20);
    }

    EXPECT_EQ(1, reporter.testFailedCount);
    EXPECT_TRUE(reporter.lastFailedFile.find(ToString(__FILE__)) != string::npos);
    EXPECT_EQ(testLine, reporter.lastFailedLine);
    EXPECT_TRUE(reporter.lastFailedTest.find("TimeConstraintMacroUsesCorrectInfo") != string::npos);
}

} // TEST_SUITE(unit_test_cpp)

} // namespace _Test

} // namespace UnitTestCpp
