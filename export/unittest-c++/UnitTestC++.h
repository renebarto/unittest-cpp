#pragma once

#include <unittest-c++/AssertionFailedException.h>
#include <unittest-c++/AssertMacros.h>
#include <unittest-c++/ConsoleTestReporter.h>
#include <unittest-c++/CurrentTest.h>
#include <unittest-c++/ITestReporter.h>
#include <unittest-c++/TestFixture.h>
#include <unittest-c++/TestMacros.h>
#include <unittest-c++/TestRunner.h>
#include <unittest-c++/TimeConstraint.h>
#include <unittest-c++/XMLTestReporter.h>

namespace UnitTestCpp
{

UNIT_TEST_CPP_EXPORT int RunAllTests();
UNIT_TEST_CPP_EXPORT int RunAllTests(ITestReporter & reporter);

template <class Predicate>
int RunSelectedTests(const Predicate & predicate)
{
	ConsoleTestReporter reporter;
	return RunSelectedTests(reporter, predicate);
}

template <class Predicate>
int RunSelectedTests(ITestReporter & reporter, const Predicate & predicate)
{
	TestRunner runner(reporter);
	return runner.RunTestsIf(Test::GetTestRegistry(), predicate, 0);
}

} // namespace UnitTestCpp
