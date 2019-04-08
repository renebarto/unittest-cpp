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
        , suiteInfo2("othersuite")
        , fixtureInfo2("otherfixture")
        , testInfo2("othertest", "otherfixture", "othersuite")
    {
    }
    TestSuiteInfo suiteInfo;
    TestFixtureInfo fixtureInfo;
    TestInfo testInfo;
    TestSuiteInfo suiteInfo2;
    TestFixtureInfo fixtureInfo2;
    TestInfo testInfo2;
};

TEST_FIXTURE(PredicateTest, TrueTest)
{
    True nonFiltering;

    EXPECT_TRUE(nonFiltering.operator()(&suiteInfo));
    EXPECT_TRUE(nonFiltering.operator()(&fixtureInfo));
    EXPECT_TRUE(nonFiltering.operator()(&testInfo));
    EXPECT_TRUE(nonFiltering.operator()(&suiteInfo2));
    EXPECT_TRUE(nonFiltering.operator()(&fixtureInfo2));
    EXPECT_TRUE(nonFiltering.operator()(&testInfo2));
}

TEST_FIXTURE(PredicateTest, InSelectionTest)
{
    InSelection selection(suiteInfo.Name().c_str(), fixtureInfo.Name().c_str(), testInfo._details.testName.c_str());

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_FALSE(selection.operator()(&suiteInfo2));
    EXPECT_FALSE(selection.operator()(&fixtureInfo2));
    EXPECT_FALSE(selection.operator()(&testInfo2));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_SuiteDotFixtureDotTest)
{
    std::string filter = suiteInfo.Name() + "." + fixtureInfo.Name() + "." + testInfo._details.testName;
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_FALSE(selection.operator()(&suiteInfo2));
    EXPECT_FALSE(selection.operator()(&fixtureInfo2));
    EXPECT_FALSE(selection.operator()(&testInfo2));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_FixtureDotTest)
{
    std::string filter = fixtureInfo.Name() + "." + testInfo._details.testName;
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_TRUE(selection.operator()(&suiteInfo2));
    EXPECT_FALSE(selection.operator()(&fixtureInfo2));
    EXPECT_FALSE(selection.operator()(&testInfo2));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_Wildcard)
{
    std::string filter = "*";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_TRUE(selection.operator()(&suiteInfo2));
    EXPECT_TRUE(selection.operator()(&fixtureInfo2));
    EXPECT_TRUE(selection.operator()(&testInfo2));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_SuiteDotFixtureDotWildcard)
{
    std::string filter = suiteInfo.Name() + "." + fixtureInfo.Name() + ".*";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_FALSE(selection.operator()(&suiteInfo2));
    EXPECT_FALSE(selection.operator()(&fixtureInfo2));
    EXPECT_TRUE(selection.operator()(&testInfo2));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_FixtureDotWildcard)
{
    std::string filter = fixtureInfo.Name() + ".*";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_TRUE(selection.operator()(&suiteInfo2));
    EXPECT_FALSE(selection.operator()(&fixtureInfo2));
    EXPECT_TRUE(selection.operator()(&testInfo2));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_WildcardDotFixtureDotTest)
{
    std::string filter = "*." + fixtureInfo.Name() + "." + testInfo._details.testName;
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_TRUE(selection.operator()(&suiteInfo2));
    EXPECT_FALSE(selection.operator()(&fixtureInfo2));
    EXPECT_FALSE(selection.operator()(&testInfo2));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_WildcardDotTest)
{
    std::string filter = "*." + testInfo._details.testName;
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_TRUE(selection.operator()(&suiteInfo2));
    EXPECT_TRUE(selection.operator()(&fixtureInfo2));
    EXPECT_FALSE(selection.operator()(&testInfo2));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_WildcardPartialSuiteDotFixtureDotTest)
{
    std::string filter = "*suite." + fixtureInfo.Name() + "." + testInfo._details.testName;
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_TRUE(selection.operator()(&suiteInfo2));
    EXPECT_FALSE(selection.operator()(&fixtureInfo2));
    EXPECT_FALSE(selection.operator()(&testInfo2));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_WildcardPartialFixtureDotTest)
{
    std::string filter = "*fixture." + testInfo._details.testName;
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_TRUE(selection.operator()(&suiteInfo2));
    EXPECT_TRUE(selection.operator()(&fixtureInfo2));
    EXPECT_FALSE(selection.operator()(&testInfo2));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_WildcardPartialTest)
{
    std::string filter = "*test";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_TRUE(selection.operator()(&suiteInfo2));
    EXPECT_TRUE(selection.operator()(&fixtureInfo2));
    EXPECT_TRUE(selection.operator()(&testInfo2));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_SuiteDotFixtureDotPartialTestWildcard)
{
    std::string filter = suiteInfo.Name() + "." + fixtureInfo.Name() + ".test*";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_FALSE(selection.operator()(&suiteInfo2));
    EXPECT_FALSE(selection.operator()(&fixtureInfo2));
    EXPECT_FALSE(selection.operator()(&testInfo2));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_SuiteDotPartialFixtureWildcard)
{
    std::string filter = suiteInfo.Name() + ".fixture*";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_FALSE(selection.operator()(&suiteInfo2));
    EXPECT_FALSE(selection.operator()(&fixtureInfo2));
    EXPECT_TRUE(selection.operator()(&testInfo2));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_PartialSuiteWildcard)
{
    std::string filter = "suite*";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&suiteInfo));
    EXPECT_TRUE(selection.operator()(&fixtureInfo));
    EXPECT_TRUE(selection.operator()(&testInfo));
    EXPECT_FALSE(selection.operator()(&suiteInfo2));
    EXPECT_TRUE(selection.operator()(&fixtureInfo2));
    EXPECT_TRUE(selection.operator()(&testInfo2));
}

} // TEST_SUITE(unit_test_cpp)

} // namespace _Test

} // namespace UnitTestCpp
