#include <unittest-cpp/TimeConstraint.h>

#include <sstream>
#include <unittest-cpp/CurrentTest.h>
#include <unittest-cpp/TestResults.h>

using namespace std;

namespace UnitTestCpp
{

TimeConstraint::TimeConstraint(int timeConstraintMilliSeconds, const TestDetails & details) :
    _details(details),
    _timeConstraintMilliSeconds(timeConstraintMilliSeconds)
{
    _timer.Start();
}

TimeConstraint::~TimeConstraint()
{
    int const totalTimeInMilliSeconds = _timer.GetTimeInMilliSeconds();
    if (totalTimeInMilliSeconds > _timeConstraintMilliSeconds)
    {
        ostringstream stream;
        stream << "Time constraint failed. Expected to run test under "
               << _timeConstraintMilliSeconds << "ms but took " << totalTimeInMilliSeconds << "ms.";

        UnitTestCpp::CurrentTest::Results()->OnTestFailure(_details, stream.str());
    }
}

} // namespace UnitTestCpp
