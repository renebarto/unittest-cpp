#pragma once

#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <cstring>
#include "unittest-c++/PlatformDefines.h"
#include "unittest-c++/Unused.h"
#include "unittest-c++/Strings.h"

#if defined(WIN_MSVC)
#define WARNING_PUSH __pragma(warning(push))
#define WARNING_PUSH_N(level) __pragma(warning(push, level))
#define WARNING_DISABLE(num) __pragma(warning(disable: num))
#define WARNING_DEFAULT(num) __pragma(warning(default: num))
#define WARNING_POP __pragma(warning(pop))

WARNING_PUSH
WARNING_DISABLE(4365)
WARNING_DISABLE(4514)
WARNING_DISABLE(4574)
WARNING_DISABLE(4668)
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#undef min
#undef max
WARNING_POP

#define SL32(x) x
#define UL32(x) x##u
#define SL64(x) x##i64
#define UL64(x) x##ui64

WARNING_PUSH
WARNING_DISABLE(4091)
#include <Dbghelp.h>
WARNING_POP
namespace UnitTestCpp {
inline std::string DemangleName(const std::string & mangledName)
{
    std::string result;
    SymSetOptions(SYMOPT_UNDNAME);
    char demangledName[_MAX_PATH];
    size_t size = UnDecorateSymbolName(ToNarrowString(mangledName).c_str() + 1, demangledName, _MAX_PATH, UNDNAME_32_BIT_DECODE | UNDNAME_NAME_ONLY);
    if (size > 0)
    {
        result = ToString(demangledName);
    }
    return result;
}

WARNING_PUSH
WARNING_DISABLE(4100)
template <class T>
inline std::string TypeName(const T & x)
{
    std::string name = ToString(typeid(x).name());
    const std::string classPrefix = "class ";
    size_t pos = name.find(classPrefix, 0);
    if (pos == 0)
        name = name.substr(classPrefix.length());
    return name;
}
WARNING_POP
} // namespace UnitTestCpp

#elif defined(WIN_MINGW)
#define WARNING_PUSH
#define WARNING_PUSH_N(level)
#define WARNING_DISABLE(num)
#define WARNING_DEFAULT(num)
#define WARNING_POP

#define SL32(x) x
#define UL32(x) x##U
#define SL64(x) x##LL
#define UL64(x) x##ULL

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include <cxxabi.h>
namespace UnitTestCpp {
inline std::string DemangleName(const std::string & mangledName)
{
    std::string result;
    int status;
    char * demangledName = abi::__cxa_demangle(mangledName.c_str(), 0, 0, &status);
    if (status == 0)
    {
        result = demangledName;
        std::free(demangledName);
    }
    return result;
}

template <class T>
inline std::string TypeName(const T & x)
{
    return DemangleName(typeid(x).name());
}
} // namespace UnitTestCpp

#elif defined(DARWIN)
#define WARNING_PUSH _Pragma("clang diagnostic push")
#define WARNING_PUSH_N(level) WARNING_PUSH
#define WARNING_DISABLE_UNUSED_PARAMETER _Pragma("clang diagnostic ignored \"-Wunused-parameter\"")
#define WARNING_DISABLE(num)
#define WARNING_DEFAULT(num)
#define WARNING_POP _Pragma("clang diagnostic pop")

#include <cxxabi.h>

namespace UnitTestCpp {
inline std::string DemangleName(const std::string & mangledName)
{
    std::string result;
    int status;
    char * demangledName = abi::__cxa_demangle(mangledName.c_str(), 0, 0, &status);
    if (status == 0)
    {
        result = demangledName;
        std::free(demangledName);
    }
    return result;
}

template <class T>
inline std::string TypeName(const T & x)
{
    return DemangleName(typeid(x).name());
}
} // namespace UnitTestCpp

#elif defined(LINUX)
#define WARNING_PUSH
#define WARNING_PUSH_N(level)
#define WARNING_DISABLE(num)
#define WARNING_DEFAULT(num)
#define WARNING_POP

#define SL32(x) x
#define UL32(x) x##U
#define SL64(x) x##LL
#define UL64(x) x##ULL

#include <cxxabi.h>

namespace UnitTestCpp {
inline std::string DemangleName(const std::string & mangledName)
{
    std::string result;
    int status;
    char * demangledName = abi::__cxa_demangle(mangledName.c_str(), 0, 0, &status);
    if (status == 0)
    {
        result = ToString(demangledName);
        std::free(demangledName);
    }
    return result;
}

template<class T>
inline std::string TypeName(const T & x)
{
    return DemangleName(typeid(x).name());
}
} // namespace UnitTestCpp

#endif

namespace UnitTestCpp {

size_t NextPowerOfTwo(size_t value);

class UNIT_TEST_CPP_EXPORT BaseException : public std::exception
{
protected:
    std::string message;
    const std::exception * innerException;
    mutable std::string whatMessage;
public:
    BaseException()
        : message()
          , innerException(nullptr)
    {
    }
    BaseException(const BaseException & innerException)
        : message()
          , innerException(&innerException)
    {
    }
    // Not a copy constructor!!
    BaseException(const std::exception & innerException)
        : message()
          , innerException(&innerException)
    {
    }
    BaseException(const std::string & message)
        : message(message)
          , innerException(nullptr)
    {
    }
    BaseException(const std::string & message, const std::exception & innerException)
        : message(message)
          , innerException(&innerException)
    {
    }
    ~BaseException() noexcept
    {
    }
    BaseException & operator = (const BaseException & other)
    {
        message = other.message;
        innerException = other.innerException;
        return *this;
    }
    const char* what() const throw()
    {
        whatMessage = ToNarrowString(FormatMessage());
        return whatMessage.c_str();
    }
    virtual std::string BuildMessage() const = 0;

    const std::string & GetMessage() const
    {
        return message;
    }
    void SetMessage(const std::string & value)
    {
        message = value;
    }
private:
    std::string FormatMessage() const
    {
        std::ostringstream stream;
        stream << TypeName(*this);
        std::string buildMessage = BuildMessage();
        if (!buildMessage.empty())
            stream << " - " << BuildMessage();
        if (!message.empty())
            stream << ": " << message;
        if (innerException)
        {
            stream << std::endl << "-->" << innerException->what();
        }
        return stream.str();
    }
};

class UNIT_TEST_CPP_EXPORT Exception : public BaseException
{
protected:
    char const * functionName;
    char const * fileName;
    int line;
public:
    Exception() = delete;
    Exception(char const * functionName, char const * fileName, int line)
        : BaseException()
          , functionName(functionName)
          , fileName(fileName)
          , line(line)
    {
    }
    Exception(char const * functionName, char const * fileName, int line, const std::string & message)
        : BaseException(message)
          , functionName(functionName)
          , fileName(fileName)
          , line(line)
    {
    }
    Exception(char const * functionName, char const * fileName, int line, const std::string & message, const std::exception & innerException)
        : BaseException(message, innerException)
          , functionName(functionName)
          , fileName(fileName)
          , line(line)
    {
    }
    ~Exception()
    {
    }
    virtual std::string BuildMessage() const override
    {
        std::ostringstream stream;
        if (functionName != nullptr)
            stream << "in " << functionName;
        if (fileName != nullptr)
        {
            stream << " [" << fileName;
            if (line > 0)
                stream << ":" << line;
            stream << "]";
        }
        return stream.str();
    }
};

class UNIT_TEST_CPP_EXPORT SystemError : public Exception
{
protected:
    int errorCode;
public:
    SystemError(char const * functionName, char const * fileName, int line, int errorCode)
        : Exception(functionName, fileName, line)
          , errorCode(errorCode)
    {
    }
    SystemError(char const * functionName, char const * fileName, int line, int errorCode, std::string message)
        : Exception(functionName, fileName, line, message)
          , errorCode(errorCode)
    {
    }
    ~SystemError()
    {
    }
    int GetErrorCode() const
    {
        return errorCode;
    }
    virtual std::string BuildMessage() const override
    {
        std::ostringstream stream;
		stream << Exception::BuildMessage() << " errno=" << errorCode
			<< " (0x" << std::hex << std::setw(8) << std::setfill('0') << errorCode << "): \""
			<< strerror(errorCode) << "\"";
        return stream.str();
    }
};

class UNIT_TEST_CPP_EXPORT RuntimeError : public Exception
{
public:
    RuntimeError(char const * functionName, char const * fileName, int line, std::string message)
        : Exception(functionName, fileName, line, message)
    {
    }
    ~RuntimeError()
    {
    }
};

class UNIT_TEST_CPP_EXPORT ArgumentException : public Exception
{
protected:
    std::string argument;
public:
    ArgumentException(char const * functionName, char const * fileName, int line,
                      std::string argument)
        : Exception(functionName, fileName, line)
          , argument(argument)
    {
    }
    ArgumentException(char const * functionName, char const * fileName, int line,
                      std::string argument, std::string message)
        : Exception(functionName, fileName, line, message)
          , argument(argument)
    {
    }
    ~ArgumentException()
    {
    }
    virtual std::string BuildMessage() const
    {
        std::ostringstream stream;
        stream << Exception::BuildMessage();
        if (!argument.empty())
            stream << ": argument: " << argument;
        return stream.str();
    }
};

class UNIT_TEST_CPP_EXPORT ArgumentNullException : public ArgumentException
{
public:
    ArgumentNullException(char const * functionName, char const * fileName, int line,
                          std::string argument) :
        ArgumentException(functionName, fileName, line, argument)
    {
    }
    ~ArgumentNullException() throw ()
    {
    }
    virtual std::string BuildMessage() const
    {
        std::ostringstream stream;
        stream << Exception::BuildMessage();
        if (!argument.empty())
            stream << ": argument null: " << argument;
        return stream.str();
    }
};

inline void ThrowOnError(const char * functionName, const char * fileName, int line, int errorCode)
{
    if (errorCode != 0)
    {
        throw SystemError(functionName, fileName, line, errorCode);
    }
}

} // namespace UnitTestCpp

