#pragma once

#include <codecvt>
#include <locale>
#include <string>
#include "unittest-cpp/exports.h"

namespace UnitTestCpp {

UNIT_TEST_CPP_EXPORT std::string Trim(const std::string & value, const std::string & stripChars);

UNIT_TEST_CPP_EXPORT std::string TrimSpaces(const std::string & value);

UNIT_TEST_CPP_EXPORT std::string TrimLeft(const std::string & value, const std::string & stripChars);

UNIT_TEST_CPP_EXPORT std::string TrimSpacesLeft(const std::string & value);

UNIT_TEST_CPP_EXPORT std::string TrimRight(const std::string & value, const std::string & stripChars);

UNIT_TEST_CPP_EXPORT std::string TrimSpacesRight(const std::string & value);

UNIT_TEST_CPP_EXPORT bool IsEqual(const std::string & lhs, const std::string & rhs);

UNIT_TEST_CPP_EXPORT bool IsEqualIgnoreCase(const std::string & lhs, const std::string & rhs);

UNIT_TEST_CPP_EXPORT bool IsEqual(const char * lhs, const char * rhs);

UNIT_TEST_CPP_EXPORT bool IsEqualIgnoreCase(const char * lhs, const char * rhs);

UNIT_TEST_CPP_EXPORT bool IsEqual(const wchar_t * lhs, const wchar_t * rhs);

UNIT_TEST_CPP_EXPORT bool IsEqualIgnoreCase(const wchar_t * lhs, const wchar_t * rhs);

UNIT_TEST_CPP_EXPORT std::string ToLower(const std::string & text);

UNIT_TEST_CPP_EXPORT std::string ToUpper(const std::string & text);

UNIT_TEST_CPP_EXPORT std::string Quote(const std::string & text);

UNIT_TEST_CPP_EXPORT std::string UnQuote(const std::string & text);

inline std::wstring StringToWString(const std::string & value)
{
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;

    return converter.from_bytes(value);
}

inline std::string WStringToString(const std::wstring & value)
{
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;

    return converter.to_bytes(value);
}

inline std::string ToString(const std::string & value)
{
    return value;
}

inline std::string ToString(const std::wstring & value)
{
    return WStringToString(value);
}

inline std::wstring ToWideString(const std::string & value)
{
    return StringToWString(value);
}

inline std::string ToNarrowString(const std::string & value)
{
    return value;
}

} // namespace UnitTestCpp

