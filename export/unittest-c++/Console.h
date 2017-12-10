#pragma once

#include "unittest-c++/ConsoleBase.h"
#if defined(WIN_MSVC)
#elif defined(WIN_MINGW)
#include <unistd.h>
#elif defined(DARWIN)
#include <unistd.h>
#elif defined(LINUX)
#include <unistd.h>
#endif

namespace UnitTestCpp {
#if defined(WIN_MSVC)
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

inline std::basic_ostream<char> * DetermineStream(int handle)
{
#if defined(UNICODE) || defined(_UNICODE)
    switch (handle)
    {
        case STDOUT_FILENO:
            return &std::wcout;
            break;
        case STDERR_FILENO:
            return &std::wcerr;
            break;
        case STDIN_FILENO:
        default:
            std::cerr << "Invalid handle specified, please specify only stdout or stderr handle" << std::endl;
    }
#else
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
#endif
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

inline const wchar_t * GetAnsiColorCode(ConsoleColorType color)
{
	switch (color & ConsoleColor::ColorMask)
	{
	case ConsoleColor::Black:
		return _("0");
	case ConsoleColor::Red:
		return _("1");
	case ConsoleColor::Green:
		return _("2");
	case ConsoleColor::Yellow:
		return _("3");
	case ConsoleColor::Blue:
		return _("4");
	case ConsoleColor::Magenta:
		return _("5");
	case ConsoleColor::Cyan:
		return _("6");
	case ConsoleColor::White:
		return _("7");
	case ConsoleColor::Default:
	case ConsoleColor::Intensity:
	default:
		return nullptr;
	};
}

template<class CharT>
ConsoleBase<CharT>::ConsoleBase(int handle)
    : _stream(DetermineStream(handle))
    , _handle(handle)
    , _currentForegroundColor(ConsoleColor::Default)
    , _currentBackgroundColor(ConsoleColor::Default)
{
}

template<class CharT>
ConsoleBase<CharT>::ConsoleBase(std::basic_ostream<CharT> & stream)
    : _stream(&stream)
    , _handle(DetermineHandle(&stream))
    , _currentForegroundColor(ConsoleColor::Default)
    , _currentBackgroundColor(ConsoleColor::Default)
{
}

template<class CharT>
void ConsoleBase<CharT>::SetTerminalColor(ConsoleColorType foregroundColor, ConsoleColorType backgroundColor)
{
    if (!ShouldUseColor())
        return;
#if defined(UNICODE) || defined(_UNICODE)
	std::wstring command = L"\033[0";
    if (foregroundColor != ConsoleColor::Default)
    {
        if ((foregroundColor & ConsoleColor::Bold) == ConsoleColor::Bold)
        {
            command += L";1";
        }
        if ((foregroundColor & ConsoleColor::Intensity) == ConsoleColor::Intensity)
            command += L";9";
        else
            command += L";3";
        command += GetAnsiColorCode(foregroundColor);
    }
    if (backgroundColor != ConsoleColor::Default)
    {
        if ((backgroundColor & ConsoleColor::Intensity) == ConsoleColor::Intensity)
            command += L";10";
        else
            command += L";4";
        command += GetAnsiColorCode(backgroundColor);
    }
    command += L"m";
#else
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
#endif
	if (_stream)
        *_stream << command;
    _currentForegroundColor = foregroundColor;
    _currentBackgroundColor = backgroundColor;
}

template<class CharT>
bool ConsoleBase<CharT>::ShouldUseColor()
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
        return OSAL::Files::fileno(stdout);
    else if (stream == &std::cerr)
        return OSAL::Files::fileno(stderr);
    return -1;
}

inline const char * GetAnsiColorCode(ConsoleColorType color)
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

template<class CharT>
ConsoleBase<CharT>::ConsoleBase(int handle)
    : _stream(DetermineStream(handle))
    , _handle(handle)
    , _currentForegroundColor(ConsoleColor::Default)
    , _currentBackgroundColor(ConsoleColor::Default)
{
}

template<class CharT>
ConsoleBase<CharT>::ConsoleBase(std::basic_ostream<CharT> & stream)
    : _stream(&stream)
    , _handle(DetermineHandle(&stream))
    , _currentForegroundColor(ConsoleColor::Default)
    , _currentBackgroundColor(ConsoleColor::Default)
{
}

template<class CharT>
void ConsoleBase<CharT>::SetTerminalColor(ConsoleColorType foregroundColor, ConsoleColorType backgroundColor)
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

template<class CharT>
bool ConsoleBase<CharT>::ShouldUseColor()
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

inline const char * GetAnsiColorCode(ConsoleColorType color)
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

template<class CharT>
ConsoleBase<CharT>::ConsoleBase(int handle)
    : _stream(DetermineStream(handle))
    , _handle(handle)
    , _currentForegroundColor(ConsoleColor::Default)
    , _currentBackgroundColor(ConsoleColor::Default)
{
}

template<class CharT>
ConsoleBase<CharT>::ConsoleBase(std::basic_ostream<CharT> & stream)
    : _stream(&stream)
    , _handle(DetermineHandle(&stream))
    , _currentForegroundColor(ConsoleColor::Default)
    , _currentBackgroundColor(ConsoleColor::Default)
{
}

template<class CharT>
void ConsoleBase<CharT>::SetTerminalColor(ConsoleColorType foregroundColor, ConsoleColorType backgroundColor)
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

template<class CharT>
bool ConsoleBase<CharT>::ShouldUseColor()
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

template<class CharT>
ConsoleBase<CharT>::ConsoleBase(int handle)
    : _stream(DetermineStream(handle))
    , _handle(handle)
    , _currentForegroundColor(ConsoleColor::Default)
    , _currentBackgroundColor(ConsoleColor::Default)
{
}

template<class CharT>
ConsoleBase<CharT>::ConsoleBase(std::basic_ostream<CharT> & stream)
    : _stream(&stream)
    , _handle(DetermineHandle(&stream))
    , _currentForegroundColor(ConsoleColor::Default)
    , _currentBackgroundColor(ConsoleColor::Default)
{
}

template<class CharT>
void ConsoleBase<CharT>::SetTerminalColor(ConsoleColor foregroundColor, ConsoleColor backgroundColor)
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

template<class CharT>
bool ConsoleBase<CharT>::ShouldUseColor()
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


typedef ConsoleBase<char> Console;

} // namespace UnitTestCpp