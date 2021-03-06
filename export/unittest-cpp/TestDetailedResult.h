#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <unittest-cpp/Strings.h>

namespace UnitTestCpp
{

class TestDetails;

class UNIT_TEST_CPP_EXPORT TestDetailedResult
{
public:
    TestDetailedResult() = delete;
    TestDetailedResult(const TestDetails & details);

    std::string suiteName;
    std::string fixtureName;
    std::string testName;
    std::string fileName;
    int lineNumber;

    typedef std::pair<int, std::string> Failure;
    typedef std::vector<Failure> FailureList;

    void AddFailure(Failure failure);
    const FailureList & Failures() const { return failures; }
    FailureList & Failures() { return failures; }
    void Failed(bool value) { failed = value; }
    bool Failed() const { return failed; }
    void MilliSecondsElapsed(int value) { milliSecondsElapsed = value; }
    int MilliSecondsElapsed() const { return milliSecondsElapsed; }

private:
    FailureList failures;
    bool failed;
    int milliSecondsElapsed;
};

inline std::ostream & operator << (std::ostream & stream, const TestDetailedResult & result)
{
    return stream << result.fileName << ":" << result.lineNumber << " : Test "
                  << result.suiteName << "::" << result.fixtureName << "::" << result.testName;
}

} // namespace UnitTestCpp
