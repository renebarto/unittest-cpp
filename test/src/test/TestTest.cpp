#include <unittest-cpp/UnitTestC++.h>
#include "unittest-cpp/ScopedCurrentTest.h"

namespace UnitTestCpp
{

namespace _Test
{

TEST_SUITE(unit_test_cpp)
{

TEST(PassingTestHasNoFailures)
{
    class PassingTest : public Test
    {
    public:
        PassingTest() : Test("passing") {}
        virtual void RunImpl() const
        {
            CHECK(true);
        }
    };

    TestResults results;
    {
        ScopedCurrentTest scopedResults(results);
        PassingTest().Run();
    }

    ASSERT_EQ(0, results.GetFailureCount());
}


TEST(FailingTestHasFailures)
{
    class FailingTest : public Test
    {
    public:
        FailingTest() : Test("failing") {}
        virtual void RunImpl() const
        {
            CHECK(false);
        }
    };

    TestResults results;
    {
        ScopedCurrentTest scopedResults(results);
        FailingTest().Run();
    }

    ASSERT_EQ(1, results.GetFailureCount());
}

TEST(ExpectContinuesAfterFailure)
{
    class FailingTest : public Test
    {
    public:
        FailingTest() : Test("failing") {}
        virtual void RunImpl() const
        {
            EXPECT_TRUE(false);
            EXPECT_TRUE(false);
        }
    };

    TestResults results;
    {
        ScopedCurrentTest scopedResults(results);
        FailingTest().Run();
    }

    ASSERT_EQ(2, results.GetFailureCount());
}

TEST(AssertTrueStopsAfterFailure)
{
    class FailingTest : public Test
    {
    public:
        FailingTest() : Test("failing") {}
        virtual void RunImpl() const
        {
            ASSERT_TRUE(false);
            ASSERT_TRUE(false);
        }
    };

    TestResults results;
    {
        ScopedCurrentTest scopedResults(results);
        FailingTest().Run();
    }

    ASSERT_EQ(1, results.GetFailureCount());
}

TEST(AssertFalseStopsAfterFailure)
{
    class FailingTest : public Test
    {
    public:
        FailingTest() : Test("failing") {}
        virtual void RunImpl() const
        {
            ASSERT_FALSE(true);
            ASSERT_FALSE(true);
        }
    };

    TestResults results;
    {
        ScopedCurrentTest scopedResults(results);
        FailingTest().Run();
    }

    ASSERT_EQ(1, results.GetFailureCount());
}

TEST(AssertEqualsStopsAfterFailure)
{
    class FailingTest : public Test
    {
    public:
        FailingTest() : Test("failing") {}
        virtual void RunImpl() const
        {
            ASSERT_EQ(1, 2);
            ASSERT_EQ(1, 2);
        }
    };

    TestResults results;
    {
        ScopedCurrentTest scopedResults(results);
        FailingTest().Run();
    }

    ASSERT_EQ(1, results.GetFailureCount());
}

TEST(AssertNotEqualsStopsAfterFailure)
{
    class FailingTest : public Test
    {
    public:
        FailingTest() : Test("failing") {}
        virtual void RunImpl() const
        {
            ASSERT_NE(1, 1);
            ASSERT_NE(1, 1);
        }
    };

    TestResults results;
    {
        ScopedCurrentTest scopedResults(results);
        FailingTest().Run();
    }

    ASSERT_EQ(1, results.GetFailureCount());
}

TEST(AssertEqualsIgnoreCaseStopsAfterFailure)
{
    class FailingTest : public Test
    {
    public:
        FailingTest() : Test("failing") {}
        virtual void RunImpl() const
        {
            ASSERT_EQ_IGNORE_CASE("A", "B");
            ASSERT_EQ_IGNORE_CASE("A", "B");
        }
    };

    TestResults results;
    {
        ScopedCurrentTest scopedResults(results);
        FailingTest().Run();
    }

    ASSERT_EQ(1, results.GetFailureCount());
}

TEST(AssertNotEqualsIgnoreCaseStopsAfterFailure)
{
    class FailingTest : public Test
    {
    public:
        FailingTest() : Test("failing") {}
        virtual void RunImpl() const
        {
            ASSERT_NE_IGNORE_CASE("A", "A");
            ASSERT_NE_IGNORE_CASE("A", "A");
        }
    };

    TestResults results;
    {
        ScopedCurrentTest scopedResults(results);
        FailingTest().Run();
    }

    ASSERT_EQ(1, results.GetFailureCount());
}

TEST(AssertNearStopsAfterFailure)
{
    class FailingTest : public Test
    {
    public:
        FailingTest() : Test("failing") {}
        virtual void RunImpl() const
        {
            ASSERT_NEAR(1, 2, 0.01);
            ASSERT_NEAR(1, 2, 0.01);
        }
    };

    TestResults results;
    {
        ScopedCurrentTest scopedResults(results);
        FailingTest().Run();
    }

    ASSERT_EQ(1, results.GetFailureCount());
}

WARNING_PUSH
WARNING_DISABLE(4702)

TEST(AssertThrowStopsAfterFailure)
{
    class FailingTest : public Test
    {
    public:
        FailingTest() : Test("failing") {}
        bool True() const { return true; }
        virtual void RunImpl() const
        {
            ASSERT_THROW(True(), std::bad_exception);
            ASSERT_TRUE(false);
        }
    };

    TestResults results;
    {
        ScopedCurrentTest scopedResults(results);
        FailingTest().Run();
    }

    ASSERT_EQ(1, results.GetFailureCount());
}

TEST(AssertNoThrowStopsAfterFailure)
{
    class FailingTest : public Test
    {
    public:
        FailingTest() : Test("failing") {}
        void ThrowingFunction() const { throw 1; }
        virtual void RunImpl() const
        {
            ASSERT_NOTHROW(ThrowingFunction());
            ASSERT_TRUE(false);
        }
    };

    TestResults results;
    {
        ScopedCurrentTest scopedResults(results);
        FailingTest().Run();
    }

    ASSERT_EQ(1, results.GetFailureCount());
}

WARNING_POP

TEST(ThrowingTestsAreReportedAsFailures)
{
    class CrashingTest : public Test
    {
    public:
        CrashingTest() : Test("throwing") {}
        virtual void RunImpl() const
        {
            throw "Blah";
        }
    };

    TestResults results;
    {
        ScopedCurrentTest scopedResult(results);
        CrashingTest().Run();
    }

    ASSERT_EQ(1, results.GetFailureCount());
}


#if defined(LINUX) || defined(DARWIN)
TEST(CrashingTestsAreReportedAsFailures)
{
    class CrashingTest : public Test
    {
    public:
        CrashingTest() : Test("crashing") {}
        virtual void RunImpl() const
        {
            // cppcheck-suppress constStatement
            reinterpret_cast< void (*)() >(0)();
        }
    };

    TestResults results;
    {
        ScopedCurrentTest scopedResult(results);
        CrashingTest().Run();
    }

    ASSERT_EQ(1, results.GetFailureCount());
}
#endif

TEST(TestWithUnspecifiedFixtureAndSuiteGetsDefaultFixtureAndSuite)
{
    Test test("test");
    ASSERT_EQ("", test._details.fixtureName);
    ASSERT_EQ("", test._details.suiteName);
}

TEST(TestReflectsSpecifiedFixtureName)
{
    Test test("test", "testFixture");
    ASSERT_EQ("testFixture", test._details.fixtureName);
    ASSERT_EQ("", test._details.suiteName);
}

TEST(TestReflectsSpecifiedFixtureAndSuiteName)
{
    Test test("test", "testFixture", "testSuite");
    ASSERT_EQ("testFixture", test._details.fixtureName);
    ASSERT_EQ("testSuite", test._details.suiteName);
}

void Fail()
{
    CHECK(false);
}

TEST(OutOfCoreCheckMacrosCanFailTests)
{
    TestResults results;
    {
        ScopedCurrentTest scopedResult(results);
        Fail();
    }

    ASSERT_EQ(1, results.GetFailureCount());
}

} // TEST_SUITE(unit_test_cpp)

} // namespace _Test

} // namespace UnitTestCpp
