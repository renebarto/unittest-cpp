#pragma once

#include <sstream>
#include <unittest-cpp/AssertException.h>
#include <unittest-cpp/AssertionFailedException.h>
#include <unittest-cpp/Config.h>
#include <unittest-cpp/CurrentTest.h>
#include <unittest-cpp/TestDetails.h>
#include <unittest-cpp/TestResults.h>

#ifdef UNITTEST_POSIX
#include <unittest-cpp/SignalTranslator.h>
#endif

using namespace std;

namespace UnitTestCpp
{

template< typename T >
void ExecuteTest(T & testObject, const TestDetails & details)
{
    CurrentTest::Details() = &details;

    try
    {
#ifdef UNITTEST_POSIX
        UNITTEST_THROW_SIGNALS
#endif
        testObject.RunImpl();
    }
    catch (const AssertException & e)
    {
        CurrentTest::Results()->OnTestFailure(TestDetails(details.testName, details.fixtureName, details.suiteName, e.Filename(), e.LineNumber()), e.what());
    }
    catch (const AssertionFailedException &)
    {
        // This assertion is thrown after ASSERT_XX failed to jump out of the test.
    }
    catch (const std::exception & e)
    {
        ostringstream stream;
        stream << "Unhandled exception: " << e.what();
        CurrentTest::Results()->OnTestFailure(details, stream.str());
    }
    catch (...)
    {
        CurrentTest::Results()->OnTestFailure(details, "Unhandled exception: Crash!");
    }
}

} // namespace UnitTestCpp
