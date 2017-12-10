#include <unittest-c++/Timer.h>
#include <unistd.h>
#include <sys/time.h>

namespace UnitTestCpp
{

Timer::Timer()
{
    memset(&startTime, 0, sizeof(startTime));
}

void Timer::Start()
{
    gettimeofday(&startTime, nullptr);
}

int Timer::GetTimeInMilliSeconds() const
{
    timeval currentTime;
	gettimeofday(&currentTime, nullptr);
    int const differenceSeconds = currentTime.tv_sec - startTime.tv_sec;
    int const differenceMicroSeconds = currentTime.tv_usec - startTime.tv_usec;
    return differenceSeconds * 1000 + differenceMicroSeconds / 1000;
}

void TimeHelpers::SleepMilliSeconds(int ms)
{
    usleep(ms * 1000);
}

} // namespace UnitTestCpp
