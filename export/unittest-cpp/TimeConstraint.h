#pragma once

#include <unittest-cpp/exports.h>
#include <unittest-cpp/Timer.h>
#include <unittest-cpp/TestDetails.h>

namespace UnitTestCpp
{

class TestResults;
class TestDetails;

class UNIT_TEST_CPP_EXPORT TimeConstraint
{
public:
    TimeConstraint() = delete;
    TimeConstraint(const TimeConstraint &) = delete;
    TimeConstraint(int timeConstraintMilliSeconds, const TestDetails & details);
    ~TimeConstraint();

    TimeConstraint & operator = (const TimeConstraint &) = delete;

private:
    Timer _timer;
    TestDetails _details;
    int const _timeConstraintMilliSeconds;
};

#define UNITTEST_TIME_CONSTRAINT(ms)                                                               \
    UnitTestCpp::TimeConstraint unitTest__timeConstraint__(ms,                                     \
                                                           UnitTestCpp::TestDetails(_details, __LINE__))

#define UNITTEST_TIME_CONSTRAINT_EXEMPT() do { _timeConstraintMilliSeconds = true; } while (0)

} // namespace UnitTestCpp
