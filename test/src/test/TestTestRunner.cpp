#include <unittest-cpp/UnitTestC++.h>
#include "unittest-cpp/ScopedCurrentTest.h"

namespace UnitTestCpp
{

namespace _Test
{

TEST_SUITE(unit_test_cpp)
{

class PredicateTest
    : public UnitTestCpp::TestFixture
{
public:
    PredicateTest()
        : suiteInfo("suite")
        , fixtureInfo("fixture")
        , testInfo("test", "fixture", "suite")
        , otherSuiteInfo("othersuite")
        , otherFixtureInfo("otherfixture")
        , otherTestInfo("othertest", "otherfixture", "othersuite")
        , dummySInfo("dummyS")
        , dummyFInfo("dummyF")
        , dummyTInfo("dummyS", "dummyF", "dummyT")
    {
    }
    TestSuiteInfo suiteInfo;
    TestFixtureInfo fixtureInfo;
    TestInfo testInfo;
    TestSuiteInfo otherSuiteInfo;
    TestFixtureInfo otherFixtureInfo;
    TestInfo otherTestInfo;
    TestSuiteInfo dummySInfo;
    TestFixtureInfo dummyFInfo;
    TestInfo dummyTInfo;
};

TEST_FIXTURE(PredicateTest, TrueTest)
{
    True nonFiltering;

    EXPECT_TRUE(nonFiltering.operator()(&suiteInfo));
    EXPECT_TRUE(nonFiltering.operator()(&fixtureInfo));
    EXPECT_TRUE(nonFiltering.operator()(&testInfo));
    EXPECT_TRUE(nonFiltering.operator()(&otherSuiteInfo));
    EXPECT_TRUE(nonFiltering.operator()(&otherFixtureInfo));
    EXPECT_TRUE(nonFiltering.operator()(&otherTestInfo));
    EXPECT_TRUE(nonFiltering.operator()(&dummySInfo));
    EXPECT_TRUE(nonFiltering.operator()(&dummyFInfo));
    EXPECT_TRUE(nonFiltering.operator()(&dummyTInfo));
}

TEST_FIXTURE(PredicateTest, InSelectionTest)
{
    InSelection selection(suiteInfo.Name().c_str(), fixtureInfo.Name().c_str(), testInfo._details.testName.c_str());

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_FALSE(selection.operator()(&otherSuiteInfo));
    EXPECT_FALSE(selection.operator()(&otherFixtureInfo));
    EXPECT_FALSE(selection.operator()(&otherTestInfo));
    EXPECT_FALSE(selection.operator()(&dummySInfo));
    EXPECT_FALSE(selection.operator()(&dummyFInfo));
    EXPECT_FALSE(selection.operator()(&dummyTInfo));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_SuiteDotFixtureDotTest)
{
    std::string filter = suiteInfo.Name() + "." + fixtureInfo.Name() + "." + testInfo._details.testName;
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_FALSE(selection.operator()(&otherSuiteInfo));
    EXPECT_FALSE(selection.operator()(&otherFixtureInfo));
    EXPECT_FALSE(selection.operator()(&otherTestInfo));
    EXPECT_FALSE(selection.operator()(&dummySInfo));
    EXPECT_FALSE(selection.operator()(&dummyFInfo));
    EXPECT_FALSE(selection.operator()(&dummyTInfo));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_FixtureDotTest)
{
    std::string filter = fixtureInfo.Name() + "." + testInfo._details.testName;
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_TRUE(selection.operator()(&otherSuiteInfo));
    EXPECT_FALSE(selection.operator()(&otherFixtureInfo));
    EXPECT_FALSE(selection.operator()(&otherTestInfo));
    EXPECT_TRUE(selection.operator()(&dummySInfo));
    EXPECT_FALSE(selection.operator()(&dummyFInfo));
    EXPECT_FALSE(selection.operator()(&dummyTInfo));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_Wildcard)
{
    std::string filter = "*";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_TRUE(selection.operator()(&otherSuiteInfo));
    EXPECT_TRUE(selection.operator()(&otherFixtureInfo));
    EXPECT_TRUE(selection.operator()(&otherTestInfo));
    EXPECT_TRUE(selection.operator()(&dummySInfo));
    EXPECT_TRUE(selection.operator()(&dummyFInfo));
    EXPECT_TRUE(selection.operator()(&dummyTInfo));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_SuiteDotFixtureDotWildcard)
{
    std::string filter = suiteInfo.Name() + "." + fixtureInfo.Name() + ".*";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_FALSE(selection.operator()(&otherSuiteInfo));
    EXPECT_FALSE(selection.operator()(&otherFixtureInfo));
    EXPECT_TRUE(selection.operator()(&otherTestInfo));
    EXPECT_FALSE(selection.operator()(&dummySInfo));
    EXPECT_FALSE(selection.operator()(&dummyFInfo));
    EXPECT_TRUE(selection.operator()(&dummyTInfo));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_FixtureDotWildcard)
{
    std::string filter = suiteInfo.Name() + ".*";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_FALSE(selection.operator()(&otherSuiteInfo));
    EXPECT_TRUE(selection.operator()(&otherFixtureInfo));
    EXPECT_TRUE(selection.operator()(&otherTestInfo));
    EXPECT_FALSE(selection.operator()(&dummySInfo));
    EXPECT_TRUE(selection.operator()(&dummyFInfo));
    EXPECT_TRUE(selection.operator()(&dummyTInfo));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_WildcardDotFixtureDotTest)
{
    std::string filter = "*." + fixtureInfo.Name() + "." + testInfo._details.testName;
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_TRUE(selection.operator()(&otherSuiteInfo));
    EXPECT_FALSE(selection.operator()(&otherFixtureInfo));
    EXPECT_FALSE(selection.operator()(&otherTestInfo));
    EXPECT_TRUE(selection.operator()(&dummySInfo));
    EXPECT_FALSE(selection.operator()(&dummyFInfo));
    EXPECT_FALSE(selection.operator()(&dummyTInfo));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_WildcardDotTest)
{
    std::string filter = "*." + testInfo._details.testName;
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_TRUE(selection.operator()(&otherSuiteInfo));
    EXPECT_TRUE(selection.operator()(&otherFixtureInfo));
    EXPECT_FALSE(selection.operator()(&otherTestInfo));
    EXPECT_TRUE(selection.operator()(&dummySInfo));
    EXPECT_TRUE(selection.operator()(&dummyFInfo));
    EXPECT_FALSE(selection.operator()(&dummyTInfo));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_SuiteDotPartialFixtureWildcardDotWildcard)
{
    std::string filter = suiteInfo.Name() + "." + fixtureInfo.Name() + "*.*";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_FALSE(selection.operator()(&otherSuiteInfo));
    EXPECT_FALSE(selection.operator()(&otherFixtureInfo));
    EXPECT_TRUE(selection.operator()(&otherTestInfo));
    EXPECT_FALSE(selection.operator()(&dummySInfo));
    EXPECT_FALSE(selection.operator()(&dummyFInfo));
    EXPECT_TRUE(selection.operator()(&dummyTInfo));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_PartialSuiteWildcardDotWildcard)
{
    std::string filter = suiteInfo.Name() + "*.*";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_FALSE(selection.operator()(&otherSuiteInfo));
    EXPECT_TRUE(selection.operator()(&otherFixtureInfo));
    EXPECT_TRUE(selection.operator()(&otherTestInfo));
    EXPECT_FALSE(selection.operator()(&dummySInfo));
    EXPECT_TRUE(selection.operator()(&dummyFInfo));
    EXPECT_TRUE(selection.operator()(&dummyTInfo));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_WildcardDotWildcardPartialFixtureDotTest)
{
    std::string filter = "*.*" + fixtureInfo.Name() + "." + testInfo._details.testName;
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_TRUE(selection.operator()(&otherSuiteInfo));
    EXPECT_TRUE(selection.operator()(&otherFixtureInfo));
    EXPECT_FALSE(selection.operator()(&otherTestInfo));
    EXPECT_TRUE(selection.operator()(&dummySInfo));
    EXPECT_FALSE(selection.operator()(&dummyFInfo));
    EXPECT_FALSE(selection.operator()(&dummyTInfo));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_WildcardDotWildcardPartialTest)
{
    std::string filter = "*.*" + testInfo._details.testName;
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_TRUE(selection.operator()(&otherSuiteInfo));
    EXPECT_TRUE(selection.operator()(&otherFixtureInfo));
    EXPECT_TRUE(selection.operator()(&otherTestInfo));
    EXPECT_TRUE(selection.operator()(&dummySInfo));
    EXPECT_TRUE(selection.operator()(&dummyFInfo));
    EXPECT_FALSE(selection.operator()(&dummyTInfo));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_WildcardPartialSuiteDotFixtureDotTest)
{
    std::string filter = "*suite." + fixtureInfo.Name() + "." + testInfo._details.testName;
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_TRUE(selection.operator()(&otherSuiteInfo));
    EXPECT_FALSE(selection.operator()(&otherFixtureInfo));
    EXPECT_FALSE(selection.operator()(&otherTestInfo));
    EXPECT_FALSE(selection.operator()(&dummySInfo));
    EXPECT_FALSE(selection.operator()(&dummyFInfo));
    EXPECT_FALSE(selection.operator()(&dummyTInfo));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_WildcardPartialFixtureDotTest)
{
    std::string filter = "*fixture." + testInfo._details.testName;
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_TRUE(selection.operator()(&otherSuiteInfo));
    EXPECT_TRUE(selection.operator()(&otherFixtureInfo));
    EXPECT_FALSE(selection.operator()(&otherTestInfo));
    EXPECT_TRUE(selection.operator()(&dummySInfo));
    EXPECT_FALSE(selection.operator()(&dummyFInfo));
    EXPECT_FALSE(selection.operator()(&dummyTInfo));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_WildcardPartialTest)
{
    std::string filter = "*test";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_TRUE(selection.operator()(&otherSuiteInfo));
    EXPECT_TRUE(selection.operator()(&otherFixtureInfo));
    EXPECT_TRUE(selection.operator()(&otherTestInfo));
    EXPECT_TRUE(selection.operator()(&dummySInfo));
    EXPECT_TRUE(selection.operator()(&dummyFInfo));
    EXPECT_FALSE(selection.operator()(&dummyTInfo));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_SuiteDotFixtureDotPartialTestWildcard)
{
    std::string filter = suiteInfo.Name() + "." + fixtureInfo.Name() + ".test*";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_FALSE(selection.operator()(&otherSuiteInfo));
    EXPECT_FALSE(selection.operator()(&otherFixtureInfo));
    EXPECT_FALSE(selection.operator()(&otherTestInfo));
    EXPECT_FALSE(selection.operator()(&dummySInfo));
    EXPECT_FALSE(selection.operator()(&dummyFInfo));
    EXPECT_FALSE(selection.operator()(&dummyTInfo));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_SuiteDotPartialFixtureWildcard)
{
    std::string filter = suiteInfo.Name() + ".fixture*";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_FALSE(selection.operator()(&otherSuiteInfo));
    EXPECT_FALSE(selection.operator()(&otherFixtureInfo));
    EXPECT_TRUE(selection.operator()(&otherTestInfo));
    EXPECT_FALSE(selection.operator()(&dummySInfo));
    EXPECT_FALSE(selection.operator()(&dummyFInfo));
    EXPECT_TRUE(selection.operator()(&dummyTInfo));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_PartialSuiteWildcard)
{
    std::string filter = "suite*";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_FALSE(selection.operator()(&otherSuiteInfo));
    EXPECT_TRUE(selection.operator()(&otherFixtureInfo));
    EXPECT_TRUE(selection.operator()(&otherTestInfo));
    EXPECT_FALSE(selection.operator()(&dummySInfo));
    EXPECT_TRUE(selection.operator()(&dummyFInfo));
    EXPECT_TRUE(selection.operator()(&dummyTInfo));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_WildcardDotPartialTestWildcard)
{
    std::string filter = "*.test*";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_TRUE(selection.operator()(&otherSuiteInfo));
    EXPECT_TRUE(selection.operator()(&otherFixtureInfo));
    EXPECT_FALSE(selection.operator()(&otherTestInfo));
    EXPECT_TRUE(selection.operator()(&dummySInfo));
    EXPECT_TRUE(selection.operator()(&dummyFInfo));
    EXPECT_FALSE(selection.operator()(&dummyTInfo));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_WildcardPartialFixtureDotWildcard)
{
    std::string filter = "*fixture.*";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_TRUE(selection.operator()(&otherSuiteInfo));
    EXPECT_TRUE(selection.operator()(&otherFixtureInfo));
    EXPECT_TRUE(selection.operator()(&otherTestInfo));
    EXPECT_TRUE(selection.operator()(&dummySInfo));
    EXPECT_FALSE(selection.operator()(&dummyFInfo));
    EXPECT_TRUE(selection.operator()(&dummyTInfo));
}

} // TEST_SUITE(unit_test_cpp)

} // namespace _Test

} // namespace UnitTestCpp
