#pragma once

#include <iostream>
#include "unittest-c++/OSAL.h"
#include "unittest-c++/FlagOperators.h"

namespace UnitTestCpp
{

class Secret;

} // namespace UnitTestCpp

// Ensures that there is at least one operator<< in the global namespace.
// See Message& operator<<(...) below for why.
void operator <<(const UnitTestCpp::Secret&, int);

namespace UnitTestCpp
{

#if defined(LINUX) || defined(DARWIN)
enum class ConsoleColor : int
{
    Default = -1,
    Black = 0,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
    ColorMask = 0x07,
    Intensity = 0x08,
    Bold = 0x10,
};
#elif defined(WIN_MSVC) || defined(WIN_MINGW)
enum class ConsoleColor : int
{
    Default = -1,
    Black = 0,
    Red = FOREGROUND_RED,
    Green = FOREGROUND_GREEN,
    Yellow = FOREGROUND_RED | FOREGROUND_GREEN,
    Blue = FOREGROUND_BLUE,
    Magenta = FOREGROUND_RED | FOREGROUND_BLUE,
    Cyan = FOREGROUND_GREEN | FOREGROUND_BLUE,
    White = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    ColorMask = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    Intensity = FOREGROUND_INTENSITY,
    Bold = 0,
};
#endif

DEFINE_FLAG_OPERATORS(ConsoleColor, int);

template<class CharT>
std::basic_ostream<CharT> & operator << (std::basic_ostream<CharT> & stream, ConsoleColor value);

struct _SetForegroundColor
{
    ConsoleColor color;
};

struct _SetBackgroundColor
{
    ConsoleColor color;
};

template<class CharT>
class ConsoleBase
{
private:
    // The type of basic IO manipulators (endl, ends, and flush) for narrow
    // streams.
    typedef std::basic_ostream<CharT> & (* BasicIoManip)(std::basic_ostream<CharT> &);

public:
    static const int InvalidHandle = -1;

    ConsoleBase(int handle = fileno(stdout));
    ConsoleBase(std::basic_ostream<CharT> & stream);

    void SetForegroundColor(ConsoleColor foregroundColor);
    void SetBackgroundColor(ConsoleColor backgroundColor);
    void SetTerminalColor(ConsoleColor foregroundColor = ConsoleColor::Default,
                          ConsoleColor backgroundColor = ConsoleColor::Default);
    void ResetTerminalColor();
    bool ShouldUseColor();

    // Streams a non-pointer _value to this object.
    template <typename T>
    inline ConsoleBase & operator << (const T & val)
    {
        using ::operator <<;
        if (_stream)
            *_stream << val;
        return *this;
    }

    ConsoleBase & operator << (BasicIoManip val)
    {
        if (_stream)
            *_stream << val;
        return *this;
    }

    ConsoleBase & operator << (_SetForegroundColor color);
    ConsoleBase & operator << (_SetBackgroundColor color);

protected:
    std::basic_ostream<CharT> * _stream;
    int _handle;
    ConsoleColor _currentForegroundColor;
    ConsoleColor _currentBackgroundColor;
#if defined(WIN_MSVC) || defined(WIN_MINGW)
    WORD _defaultColorAttributes;
#endif

};

template<class CharT>
inline void ConsoleBase<CharT>::SetForegroundColor(ConsoleColor foregroundColor)
{
    SetTerminalColor(foregroundColor, _currentBackgroundColor);
}

template<class CharT>
inline void ConsoleBase<CharT>::SetBackgroundColor(ConsoleColor backgroundColor)
{
    SetTerminalColor(_currentForegroundColor, backgroundColor);
}

template<class CharT>
inline void ConsoleBase<CharT>::ResetTerminalColor()
{
    SetTerminalColor();
}

template<class CharT>
inline ConsoleBase<CharT> & ConsoleBase<CharT>::operator<<(_SetForegroundColor color)
{
    SetForegroundColor(color.color);
    return *this;
}

template<class CharT>
inline ConsoleBase<CharT> & ConsoleBase<CharT>::operator<<(_SetBackgroundColor color)
{
    SetBackgroundColor(color.color);
    return *this;
}

} // namespace UnitTestCpp

UnitTestCpp::_SetForegroundColor fgcolor(UnitTestCpp::ConsoleColor color);
UnitTestCpp::_SetBackgroundColor bgcolor(UnitTestCpp::ConsoleColor color);

