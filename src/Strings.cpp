#include "unittest-c++/Strings.h"
#include "unittest-c++/OSAL.h"

using namespace UnitTestCpp;

std::string UnitTestCpp::Trim(const std::string & value, const std::string & stripChars)
{
    const char * find = stripChars.c_str();

    size_t indexLeft = 0;
    while ((indexLeft < value.length()) && strchr(find, value[indexLeft]))
        ++indexLeft;
    size_t indexRight = value.length();
    while ((indexRight > indexLeft) && strchr(find, value[indexRight - 1]))
        --indexRight;
    return value.substr(indexLeft, indexRight - indexLeft);
}

std::string UnitTestCpp::TrimSpaces(const std::string & value)
{
    size_t indexLeft = 0;
    while ((indexLeft < value.length()) && isspace(value[indexLeft]))
        ++indexLeft;
    size_t indexRight = value.length();
    while ((indexRight > indexLeft) && isspace(value[indexRight - 1]))
        --indexRight;
    return value.substr(indexLeft, indexRight - indexLeft);
}

std::string UnitTestCpp::TrimLeft(const std::string & value, const std::string & stripChars)
{
    const char * find = stripChars.c_str();

    size_t indexLeft = 0;
    while ((indexLeft < value.length()) && strchr(find, value[indexLeft]))
        ++indexLeft;
    return value.substr(indexLeft, value.length() - indexLeft);
}

std::string UnitTestCpp::TrimSpacesLeft(const std::string & value)
{
    size_t indexLeft = 0;
    while ((indexLeft < value.length()) && isspace(value[indexLeft]))
        ++indexLeft;
    return value.substr(indexLeft, value.length() - indexLeft);
}

std::string UnitTestCpp::TrimRight(const std::string & value, const std::string & stripChars)
{
    const char * find = stripChars.c_str();

    size_t indexRight = value.length();
    while ((indexRight > 0) && strchr(find, value[indexRight - 1]))
        --indexRight;
    return value.substr(0, indexRight);
}

std::string UnitTestCpp::TrimSpacesRight(const std::string & value)
{
    size_t indexRight = value.length();
    while ((indexRight > 0) && isspace(value[indexRight - 1]))
        --indexRight;
    return value.substr(0, indexRight - 0);
}

bool UnitTestCpp::IsEqual(const std::string & lhs, const std::string & rhs)
{
    return (lhs == rhs);
}

bool UnitTestCpp::IsEqualIgnoreCase(const std::string & lhs, const std::string & rhs)
{
    if (lhs.size() != rhs.size())
        return false;

    size_t size = rhs.size();
    for (size_t n = 0; n < size; n++)
        if (tolower(lhs[n]) != tolower(rhs[n]))
            return false;

    return true;
}

bool UnitTestCpp::IsEqual(const char * lhs, const char * rhs)
{
    return strcmp(lhs, rhs) == 0;
}

bool UnitTestCpp::IsEqualIgnoreCase(const char * lhs, const char * rhs)
{
    return strcasecmp(lhs, rhs) == 0;
}

bool UnitTestCpp::IsEqual(const wchar_t * lhs, const wchar_t * rhs)
{
    return wcscmp(lhs, rhs) == 0;
}

bool UnitTestCpp::IsEqualIgnoreCase(const wchar_t * lhs, const wchar_t * rhs)
{
    return wcscasecmp(lhs, rhs) == 0;
}

std::string UnitTestCpp::ToLower(const std::string & text)
{
    std::string result;
    for (size_t i = 0; i < text.length(); i++)
    {
#if defined(UNICODE) || defined(_UNICODE)
        result += towlower(text[i]);
#else
        result += tolower(text[i]);
#endif
    }
    return result;
}

std::string UnitTestCpp::ToUpper(const std::string & text)
{
    std::string result;
    for (size_t i = 0; i < text.length(); i++)
    {
#if defined(UNICODE) || defined(_UNICODE)
        result += towupper(text[i]);
#else
        result += toupper(text[i]);
#endif
    }
    return result;
}

std::string UnitTestCpp::Quote(const std::string & text)
{
    if ((text[0] == '\"') && (text[text.length() - 1] == '\"'))
    {
        return text;
    }
    return "\"" + text + "\"";
}

std::string UnitTestCpp::UnQuote(const std::string & text)
{
    if ((text.length() >= 2) && (text[0] == '\"') && (text[text.length() - 1] == '\"'))
    {
        return text.substr(1, text.length() - 2);
    }
    return text;
}
