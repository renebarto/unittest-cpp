#include <unittest-cpp/AssertException.h>

#include <cstring>

using namespace UnitTestCpp;

AssertException::AssertException(const std::string & description,
                                 const std::string & filename,
                                 int lineNumber)
    : description(ToNarrowString(description))
    , fileName(filename)
    , lineNumber(lineNumber)
{
}

AssertException::~AssertException()
{
}
