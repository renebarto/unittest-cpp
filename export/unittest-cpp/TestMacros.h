#pragma once

#include <unittest-cpp/TestSuite.h>
#include <unittest-cpp/TestRegistry.h>
#include <unittest-cpp/ExecuteTest.h>
#include <unittest-cpp/AssertException.h>

#define TEST_EX(Name, List)                                                                        \
class Test##Name : public UnitTestCpp::Test                                                        \
{                                                                                                  \
public:                                                                                            \
    Test##Name() : Test(#Name, "", GetSuiteName(), __FILE__, __LINE__) {}                          \
private:                                                                                           \
    virtual void RunImpl() const;                                                                  \
} test##Name##Instance;                                                                            \
                                                                                                   \
UnitTestCpp::TestRegistrar registrar##Name(List, &test##Name##Instance);                           \
                                                                                                   \
void Test##Name::RunImpl() const

#define TEST(Name) TEST_EX(Name, UnitTestCpp::Test::GetTestRegistry())

#define TEST_FIXTURE_EX(Fixture,Name,List)                                                         \
class Fixture##Name##Helper : public Fixture                                                       \
{                                                                                                  \
public:                                                                                            \
    Fixture##Name##Helper(const Fixture##Name##Helper &) = delete;                                 \
    explicit Fixture##Name##Helper(const UnitTestCpp::TestDetails & details) :                     \
        __details(details)                                                                         \
    {                                                                                              \
        SetUp();                                                                                   \
    }                                                                                              \
    ~Fixture##Name##Helper()                                                                       \
    {                                                                                              \
        TearDown();                                                                                \
    }                                                                                              \
    Fixture##Name##Helper & operator = (const Fixture##Name##Helper &) = delete;                   \
    void RunImpl();                                                                                \
    const UnitTestCpp::TestDetails & __details;                                                    \
};                                                                                                 \
                                                                                                   \
class Test##Fixture##Name : public UnitTestCpp::Test                                               \
{                                                                                                  \
public:                                                                                            \
    Test##Fixture##Name() :                                                                        \
        Test(#Name, #Fixture, GetSuiteName(), __FILE__, __LINE__)                                  \
    {                                                                                              \
    }                                                                                              \
private:                                                                                           \
    virtual void RunImpl() const;                                                                  \
} test##Fixture##Name##Instance;                                                                   \
                                                                                                   \
UnitTestCpp::TestRegistrar registrar##Fixture##Name(List, &test##Fixture##Name##Instance);         \
                                                                                                   \
void Test##Fixture##Name::RunImpl() const                                                          \
{                                                                                                  \
    bool ctorOk = false;                                                                           \
    try                                                                                            \
    {                                                                                              \
        Fixture##Name##Helper fixtureHelper(_details);                                             \
        ctorOk = true;                                                                             \
        UnitTestCpp::ExecuteTest(fixtureHelper, _details);                                         \
    }                                                                                              \
    catch (const ::UnitTestCpp::AssertException & e)                                               \
    {                                                                                              \
        UnitTestCpp::CurrentTest::Results()->OnTestFailure(                                        \
            UnitTestCpp::TestDetails(_details.testName,                                            \
                                     _details.fixtureName,                                         \
                                     _details.suiteName,                                           \
                                     e), e.what());                                                \
    }                                                                                              \
    catch (const std::exception & e)                                                               \
    {                                                                                              \
        std::ostringstream stream;                                                                 \
        stream << "Unhandled exception: " << e.what();                                             \
        UnitTestCpp::CurrentTest::Results()->OnTestFailure(_details, stream.str());                \
    }                                                                                              \
    catch (...)                                                                                    \
    {                                                                                              \
        if (ctorOk)                                                                                \
        {                                                                                          \
            UnitTestCpp::CurrentTest::Results()->OnTestFailure(                                    \
                UnitTestCpp::TestDetails(_details, __LINE__),                                      \
                "Unhandled exception while destroying fixture " #Fixture);                         \
        }                                                                                          \
        else                                                                                       \
        {                                                                                          \
            UnitTestCpp::CurrentTest::Results()->OnTestFailure(                                    \
                UnitTestCpp::TestDetails(_details, __LINE__),                                      \
                "Unhandled exception while constructing fixture " #Fixture);                       \
        }                                                                                          \
    }                                                                                              \
}                                                                                                  \
void Fixture##Name##Helper::RunImpl()

#define TEST_FIXTURE(Fixture,Name) TEST_FIXTURE_EX(Fixture,Name, UnitTestCpp::Test::GetTestRegistry())

#define TEST_SUITE(Name)                                                                           \
    namespace Suite##Name                                                                          \
    {                                                                                              \
        inline char const * GetSuiteName()                                                         \
        {                                                                                          \
            return #Name ;                                                                         \
        }                                                                                          \
    }                                                                                              \
    namespace Suite##Name
