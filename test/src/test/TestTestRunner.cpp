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
        : testName("test")
        , otherTestName("othertest")
        , dummyTestName("dummyT")
        , fixtureName("fixture")
        , otherFixtureName("otherfixture")
        , dummyFixtureName("dummyF")
        , suiteName("suite")
        , otherSuiteName("othersuite")
        , dummySuiteName("dummyS")
        , testInfoSuiteFixtureTest(testName, fixtureName, suiteName)
        , testInfoSuiteFixtureOtherTest(otherTestName, fixtureName, suiteName)
        , testInfoSuiteOtherFixtureTest(testName, otherFixtureName, suiteName)
        , testInfoOtherSuiteFixtureTest(testName, fixtureName, otherSuiteName)
        , testInfoOtherSuiteOtherFixtureOtherTest(otherTestName, otherFixtureName, otherSuiteName)
        , testInfoDummySDummyFDummyT(dummyTestName, dummyFixtureName, dummySuiteName)
        , registry()
    {
        {
            TestRegistrar registrar(registry, &testInfoSuiteFixtureTest);
        }
        {
            TestRegistrar registrar(registry, &testInfoSuiteFixtureOtherTest);
        }
        {
            TestRegistrar registrar(registry, &testInfoSuiteOtherFixtureTest);
        }
        {
            TestRegistrar registrar(registry, &testInfoOtherSuiteFixtureTest);
        }
        {
            TestRegistrar registrar(registry, &testInfoOtherSuiteOtherFixtureOtherTest);
        }
        {
            TestRegistrar registrar(registry, &testInfoDummySDummyFDummyT);
        }
    }
    std::string testName;
    std::string otherTestName;
    std::string dummyTestName;
    std::string fixtureName;
    std::string otherFixtureName;
    std::string dummyFixtureName;
    std::string suiteName;
    std::string otherSuiteName;
    std::string dummySuiteName;

    Test testInfoSuiteFixtureTest;
    Test testInfoSuiteFixtureOtherTest;
    Test testInfoSuiteOtherFixtureTest;
    Test testInfoOtherSuiteFixtureTest;
    Test testInfoOtherSuiteOtherFixtureOtherTest;
    Test testInfoDummySDummyFDummyT;
    TestRegistry registry;
};

TEST_FIXTURE(PredicateTest, TrueTest)
{
    True nonFiltering;

    EXPECT_TRUE(nonFiltering.operator()(&testInfoSuiteFixtureTest));
    EXPECT_TRUE(nonFiltering.operator()(&testInfoSuiteFixtureOtherTest));
    EXPECT_TRUE(nonFiltering.operator()(&testInfoSuiteOtherFixtureTest));
    EXPECT_TRUE(nonFiltering.operator()(&testInfoOtherSuiteFixtureTest));
    EXPECT_TRUE(nonFiltering.operator()(&testInfoOtherSuiteOtherFixtureOtherTest));
    EXPECT_TRUE(nonFiltering.operator()(&testInfoDummySDummyFDummyT));

    EXPECT_EQ(3, CountSuites(registry));
    EXPECT_EQ(3, CountSuitesIf(registry, nonFiltering));
    EXPECT_EQ(5, CountFixtures(registry));
    EXPECT_EQ(5, CountFixturesIf(registry, nonFiltering));
    EXPECT_EQ(6, CountTests(registry));
    EXPECT_EQ(6, CountTestsIf(registry, nonFiltering));
}

TEST_FIXTURE(PredicateTest, InSelectionTest)
{
    InSelection selection(suiteName.c_str(), fixtureName.c_str(), testName.c_str());

    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoSuiteFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoSuiteOtherFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoOtherSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoOtherSuiteOtherFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoDummySDummyFDummyT));

    EXPECT_EQ(3, CountSuites(registry));
    EXPECT_EQ(1, CountSuitesIf(registry, selection));
    EXPECT_EQ(5, CountFixtures(registry));
    EXPECT_EQ(1, CountFixturesIf(registry, selection));
    EXPECT_EQ(6, CountTests(registry));
    EXPECT_EQ(1, CountTestsIf(registry, selection));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_SuiteDotFixtureDotTest)
{
    std::string filter = suiteName + "." + fixtureName + "." + testName;
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoSuiteFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoSuiteOtherFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoOtherSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoOtherSuiteOtherFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoDummySDummyFDummyT));

    EXPECT_EQ(3, CountSuites(registry));
    EXPECT_EQ(1, CountSuitesIf(registry, selection));
    EXPECT_EQ(5, CountFixtures(registry));
    EXPECT_EQ(1, CountFixturesIf(registry, selection));
    EXPECT_EQ(6, CountTests(registry));
    EXPECT_EQ(1, CountTestsIf(registry, selection));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_FixtureDotTest)
{
    std::string filter = fixtureName + "." + testName;
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoSuiteFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoSuiteOtherFixtureTest));
    EXPECT_TRUE(selection.operator()(&testInfoOtherSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoOtherSuiteOtherFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoDummySDummyFDummyT));

    EXPECT_EQ(3, CountSuites(registry));
    EXPECT_EQ(2, CountSuitesIf(registry, selection));
    EXPECT_EQ(5, CountFixtures(registry));
    EXPECT_EQ(2, CountFixturesIf(registry, selection));
    EXPECT_EQ(6, CountTests(registry));
    EXPECT_EQ(2, CountTestsIf(registry, selection));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_FixtureDotTestWithNegative)
{
    std::string filter = fixtureName + "." + testName + ":-" + suiteName + "." + fixtureName + "." + testName;
    InSelectionFilter selection(filter);

    EXPECT_FALSE(selection.operator()(&testInfoSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoSuiteFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoSuiteOtherFixtureTest));
    EXPECT_TRUE(selection.operator()(&testInfoOtherSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoOtherSuiteOtherFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoDummySDummyFDummyT));

    EXPECT_EQ(3, CountSuites(registry));
    EXPECT_EQ(1, CountSuitesIf(registry, selection));
    EXPECT_EQ(5, CountFixtures(registry));
    EXPECT_EQ(1, CountFixturesIf(registry, selection));
    EXPECT_EQ(6, CountTests(registry));
    EXPECT_EQ(1, CountTestsIf(registry, selection));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_FixtureDotTestWithEmptyNegative)
{
    std::string filter = fixtureName + "." + testName + ":-" + dummySuiteName + "." + fixtureName + "." + testName;
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoSuiteFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoSuiteOtherFixtureTest));
    EXPECT_TRUE(selection.operator()(&testInfoOtherSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoOtherSuiteOtherFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoDummySDummyFDummyT));

    EXPECT_EQ(3, CountSuites(registry));
    EXPECT_EQ(2, CountSuitesIf(registry, selection));
    EXPECT_EQ(5, CountFixtures(registry));
    EXPECT_EQ(2, CountFixturesIf(registry, selection));
    EXPECT_EQ(6, CountTests(registry));
    EXPECT_EQ(2, CountTestsIf(registry, selection));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_Wildcard)
{
    std::string filter = "*";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureTest));
    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureOtherTest));
    EXPECT_TRUE(selection.operator()(&testInfoSuiteOtherFixtureTest));
    EXPECT_TRUE(selection.operator()(&testInfoOtherSuiteFixtureTest));
    EXPECT_TRUE(selection.operator()(&testInfoOtherSuiteOtherFixtureOtherTest));
    EXPECT_TRUE(selection.operator()(&testInfoDummySDummyFDummyT));

    EXPECT_EQ(3, CountSuites(registry));
    EXPECT_EQ(3, CountSuitesIf(registry, selection));
    EXPECT_EQ(5, CountFixtures(registry));
    EXPECT_EQ(5, CountFixturesIf(registry, selection));
    EXPECT_EQ(6, CountTests(registry));
    EXPECT_EQ(6, CountTestsIf(registry, selection));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_SuiteDotFixtureDotWildcard)
{
    std::string filter = suiteName + "." + fixtureName + ".*";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureTest));
    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoSuiteOtherFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoOtherSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoOtherSuiteOtherFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoDummySDummyFDummyT));

    EXPECT_EQ(3, CountSuites(registry));
    EXPECT_EQ(1, CountSuitesIf(registry, selection));
    EXPECT_EQ(5, CountFixtures(registry));
    EXPECT_EQ(1, CountFixturesIf(registry, selection));
    EXPECT_EQ(6, CountTests(registry));
    EXPECT_EQ(2, CountTestsIf(registry, selection));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_FixtureDotWildcard)
{
    std::string filter = suiteName + ".*";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureTest));
    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureOtherTest));
    EXPECT_TRUE(selection.operator()(&testInfoSuiteOtherFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoOtherSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoOtherSuiteOtherFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoDummySDummyFDummyT));

    EXPECT_EQ(3, CountSuites(registry));
    EXPECT_EQ(1, CountSuitesIf(registry, selection));
    EXPECT_EQ(5, CountFixtures(registry));
    EXPECT_EQ(2, CountFixturesIf(registry, selection));
    EXPECT_EQ(6, CountTests(registry));
    EXPECT_EQ(3, CountTestsIf(registry, selection));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_WildcardDotFixtureDotTest)
{
    std::string filter = "*." + fixtureName + "." + testName;
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoSuiteFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoSuiteOtherFixtureTest));
    EXPECT_TRUE(selection.operator()(&testInfoOtherSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoOtherSuiteOtherFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoDummySDummyFDummyT));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_WildcardDotTest)
{
    std::string filter = "*." + testName;
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoSuiteFixtureOtherTest));
    EXPECT_TRUE(selection.operator()(&testInfoSuiteOtherFixtureTest));
    EXPECT_TRUE(selection.operator()(&testInfoOtherSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoOtherSuiteOtherFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoDummySDummyFDummyT));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_SuiteDotPartialFixtureWildcardDotWildcard)
{
    std::string filter = suiteName + "." + fixtureName + "*.*";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureTest));
    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoSuiteOtherFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoOtherSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoOtherSuiteOtherFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoDummySDummyFDummyT));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_PartialSuiteWildcardDotWildcard)
{
    std::string filter = suiteName + "*.*";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureTest));
    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureOtherTest));
    EXPECT_TRUE(selection.operator()(&testInfoSuiteOtherFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoOtherSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoOtherSuiteOtherFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoDummySDummyFDummyT));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_WildcardDotWildcardPartialFixtureDotTest)
{
    std::string filter = "*.*" + fixtureName + "." + testName;
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoSuiteFixtureOtherTest));
    EXPECT_TRUE(selection.operator()(&testInfoSuiteOtherFixtureTest));
    EXPECT_TRUE(selection.operator()(&testInfoOtherSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoOtherSuiteOtherFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoDummySDummyFDummyT));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_WildcardDotWildcardPartialTest)
{
    std::string filter = "*.*" + testName;
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureTest));
    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureOtherTest));
    EXPECT_TRUE(selection.operator()(&testInfoSuiteOtherFixtureTest));
    EXPECT_TRUE(selection.operator()(&testInfoOtherSuiteFixtureTest));
    EXPECT_TRUE(selection.operator()(&testInfoOtherSuiteOtherFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoDummySDummyFDummyT));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_WildcardPartialSuiteDotFixtureDotTest)
{
    std::string filter = "*suite." + fixtureName + "." + testName;
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoSuiteFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoSuiteOtherFixtureTest));
    EXPECT_TRUE(selection.operator()(&testInfoOtherSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoOtherSuiteOtherFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoDummySDummyFDummyT));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_WildcardPartialFixtureDotTest)
{
    std::string filter = "*fixture." + testName;
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoSuiteFixtureOtherTest));
    EXPECT_TRUE(selection.operator()(&testInfoSuiteOtherFixtureTest));
    EXPECT_TRUE(selection.operator()(&testInfoOtherSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoOtherSuiteOtherFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoDummySDummyFDummyT));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_WildcardPartialTest)
{
    std::string filter = "*test";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureTest));
    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureOtherTest));
    EXPECT_TRUE(selection.operator()(&testInfoSuiteOtherFixtureTest));
    EXPECT_TRUE(selection.operator()(&testInfoOtherSuiteFixtureTest));
    EXPECT_TRUE(selection.operator()(&testInfoOtherSuiteOtherFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoDummySDummyFDummyT));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_SuiteDotFixtureDotPartialTestWildcard)
{
    std::string filter = suiteName + "." + fixtureName + ".test*";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoSuiteFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoSuiteOtherFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoOtherSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoOtherSuiteOtherFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoDummySDummyFDummyT));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_SuiteDotPartialFixtureWildcard)
{
    std::string filter = suiteName + ".fixture*";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureTest));
    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoSuiteOtherFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoOtherSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoOtherSuiteOtherFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoDummySDummyFDummyT));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_PartialSuiteWildcard)
{
    std::string filter = "suite*";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureTest));
    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureOtherTest));
    EXPECT_TRUE(selection.operator()(&testInfoSuiteOtherFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoOtherSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoOtherSuiteOtherFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoDummySDummyFDummyT));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_WildcardDotPartialTestWildcard)
{
    std::string filter = "*.test*";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoSuiteFixtureOtherTest));
    EXPECT_TRUE(selection.operator()(&testInfoSuiteOtherFixtureTest));
    EXPECT_TRUE(selection.operator()(&testInfoOtherSuiteFixtureTest));
    EXPECT_FALSE(selection.operator()(&testInfoOtherSuiteOtherFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoDummySDummyFDummyT));
}

TEST_FIXTURE(PredicateTest, InSelectionFilterTest_WildcardPartialFixtureDotWildcard)
{
    std::string filter = "*fixture.*";
    InSelectionFilter selection(filter);

    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureTest));
    EXPECT_TRUE(selection.operator()(&testInfoSuiteFixtureOtherTest));
    EXPECT_TRUE(selection.operator()(&testInfoSuiteOtherFixtureTest));
    EXPECT_TRUE(selection.operator()(&testInfoOtherSuiteFixtureTest));
    EXPECT_TRUE(selection.operator()(&testInfoOtherSuiteOtherFixtureOtherTest));
    EXPECT_FALSE(selection.operator()(&testInfoDummySDummyFDummyT));
}

} // TEST_SUITE(unit_test_cpp)

} // namespace _Test

} // namespace UnitTestCpp
