#include <unittest-cpp/Timer.h>
#if defined(LINUX) || defined(DARWIN)
#include <unistd.h>
#include <sys/time.h>
#elif defined(WIN_MSVC) || defined(WIN_MINGW)
#include <time.h>

/* FILETIME of Jan 1 1970 00:00:00. */
constexpr unsigned __int64 epoch = ((unsigned __int64)116444736000000000ULL);

inline int gettimeofday(struct timeval *time, struct timezone *UNUSED(timeZone))
{
    FILETIME file_time;
    SYSTEMTIME system_time;
    ULARGE_INTEGER ularge;

    GetSystemTime(&system_time);
    SystemTimeToFileTime(&system_time, &file_time);
    ularge.u.LowPart = file_time.dwLowDateTime;
    ularge.u.HighPart = file_time.dwHighDateTime;

    time->tv_sec = (long)((ularge.QuadPart - epoch) / 10000000L);
    time->tv_usec = (long)(system_time.wMilliseconds * 1000);

    return 0;
}

inline bool sleep(const LARGE_INTEGER & timerValue)
{
    HANDLE timer = CreateWaitableTimer(NULL, TRUE, NULL);
    if (!timer)
        return false;
    if (!SetWaitableTimer(timer, &timerValue, 0, NULL, NULL, 0))
        return false;
    DWORD result = WaitForSingleObject(timer, INFINITE);
    if ((result == WAIT_FAILED) || (result == WAIT_TIMEOUT))
        return false;
    CloseHandle(timer);
    return true;
}

inline int usleep(int64_t microSeconds)
{
    LARGE_INTEGER ft;

    ft.QuadPart = -(10 * microSeconds); // Convert to 100 nanosecond interval, negative value indicates relative time
    return sleep(ft) ? 0 : -1;
}

#endif

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
