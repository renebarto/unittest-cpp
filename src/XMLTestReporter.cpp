#include <unittest-cpp/XMLTestReporter.h>

#include <sstream>
#include <unittest-cpp/TestFailure.h>
#include <unittest-cpp/TestResults.h>

using namespace std;

namespace
{

void ReplaceChar(std::string & str, char c, const std::string & replacement)
{
    for (size_t pos = str.find(c); pos != std::string::npos; pos = str.find(c, pos + 1))
        str.replace(pos, 1, replacement);
}

std::string XmlEscape(const std::string & value)
{
    std::string escaped = value;

    ReplaceChar(escaped, '&', "&amp;");
    ReplaceChar(escaped, '<', "&lt;");
    ReplaceChar(escaped, '>', "&gt;");
    ReplaceChar(escaped, '\'', "&apos;");
    ReplaceChar(escaped, '\"', "&quot;");

    return escaped;
}

std::string BuildFailureMessage(const std::string & file, int line, const std::string & message)
{
    basic_ostringstream<char> failureMessage;
    failureMessage << file << "(" << line << ") : " << message;
    return failureMessage.str();
}

}

namespace UnitTestCpp
{

XMLTestReporter::XMLTestReporter(basic_ostream<char> & _stream)
    : _stream(_stream)
{
}

void XMLTestReporter::ReportTestRunSummary(const TestResults * results, int milliSecondsElapsed)
{
    AddXmlElement(nullptr);

    BeginResults(results->GetTotalTestCount(), results->GetFailedTestCount(),
                 results->GetFailureCount(), milliSecondsElapsed);

    for (auto result : Results())
    {
        BeginTest(result);

        if (result.Failed())
            AddFailure(result);

        EndTest(result);
    }

    EndResults();
}

void XMLTestReporter::AddXmlElement(const char * encoding)
{
    _stream << "<?xml version=\"1.0\"";

    if (encoding != nullptr)
        _stream << " encoding=\"" << encoding << "\"";

    _stream << "?>" << endl;
}

void XMLTestReporter::BeginResults(int totalTestCount, int failedTestCount,
                                   int failureCount, int milliSecondsElapsed)
{
   _stream << "<unittest-results"
       << " tests=\"" << totalTestCount << "\""
       << " failedtests=\"" << failedTestCount << "\""
       << " failures=\"" << failureCount << "\""
       << " time=\"" << static_cast<double>(milliSecondsElapsed) / 1000.0 << "\""
       << ">" << endl;
}

void XMLTestReporter::EndResults()
{
    _stream << "</unittest-results>" << endl;
}

void XMLTestReporter::BeginTest(const TestDetailedResult & result)
{
    _stream << "<test"
        << " suite=\"" << result.suiteName << "\""
        << " fixture=\"" << result.fixtureName << "\""
        << " name=\"" << result.testName << "\""
        << " time=\"" << static_cast<double>(result.MilliSecondsElapsed()) / 1000.0 << "\"";
}

void XMLTestReporter::EndTest(const TestDetailedResult & result)
{
    if (result.Failed())
        _stream << "</test>" << endl;
    else
        _stream << "/>" << endl;
}

void XMLTestReporter::AddFailure(const TestDetailedResult & result)
{
    _stream << ">" << endl; // close <test> element

    for (auto failure : result.Failures())
    {
        std::string const escapedMessage = XmlEscape(failure.second);
        std::string const message = BuildFailureMessage(result.fileName, failure.first, escapedMessage);

        _stream << "<failure" << " message=\"" << message << "\"" << "/>" << endl;
    }
}

} // namespace UnitTestCpp
