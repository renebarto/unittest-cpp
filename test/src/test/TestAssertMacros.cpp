#include <unittest-cpp/UnitTestC++.h>
#include <unittest-cpp/RecordingReporter.h>
#include <unittest-cpp/ScopedCurrentTest.h>

using namespace std;

namespace UnitTestCpp
{

namespace _Test
{

class AssertMacrosTest : public UnitTestCpp::TestFixture
{
};

TEST_SUITE(unit_test_cpp)
{

TEST_FIXTURE(AssertMacrosTest, AssertExpectNull)
{
    void * nullPtr = nullptr;

    EXPECT_NULL(nullPtr);
    ASSERT_NULL(nullPtr);
}

TEST_FIXTURE(AssertMacrosTest, AssertExpectNotNullCharPtr)
{
    const char * charPtr = "ABCD";

    EXPECT_NOT_NULL(charPtr);
    ASSERT_NOT_NULL(charPtr);
}

TEST_FIXTURE(AssertMacrosTest, AssertExpectNotNull)
{
    void * nonNullPtr = reinterpret_cast<void *>(0x12345678);

    EXPECT_NOT_NULL(nonNullPtr);
    ASSERT_NOT_NULL(nonNullPtr);
}

WARNING_PUSH
WARNING_DISABLE(4702)
TEST_FIXTURE(AssertMacrosTest, Fail)
{
    bool testSucceeded = false;
    {
        RecordingReporter reporter;
        UnitTestCpp::TestResults testResults(&reporter);

        ScopedCurrentTest scopedResults(testResults);
        FAIL();

        testSucceeded = (testResults.GetFailureCount() == 1);
    }

    EXPECT_TRUE(testSucceeded);
}
WARNING_POP
} // TEST_SUITE(unit_test_cpp)

} // namespace _Test

} // namespace UnitTestCpp
