#pragma once

#include <iostream>
#include "unittest-c++/OSAL.h"
#include "unittest-c++/FlagOperators.h"
#include "unittest-c++/Strings.h"

#if defined(WIN_MSVC)
#include <io.h>
#elif defined(WIN_MINGW)
#include <unistd.h>
#elif defined(DARWIN)
#include <unistd.h>
#elif defined(LINUX)
#include <unistd.h>
#endif

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

std::ostream & operator << (std::ostream & stream, ConsoleColor value);

struct _SetForegroundColor
{
    ConsoleColor color;
};

struct _SetBackgroundColor
{
    ConsoleColor color;
};

class Console
{
private:
    // The type of basic IO manipulators (endl, ends, and flush) for narrow
    // streams.
    typedef std::ostream & (* BasicIoManip)(std::ostream &);

public:
    static const int InvalidHandle = -1;

    Console(int handle = _fileno(stdout));
    Console(std::ostream & stream);

    void SetForegroundColor(ConsoleColor foregroundColor);
    void SetBackgroundColor(ConsoleColor backgroundColor);
    void SetTerminalColor(ConsoleColor foregroundColor = ConsoleColor::Default,
                          ConsoleColor backgroundColor = ConsoleColor::Default);
    void ResetTerminalColor();
    bool ShouldUseColor();

    // Streams a non-pointer _value to this object.
    template <typename T>
    inline Console & operator << (const T & val)
    {
        using ::operator <<;
        if (_stream)
            *_stream << val;
        return *this;
    }

    Console & operator << (BasicIoManip val)
    {
        if (_stream)
            *_stream << val;
        return *this;
    }

    Console & operator << (_SetForegroundColor color);
    Console & operator << (_SetBackgroundColor color);

protected:
    std::ostream * _stream;
    int _handle;
    ConsoleColor _currentForegroundColor;
    ConsoleColor _currentBackgroundColor;
#if defined(WIN_MSVC) || defined(WIN_MINGW)
    WORD _defaultColorAttributes;
#endif

};

inline void Console::SetForegroundColor(ConsoleColor foregroundColor)
{
    SetTerminalColor(foregroundColor, _currentBackgroundColor);
}

inline void Console::SetBackgroundColor(ConsoleColor backgroundColor)
{
    SetTerminalColor(_currentForegroundColor, backgroundColor);
}

inline void Console::ResetTerminalColor()
{
    SetTerminalColor();
}

inline Console & Console::operator<<(_SetForegroundColor color)
{
    SetForegroundColor(color.color);
    return *this;
}

inline Console & Console::operator<<(_SetBackgroundColor color)
{
    SetBackgroundColor(color.color);
    return *this;
}

} // namespace UnitTestCpp

inline UnitTestCpp::_SetForegroundColor fgcolor(UnitTestCpp::ConsoleColor color)
{
    return{ color };
}

inline UnitTestCpp::_SetBackgroundColor bgcolor(UnitTestCpp::ConsoleColor color)
{
    return{ color };
}

namespace UnitTestCpp {

#if defined(WIN_MSVC)
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

inline std::ostream * DetermineStream(int handle)
{
    switch (handle)
    {
    case STDOUT_FILENO:
        return &std::cout;
        break;
    case STDERR_FILENO:
        return &std::cerr;
        break;
    case STDIN_FILENO:
    default:
        std::cerr << "Invalid handle specified, please specify only stdout or stderr handle" << std::endl;
    }
    return nullptr;
}

inline int DetermineHandle(std::ostream * stream)
{
    if (stream == &std::cout)
        return _fileno(stdout);
    else if (stream == &std::cerr)
        return _fileno(stderr);
    return -1;
}

inline const char * GetAnsiColorCode(ConsoleColor color)
{
    switch (color & ConsoleColor::ColorMask)
    {
    case ConsoleColor::Black:
        return "0";
    case ConsoleColor::Red:
        return "1";
    case ConsoleColor::Green:
        return "2";
    case ConsoleColor::Yellow:
        return "3";
    case ConsoleColor::Blue:
        return "4";
    case ConsoleColor::Magenta:
        return "5";
    case ConsoleColor::Cyan:
        return "6";
    case ConsoleColor::White:
        return "7";
    case ConsoleColor::Default:
    case ConsoleColor::Intensity:
    default:
        return nullptr;
    };
}

inline Console::Console(int handle)
    : _stream(DetermineStream(handle))
    , _handle(handle)
    , _currentForegroundColor(ConsoleColor::Default)
    , _currentBackgroundColor(ConsoleColor::Default)
{
}

inline Console::Console(std::ostream & stream)
    : _stream(&stream)
    , _handle(DetermineHandle(&stream))
    , _currentForegroundColor(ConsoleColor::Default)
    , _currentBackgroundColor(ConsoleColor::Default)
{
}

inline void Console::SetTerminalColor(ConsoleColor foregroundColor, ConsoleColor backgroundColor)
{
    if (!ShouldUseColor())
        return;
    std::string command = "\033[0";
    if (foregroundColor != ConsoleColor::Default)
    {
        if ((foregroundColor & ConsoleColor::Bold) == ConsoleColor::Bold)
        {
            command += ";1";
        }
        if ((foregroundColor & ConsoleColor::Intensity) == ConsoleColor::Intensity)
            command += ";9";
        else
            command += ";3";
        command += GetAnsiColorCode(foregroundColor);
    }
    if (backgroundColor != ConsoleColor::Default)
    {
        if ((backgroundColor & ConsoleColor::Intensity) == ConsoleColor::Intensity)
            command += ";10";
        else
            command += ";4";
        command += GetAnsiColorCode(backgroundColor);
    }
    command += "m";
    if (_stream)
        *_stream << command;
    _currentForegroundColor = foregroundColor;
    _currentBackgroundColor = backgroundColor;
}

inline const char * getenv(const char * name)
{
    static char buffer[4096];
    if (::GetEnvironmentVariableA(name, buffer, sizeof(buffer)) == 0)
    {
        return nullptr;
    }
    return buffer;
}

inline bool Console::ShouldUseColor()
{
    if (_handle == InvalidHandle)
        return false;
    if (!_isatty(_handle))
        return false;

    const char * termSetting = getenv("TERM");
    if (!termSetting)
        return false;
    std::string term = termSetting;
    const bool term_supports_color =
    (term == "xterm") ||
    (term == "xterm-color") ||
    (term == "xterm-256color") ||
    (term == "screen") ||
    (term == "screen-256color") ||
    (term == "linux") ||
    (term == "cygwin");
    return term_supports_color;
}
#elif defined(WIN_MINGW)
inline std::ostream * DetermineStream(int handle)
{
    switch (handle)
    {
        case STDOUT_FILENO:
            return &std::cout;
            break;
        case STDERR_FILENO:
            return &std::cerr;
            break;
        case STDIN_FILENO:
        default:
            std::cerr << "Invalid handle specified, please specify only stdout or stderr handle" << std::endl;
    }
    return nullptr;
}

inline int DetermineHandle(std::ostream * stream)
{
    if (stream == &std::cout)
        return fileno(stdout);
    else if (stream == &std::cerr)
        return fileno(stderr);
    return -1;
}

inline const char * GetAnsiColorCode(ConsoleColor color)
{
    switch (color & ConsoleColor::ColorMask)
    {
        case ConsoleColor::Black:
            return "0";
        case ConsoleColor::Red:
            return "1";
        case ConsoleColor::Green:
            return "2";
        case ConsoleColor::Yellow:
            return "3";
        case ConsoleColor::Blue:
            return "4";
        case ConsoleColor::Magenta:
            return "5";
        case ConsoleColor::Cyan:
            return "6";
        case ConsoleColor::White:
            return "7";
        default:
            return nullptr;
    };
}

inline Console::Console(int handle)
    : _stream(DetermineStream(handle))
    , _handle(handle)
    , _currentForegroundColor(ConsoleColor::Default)
    , _currentBackgroundColor(ConsoleColor::Default)
{
}

inline Console::Console(std::ostream & stream)
    : _stream(&stream)
    , _handle(DetermineHandle(&stream))
    , _currentForegroundColor(ConsoleColor::Default)
    , _currentBackgroundColor(ConsoleColor::Default)
{
}

inline void Console::SetTerminalColor(ConsoleColor foregroundColor, ConsoleColor backgroundColor)
{
    if (!ShouldUseColor())
        return;
    std::string command = "\033[0";
    if (foregroundColor != ConsoleColor::Default)
    {
        if ((foregroundColor & ConsoleColor::Bold) == ConsoleColor::Bold)
        {
            command += ";1";
        }
        if ((foregroundColor & ConsoleColor::Intensity) == ConsoleColor::Intensity)
            command += ";9";
        else
            command += ";3";
        command += GetAnsiColorCode(foregroundColor);
    }
    if (backgroundColor != ConsoleColor::Default)
    {
        if ((backgroundColor & ConsoleColor::Intensity) == ConsoleColor::Intensity)
            command += ";10";
        else
            command += ";4";
        command += GetAnsiColorCode(backgroundColor);
    }
    command += "m";
    if (_stream)
        *_stream << command;
    _currentForegroundColor = foregroundColor;
    _currentBackgroundColor = backgroundColor;
}

inline bool Console::ShouldUseColor()
{
    if (_handle == InvalidHandle)
        return false;
    if (!isatty(_handle))
        return false;

    const char * termSetting = getenv("TERM");
    if (!termSetting)
        return false;
    std::string term = termSetting;
    const bool term_supports_color =
    (term == "xterm") ||
    (term == "xterm-color") ||
    (term == "xterm-256color") ||
    (term == "screen") ||
    (term == "screen-256color") ||
    (term == "linux") ||
    (term == "cygwin");
    return term_supports_color;
}
#elif defined(DARWIN)
inline std::ostream * DetermineStream(int handle)
{
    switch (handle)
    {
        case STDOUT_FILENO:
            return &std::cout;
            break;
        case STDERR_FILENO:
            return &std::cerr;
            break;
        case STDIN_FILENO:
        default:
            std::cerr << "Invalid handle specified, please specify only stdout or stderr handle" << std::endl;
    }
    return nullptr;
}

inline int DetermineHandle(std::ostream * stream)
{
    if (stream == &std::cout)
        return OSAL::Files::fileno(stdout);
    else if (stream == &std::cerr)
        return OSAL::Files::fileno(stderr);
    return -1;
}

inline const char * GetAnsiColorCode(ConsoleColor color)
{
    switch (color & ConsoleColor::ColorMask)
    {
        case ConsoleColor::Black:
            return "0";
        case ConsoleColor::Red:
            return "1";
        case ConsoleColor::Green:
            return "2";
        case ConsoleColor::Yellow:
            return "3";
        case ConsoleColor::Blue:
            return "4";
        case ConsoleColor::Magenta:
            return "5";
        case ConsoleColor::Cyan:
            return "6";
        case ConsoleColor::White:
            return "7";
        default:
            return nullptr;
    };
}

inline Console::Console(int handle)
    : _stream(DetermineStream(handle))
    , _handle(handle)
    , _currentForegroundColor(ConsoleColor::Default)
    , _currentBackgroundColor(ConsoleColor::Default)
{
}

inline Console::Console(std::ostream & stream)
    : _stream(&stream)
    , _handle(DetermineHandle(&stream))
    , _currentForegroundColor(ConsoleColor::Default)
    , _currentBackgroundColor(ConsoleColor::Default)
{
}

inline void Console::SetTerminalColor(ConsoleColor foregroundColor, ConsoleColor backgroundColor)
{
    if (!ShouldUseColor())
        return;
    std::string command = "\033[0";
    if (foregroundColor != ConsoleColor::Default)
    {
        if ((foregroundColor & ConsoleColor::Bold) == ConsoleColor::Bold)
        {
            command += ";1";
        }
        if ((foregroundColor & ConsoleColor::Intensity) == ConsoleColor::Intensity)
            command += ";9";
        else
            command += ";3";
        command += GetAnsiColorCode(foregroundColor);
    }
    if (backgroundColor != ConsoleColor::Default)
    {
        if ((backgroundColor & ConsoleColor::Intensity) == ConsoleColor::Intensity)
            command += ";10";
        else
            command += ";4";
        command += GetAnsiColorCode(backgroundColor);
    }
    command += "m";
    if (_stream)
        *_stream << command;
    _currentForegroundColor = foregroundColor;
    _currentBackgroundColor = backgroundColor;
}

inline bool Console::ShouldUseColor()
{
    if (_handle == InvalidHandle)
        return false;
    if (!OSAL::Files::isatty(_handle))
        return false;

    const char * termSetting = OSAL::System::getenv("TERM");
    if (!termSetting)
        return false;
    std::string term = termSetting;
    const bool term_supports_color =
        (term == "xterm") ||
        (term == "xterm-color") ||
        (term == "xterm-256color") ||
        (term == "screen") ||
        (term == "screen-256color") ||
        (term == "linux") ||
        (term == "cygwin");
    return term_supports_color;
}
#elif defined(LINUX)

inline std::ostream * DetermineStream(int handle)
{
    switch (handle)
    {
        case STDOUT_FILENO:
            return &std::cout;
            break;
        case STDERR_FILENO:
            return &std::cerr;
            break;
        case STDIN_FILENO:
        default:
            std::cerr << "Invalid handle specified, please specify only stdout or stderr handle" << std::endl;
    }
    return nullptr;
}

inline int DetermineHandle(std::ostream * stream)
{
    if (stream == &std::cout)
        return fileno(stdout);
    else if (stream == &std::cerr)
        return fileno(stderr);
    return -1;
}

inline const char * GetAnsiColorCode(ConsoleColor color)
{
    switch (color & ConsoleColor::ColorMask)
    {
        case ConsoleColor::Black:
            return "0";
        case ConsoleColor::Red:
            return "1";
        case ConsoleColor::Green:
            return "2";
        case ConsoleColor::Yellow:
            return "3";
        case ConsoleColor::Blue:
            return "4";
        case ConsoleColor::Magenta:
            return "5";
        case ConsoleColor::Cyan:
            return "6";
        case ConsoleColor::White:
            return "7";
        default:
            return nullptr;
    };
}

inline Console::Console(int handle)
    : _stream(DetermineStream(handle))
    , _handle(handle)
    , _currentForegroundColor(ConsoleColor::Default)
    , _currentBackgroundColor(ConsoleColor::Default)
{
}

inline Console::Console(std::ostream & stream)
    : _stream(&stream)
    , _handle(DetermineHandle(&stream))
    , _currentForegroundColor(ConsoleColor::Default)
    , _currentBackgroundColor(ConsoleColor::Default)
{
}

inline void Console::SetTerminalColor(ConsoleColor foregroundColor, ConsoleColor backgroundColor)
{
    if (!ShouldUseColor())
        return;
    std::string command = "\033[0";
    if (foregroundColor != ConsoleColor::Default)
    {
        if ((foregroundColor & ConsoleColor::Bold) == ConsoleColor::Bold)
        {
            command += ";1";
        }
        if ((foregroundColor & ConsoleColor::Intensity) == ConsoleColor::Intensity)
            command += ";9";
        else
            command += ";3";
        command += GetAnsiColorCode(foregroundColor);
    }
    if (backgroundColor != ConsoleColor::Default)
    {
        if ((backgroundColor & ConsoleColor::Intensity) == ConsoleColor::Intensity)
            command += ";10";
        else
            command += ";4";
        command += GetAnsiColorCode(backgroundColor);
    }
    command += "m";
    if (_stream)
        *_stream << command;
    _currentForegroundColor = foregroundColor;
    _currentBackgroundColor = backgroundColor;
}

inline bool Console::ShouldUseColor()
{
    if (_handle == InvalidHandle)
        return false;
    if (!isatty(_handle))
        return false;

    const char * termSetting = getenv("TERM");
    if (!termSetting)
        return false;
    std::string term = termSetting;
    const bool term_supports_color =
        (term == "xterm") ||
        (term == "xterm-color") ||
        (term == "xterm-256color") ||
        (term == "screen") ||
        (term == "screen-256color") ||
        (term == "linux") ||
        (term == "cygwin");
    return term_supports_color;
}

#endif

} // namespace UnitTestCpp