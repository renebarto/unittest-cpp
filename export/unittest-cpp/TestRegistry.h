#pragma once

#include <string>
#include <unittest-cpp/exports.h>
#include <unittest-cpp/TestSuiteInfo.h>

namespace UnitTestCpp
{

class TestInfo;

class UNIT_TEST_CPP_EXPORT TestRegistry
{
public:
    TestRegistry();
    TestRegistry(const TestRegistry &) = delete;
    virtual ~TestRegistry();

    TestRegistry & operator = (const TestRegistry &) = delete;

    TestSuiteInfo * GetTestSuite(const std::string & suiteName);
    TestSuiteInfo * GetHead() const;

private:
    TestSuiteInfo * head;
    TestSuiteInfo * tail;

    void AddSuite(TestSuiteInfo * testSuite);
};

class UNIT_TEST_CPP_EXPORT TestRegistrar
{
public:
    TestRegistrar(TestRegistry & registry, Test * test);
};

} // namespace UnitTestCpp
