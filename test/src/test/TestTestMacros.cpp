#include <unittest-cpp/UnitTestC++.h>
#include "unittest-cpp/ReportAssert.h"
#include "unittest-cpp/RecordingReporter.h"
#include "unittest-cpp/ScopedCurrentTest.h"

using namespace std;

namespace UnitTestCpp
{

namespace _Test
{

TEST_SUITE(unit_test_cpp)
{

TestRegistry list1;
TEST_EX(DummyTest, list1)
{
}

TEST(TestsAreAddedToTheListThroughMacro)
{
    CHECK(list1.GetHead() != 0);
    CHECK(list1.GetHead()->next == 0);
}

struct ThrowingThingie : public TestFixture
{
    ThrowingThingie() : dummy(false)
    {
        if (!dummy)
            throw "Oops";
    }

    bool dummy;
};

TestRegistry list2;
TEST_FIXTURE_EX(ThrowingThingie, DummyTestName, list2)
{
}

TEST (ExceptionsInFixtureAreReportedAsHappeningInTheFixture)
{
    RecordingReporter reporter;
    TestResults result(&reporter);
    {
        ScopedCurrentTest scopedResults(result);
        RunIf(*list2.GetHead(), True(), 0, &result);
    }

    EXPECT_TRUE(reporter.lastFailedMessage.find("Unhandled exception") != string::npos);
    EXPECT_TRUE(reporter.lastFailedMessage.find("fixture") != string::npos);
    EXPECT_TRUE(reporter.lastFailedMessage.find("ThrowingThingie") != string::npos);
}

struct DummyFixture : public TestFixture
{
    int x;
};

// We're really testing the macros so we just want them to compile and link
TEST_SUITE(TestSuite1)
{
    TEST(SimilarlyNamedTestsInDifferentSuitesWork)
    {
    }

    TEST_FIXTURE(DummyFixture, SimilarlyNamedFixtureTestsInDifferentSuitesWork)
    {
    }
}

TEST_SUITE(TestSuite2)
{
    TEST(SimilarlyNamedTestsInDifferentSuitesWork)
    {
    }

    TEST_FIXTURE(DummyFixture,SimilarlyNamedFixtureTestsInDifferentSuitesWork)
    {
    }
}

TestRegistry macroTestRegistry1;
TEST_EX(MacroTestHelper1, macroTestRegistry1)
{
}

TEST(TestAddedWithTEST_EXMacroGetsDefaultSuite)
{
    CHECK(macroTestRegistry1.GetHead() != 0);
    CHECK(macroTestRegistry1.GetHead()->GetHead() != 0);
    CHECK(macroTestRegistry1.GetHead()->GetHead()->GetHead() != 0);
    ASSERT_EQ("MacroTestHelper1", macroTestRegistry1.GetHead()->GetHead()->GetHead()->_details.testName);
    ASSERT_EQ("", macroTestRegistry1.GetHead()->GetHead()->GetHead()->_details.fixtureName);
    ASSERT_EQ("unit_test_cpp", macroTestRegistry1.GetHead()->GetHead()->GetHead()->_details.suiteName);
}

TestRegistry macroTestRegistry2;
TEST_FIXTURE_EX(DummyFixture, MacroTestHelper2, macroTestRegistry2)
{
}

TEST(TestAddedWithTEST_FIXTURE_EXMacroGetsDefaultSuite)
{
    CHECK(macroTestRegistry2.GetHead() != 0);
    CHECK(macroTestRegistry2.GetHead()->GetHead() != 0);
    CHECK(macroTestRegistry2.GetHead()->GetHead()->GetHead() != 0);
    ASSERT_EQ("MacroTestHelper2", macroTestRegistry2.GetHead()->GetHead()->GetHead()->_details.testName);
    ASSERT_EQ("DummyFixture", macroTestRegistry2.GetHead()->GetHead()->GetHead()->_details.fixtureName);
    ASSERT_EQ("unit_test_cpp", macroTestRegistry2.GetHead()->GetHead()->GetHead()->_details.suiteName);
}

struct FixtureCtorThrows : public TestFixture
{
WARNING_PUSH
WARNING_DISABLE(4702)
    FixtureCtorThrows()    { throw "exception"; }
WARNING_POP
};

TestRegistry throwingFixtureTestRegistry1;
TEST_FIXTURE_EX(FixtureCtorThrows, FixtureCtorThrowsTestName, throwingFixtureTestRegistry1)
{
}

TEST(FixturesWithThrowingCtorsAreFailures)
{
    CHECK(throwingFixtureTestRegistry1.GetHead() != 0);
    RecordingReporter reporter;
    TestResults result(&reporter);
    {
        ScopedCurrentTest scopedResult(result);
        RunIf(*throwingFixtureTestRegistry1.GetHead(), True(), 0, &result);
    }

    int const failureCount = result.GetFailedTestCount();
    ASSERT_EQ(1, failureCount);
    ASSERT_TRUE(reporter.lastFailedMessage.find("while constructing fixture") != string::npos);
}

struct FixtureDtorThrows : public TestFixture
{
    ~FixtureDtorThrows() noexcept(false)
    {
        WARNING_DISABLE(4297)
        // cppcheck-suppress exceptThrowInDestructor
        throw "exception";
        WARNING_DEFAULT(4297)
    }
};

TestRegistry throwingFixtureTestRegistry2;
TEST_FIXTURE_EX(FixtureDtorThrows, FixtureDtorThrowsTestName, throwingFixtureTestRegistry2)
{
}

// For some reason Ubuntu from 14.04 onwards does not accept throwing desctructors
// a throwing destructor always crashes the application without throwing exceptions
//TEST(FixturesWithThrowingDtorsAreFailures)
//{
//    CHECK(throwingFixtureTestRegistry2.GetHead() != 0);
//
//    RecordingReporter reporter;
//    TestResults result(&reporter);
//    {
//        ScopedCurrentTest scopedResult(result);
//        throwingFixtureTestRegistry2.GetHead()->RunIf(True(), 0, &result);
//    }
//
//    int const failureCount = result.GetFailedTestCount();
//    ASSERT_EQ(1, failureCount);
//    ASSERT_TRUE(reporter.lastFailedMessage.find("while destroying fixture") != string::npos);
//}

const int FailingLine = 123;

struct FixtureCtorAsserts : public TestFixture
{
    FixtureCtorAsserts()
    {
        UnitTestCpp::ReportAssert("assert failure", "file", FailingLine);
    }
};

TestRegistry ctorAssertFixtureTestRegistry;
TEST_FIXTURE_EX(FixtureCtorAsserts, CorrectlyReportsAssertFailureInCtor, ctorAssertFixtureTestRegistry)
{
}

TEST(CorrectlyReportsFixturesWithCtorsThatAssert)
{
    RecordingReporter reporter;
    TestResults result(&reporter);
    {
        ScopedCurrentTest scopedResults(result);
        RunIf(*ctorAssertFixtureTestRegistry.GetHead(), True(), 0, &result);
    }

    const int failureCount = result.GetFailedTestCount();
    ASSERT_EQ(1, failureCount);
    ASSERT_EQ(FailingLine, reporter.lastFailedLine);
    ASSERT_TRUE(reporter.lastFailedMessage.find("assert failure") != string::npos);
}

// We're really testing if it's possible to use the same suite in two files
// to compile and link successfuly (TestTestSuite.cpp has suite with the same name)
// Note: we are outside of the anonymous namespace
TEST_SUITE(SameTestSuite)
{
    TEST(DummyTest1)
    {
    }
}

#define CUR_TEST_NAME CurrentTestDetailsContainCurrentTestInfo
#define INNER_STRINGIFY(X) #X
#define STRINGIFY(X) INNER_STRINGIFY(X)

TEST(CUR_TEST_NAME)
{
    const UnitTestCpp::TestDetails* details = CurrentTest::Details();
    ASSERT_EQ(ToString(STRINGIFY(CUR_TEST_NAME)), details->testName);
}

#undef CUR_TEST_NAME
#undef INNER_STRINGIFY
#undef STRINGIFY

} // TEST_SUITE(unit_test_cpp)

} // namespace _Test

} // namespace UnitTestCpp
