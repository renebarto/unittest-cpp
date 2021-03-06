#include <unittest-cpp/UnitTestC++.h>
#include "unittest-cpp/ScopedCurrentTest.h"

namespace UnitTestCpp
{

namespace _Test
{

TEST_SUITE(unit_test_cpp)
{

TEST(CanSetandGetDetails)
{
    bool ok = false;
    {
        ScopedCurrentTest scopedTest;

        const UnitTestCpp::TestDetails* details =
            reinterpret_cast<const UnitTestCpp::TestDetails*>(12345);
        UnitTestCpp::CurrentTest::Details() = details;

        ok = (UnitTestCpp::CurrentTest::Details() == details);
    }

    CHECK(ok);
}

TEST(CanSetAndGetResults)
{
    bool ok = false;
    {
        ScopedCurrentTest scopedTest;

        UnitTestCpp::TestResults results;
        UnitTestCpp::CurrentTest::Results() = &results;

        ok = (UnitTestCpp::CurrentTest::Results() == &results);
    }

    CHECK(ok);
}

} // TEST_SUITE(unit_test_cpp)

} // namespace _Test

} // namespace UnitTestCpp
