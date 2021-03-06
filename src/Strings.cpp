#include "unittest-cpp/Strings.h"
#include "unittest-cpp/OSAL.h"

namespace UnitTestCpp
{

std::string Trim(const std::string & value, const std::string & stripChars)
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

std::string TrimSpaces(const std::string & value)
{
    size_t indexLeft = 0;
    while ((indexLeft < value.length()) && isspace(value[indexLeft]))
        ++indexLeft;
    size_t indexRight = value.length();
    while ((indexRight > indexLeft) && isspace(value[indexRight - 1]))
        --indexRight;
    return value.substr(indexLeft, indexRight - indexLeft);
}

std::string TrimLeft(const std::string & value, const std::string & stripChars)
{
    const char * find = stripChars.c_str();

    size_t indexLeft = 0;
    while ((indexLeft < value.length()) && strchr(find, value[indexLeft]))
        ++indexLeft;
    return value.substr(indexLeft, value.length() - indexLeft);
}

std::string TrimSpacesLeft(const std::string & value)
{
    size_t indexLeft = 0;
    while ((indexLeft < value.length()) && isspace(value[indexLeft]))
        ++indexLeft;
    return value.substr(indexLeft, value.length() - indexLeft);
}

std::string TrimRight(const std::string & value, const std::string & stripChars)
{
    const char * find = stripChars.c_str();

    size_t indexRight = value.length();
    while ((indexRight > 0) && strchr(find, value[indexRight - 1]))
        --indexRight;
    return value.substr(0, indexRight);
}

std::string TrimSpacesRight(const std::string & value)
{
    size_t indexRight = value.length();
    while ((indexRight > 0) && isspace(value[indexRight - 1]))
        --indexRight;
    return value.substr(0, indexRight - 0);
}

bool IsEqual(const std::string & lhs, const std::string & rhs)
{
    return (lhs == rhs);
}

bool IsEqualIgnoreCase(const std::string & lhs, const std::string & rhs)
{
    if (lhs.size() != rhs.size())
        return false;

    size_t size = rhs.size();
    for (size_t n = 0; n < size; n++)
        if (tolower(lhs[n]) != tolower(rhs[n]))
            return false;

    return true;
}

bool IsEqual(const char * lhs, const char * rhs)
{
    return strcmp(lhs, rhs) == 0;
}

bool IsEqualIgnoreCase(const char * lhs, const char * rhs)
{
#if defined(LINUX) || defined(DARWIN) || defined(WIN_MINGW)
    return strcasecmp(lhs, rhs) == 0;
#elif defined(WIN_MSVC)
    return _stricmp(lhs, rhs) == 0;
#endif
}

bool IsEqual(const wchar_t * lhs, const wchar_t * rhs)
{
    return wcscmp(lhs, rhs) == 0;
}

bool IsEqualIgnoreCase(const wchar_t * lhs, const wchar_t * rhs)
{
#if defined(LINUX) || defined(DARWIN)
    return wcscasecmp(lhs, rhs) == 0;
#elif defined(WIN_MSVC) || defined(WIN_MINGW)
    return _wcsicmp(lhs, rhs) == 0;
#endif
}

std::string ToLower(const std::string & text)
{
    std::string result;
    for (size_t i = 0; i < text.length(); i++)
    {
        result += static_cast<char>(tolower(text[i]));
    }
    return result;
}

std::string ToUpper(const std::string & text)
{
    std::string result;
    for (size_t i = 0; i < text.length(); i++)
    {
        result += static_cast<char>(toupper(text[i]));
    }
    return result;
}

std::string Quote(const std::string & text)
{
    if ((text[0] == '\"') && (text[text.length() - 1] == '\"'))
    {
        return text;
    }
    return "\"" + text + "\"";
}

std::string UnQuote(const std::string & text)
{
    if ((text.length() >= 2) && (text[0] == '\"') && (text[text.length() - 1] == '\"'))
    {
        return text.substr(1, text.length() - 2);
    }
    return text;
}

} // namespace UnitTestCpp