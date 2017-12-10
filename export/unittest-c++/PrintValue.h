#pragma once

#include <type_traits>
#include <typeinfo>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <unittest-c++/OSAL.h>
#include <unittest-c++/exports.h>

namespace UnitTestCpp
{
//
// This is tricky stuff to determine whether a type has a stream insertion operator
// ostream & operator << (ostream &, T)
namespace has_insertion_operator_impl
{
    typedef char no;
    typedef char yes[2];

    struct any_t
    {
        template<typename T> any_t(T const&);
    };

    no operator<<(std::basic_ostream<char> const&, any_t const&);

    yes& test(std::basic_ostream<char>&);
    no test(no);

    template<typename T>
    struct has_insertion_operator {
        static std::basic_ostream<char> &s;
        static const T &t;
        static bool const value = sizeof(test(s << t)) == sizeof(yes);
    };
}

template<typename T>
struct has_insertion_operator : has_insertion_operator_impl::has_insertion_operator<T>
{
};

template<typename To>
inline To ImplicitCast_(To x) { return x; }

// ImplicitlyConvertible<From, To>::_value is a compile-time bool
// constant that's true iff type From can be implicitly converted to
// type To.
template <typename From, typename To>
class ImplicitlyConvertible
{
private:
    // We need the following helper functions only for their types.
    // They have no implementations.

    // MakeFrom() is an expression whose type is From.  We cannot simply
    // use From(), as the type From may not have a public default
    // constructor.
    static From MakeFrom();

    // These two functions are overloaded.  Given an expression
    // Helper(x), the compiler will pick the first version if x can be
    // implicitly converted to type To; otherwise it will pick the
    // second version.
    //
    // The first version returns a _value of size 1, and the second
    // version returns a _value of size 2.  Therefore, by checking the
    // size of Helper(x), which can be done at compile time, we can tell
    // which version of Helper() is used, and hence whether x can be
    // implicitly converted to type To.
    static char Helper(To);
    static char (&Helper(...))[2];  // NOLINT

    // We have to put the 'public' section after the 'private' section,
    // or MSVC refuses to compile the code.
public:
    static const bool value = sizeof(Helper(ImplicitlyConvertible::MakeFrom())) == 1;
};
template <typename From, typename To>
const bool ImplicitlyConvertible<From, To>::value;

inline bool IsTrue(bool condition) { return condition; }

template <typename T>
std::string PrintToString(const T& value);

template <typename T>
void DefaultPrintNonContainerTo(const T& value, std::basic_ostream<char> & stream)
{
    stream << value;
}

std::string WideStringToUtf8(const std::wstring & value);

template <typename T>
void UniversalPrint(const T& value, std::basic_ostream<char> & stream);

WARNING_PUSH
WARNING_DISABLE(4242)
typedef int IsContainer;
template <class C>
IsContainer IsContainerTest(int /* dummy */,
                            typename C::iterator * /* it */ = nullptr,
                            typename C::const_iterator * /* const_it */ = nullptr)
{
    return 0;
}

typedef char IsNotContainer;
template <class C>
IsNotContainer IsContainerTest(long /* dummy */) { return '\0'; }

// Used to print an STL-style container when the user doesn't define
// a PrintTo() for it.
#if defined(DARWIN)
WARNING_DISABLE_UNUSED_PARAMETER
#endif
template <typename C>
void DefaultPrintTo(IsContainer /* dummy */,
                    std::false_type isClass,
                    std::false_type isEnum,
                    std::false_type isPointer,
                    const C& container,
                    std::basic_ostream<char> & stream)
{
    const size_t MaxCount = 32;  // The maximum number of elements to print.
    stream << '{';
    size_t count = 0;
    for (typename C::const_iterator it = container.begin(); it != container.end(); ++it, ++count)
    {
        if (count > 0)
        {
            stream << ',';
            if (count == MaxCount)
            {  // Enough has been printed.
                stream << " ...";
                break;
            }
        }
        stream << ' ';
        // We cannot call PrintTo(*it, stream) here as PrintTo() doesn't
        // handle *it being a native array.
        UniversalPrint(*it, stream);
    }

    if (count > 0)
    {
        stream << ' ';
    }
    stream << '}';
}
WARNING_POP

//template <typename T>
//void DefaultPrintTo(IsContainer /* dummy */,
//                    std::false_type isClass,
//                    std::false_type isEnum,
//                    std::true_type isPointer,
//                    const T& _value,
//                    std::basic_ostream<char> & stream)
//{
//    stream << "container pointer";
//}
//
//template <typename T>
//void DefaultPrintTo(IsContainer /* dummy */,
//                    std::false_type isClass,
//                    std::true_type isEnum,
//                    std::false_type isPointer,
//                    const T& _value,
//                    std::basic_ostream<char> & stream)
//{
//    stream << "container enum";
//}
//
//template <typename T>
//void DefaultPrintTo(IsContainer /* dummy */,
//                    std::false_type isClass,
//                    std::true_type isEnum,
//                    std::true_type isPointer,
//                    const T& _value,
//                    std::basic_ostream<char> & stream)
//{
//    stream << "container enum pointer";
//}

template <typename T>
void DefaultPrintTo(IsContainer /* dummy */,
                    std::true_type UNUSED(isClass),
                    std::false_type UNUSED(isEnum),
                    std::false_type UNUSED(isPointer),
                    const T& value,
                    std::basic_ostream<char> & stream)
{
    for (auto x : value)
        UniversalPrint(x, stream);
}

//template <typename T>
//void DefaultPrintTo(IsContainer /* dummy */,
//                    std::true_type isClass,
//                    std::false_type isEnum,
//                    std::true_type isPointer,
//                    const T& _value,
//                    std::basic_ostream<char> & stream)
//{
//    stream << "container class pointer";
//}
//
//template <typename T>
//void DefaultPrintTo(IsContainer /* dummy */,
//                    std::true_type isClass,
//                    std::true_type isEnum,
//                    std::false_type isPointer,
//                    const T& _value,
//                    std::basic_ostream<char> & stream)
//{
//    stream << "container enum class";
//}
//
//template <typename T>
//void DefaultPrintTo(IsContainer /* dummy */,
//                    std::true_type isClass,
//                    std::true_type isEnum,
//                    std::true_type isPointer,
//                    const T& _value,
//                    std::basic_ostream<char> & stream)
//{
//    stream << "container enum class pointer";
//}

// Normal _value
template <typename T>
void DefaultPrintTo(IsNotContainer /* dummy */,
                    std::false_type UNUSED(isClass),
                    std::false_type UNUSED(isEnum),
                    std::false_type UNUSED(isPointer),
                    const T& value,
                    std::basic_ostream<char> & stream)
{
    DefaultPrintNonContainerTo(value, stream);
}

// Normal _value pointer
template <typename T>
void DefaultPrintTo(IsNotContainer /* dummy */,
                    std::false_type,
                    std::false_type,
                    std::true_type,
                    const T& value,
                    std::basic_ostream<char> & stream)
{
    if (value == nullptr)
    {
        stream << "nullptr";
    }
    else
    {
        // C++ doesn't allow casting from a function pointer to any object
        // pointer.
        //
        // IsTrue() silences warnings: "Condition is always true",
        // "unreachable code".
        if (IsTrue(ImplicitlyConvertible<T*, const void*>::value))
        {
            // T is not a function type.  We just call << to print p,
            // relying on ADL to pick up user-defined << for their pointer
            // types, if any.
            stream << value;
        }
        else
        {
            // T is a function type, so 'stream << p' doesn't do what we want
            // (it just prints p as bool).  We want to print p as a const
            // void*.  However, we cannot cast it to const void* directly,
            // even using reinterpret_cast, as earlier versions of gcc
            // (e.g. 3.4.5) cannot compile the cast when p is a function
            // pointer.  Casting to UInt64 first solves the problem.
            stream << reinterpret_cast<const void*>(reinterpret_cast<uint64_t>(value));
        }
    }
}

// Enum _value
template <typename T>
typename std::enable_if<has_insertion_operator<T>::value, void>::type
DefaultPrintTo(IsNotContainer /* dummy */,
               std::false_type UNUSED(isClass),
               std::true_type UNUSED(isEnum),
               std::false_type UNUSED(isPointer),
               const T& value,
               std::basic_ostream<char> & stream)
{
    stream << value;
}

// Enum _value
template <typename T>
typename std::enable_if<!has_insertion_operator<T>::value, void>::type
DefaultPrintTo(IsNotContainer /* dummy */,
               std::false_type UNUSED(isClass),
               std::true_type UNUSED(isEnum),
               std::false_type UNUSED(isPointer),
               const T& value,
               std::basic_ostream<char> & stream)
{
    stream << (int)value;
}

//// Enum pointer
//template <typename T>
//void DefaultPrintTo(IsNotContainer /* dummy */,
//                    std::false_type isClass,
//                    std::true_type isEnum,
//                    std::true_type isPointer,
//                    const T& _value,
//                    std::basic_ostream<char> & stream)
//{
//    stream << "enum " << (int)*_value;
//}

// Class or Struct _value
template <typename T>
typename std::enable_if<has_insertion_operator<T>::value, void>::type
DefaultPrintTo(IsNotContainer /* dummy */,
               std::true_type,
               std::false_type,
               std::false_type,
               const T& value,
               std::basic_ostream<char> & stream)
{
    stream << value;
}

// Class or Struct _value
template <typename T>
typename std::enable_if<!has_insertion_operator<T>::value, void>::type
DefaultPrintTo(IsNotContainer /* dummy */,
               std::true_type UNUSED(isClass),
               std::false_type UNUSED(isEnum),
               std::false_type UNUSED(isPointer),
               const T& UNUSED(value),
               std::basic_ostream<char> & stream)
{
#if defined(WIN_MSVC)
    stream << typeid(T).name();
#else
    stream << DemangleName(typeid(T).name());
#endif
}

//// Class or Struct value
//template <typename T>
//void DefaultPrintTo(IsNotContainer /* dummy */,
//                    std::true_type isClass,
//                    std::false_type isEnum,
//                    std::true_type isPointer,
//                    const T& _value,
//                    std::basic_ostream<char> & stream)
//{
//    stream << "class pointer " << ConvertTypeName(typeid(T).name());
//}
//
//template <typename T>
//void DefaultPrintTo(IsNotContainer /* dummy */,
//                    std::true_type isClass,
//                    std::true_type isEnum,
//                    std::false_type isPointer,
//                    const T& _value,
//                    std::basic_ostream<char> & stream)
//{
//    stream << "class enum " << ConvertTypeName(typeid(T).name());
//}
//
//template <typename T>
//void DefaultPrintTo(IsNotContainer /* dummy */,
//                    std::true_type isClass,
//                    std::true_type isEnum,
//                    std::true_type isPointer,
//                    const T& _value,
//                    std::basic_ostream<char> & stream)
//{
//    stream << "class enum pointer " << ConvertTypeName(typeid(T).name());
//}

template <typename T>
void PrintTo(const T& value, std::basic_ostream<char> & stream)
{
    DefaultPrintTo(IsContainerTest<T>(0), std::is_class<T>(), std::is_enum<T>(), std::is_pointer<T>(), value, stream);
}

UNIT_TEST_CPP_EXPORT void PrintTo(unsigned char c, std::basic_ostream<char> & stream);
UNIT_TEST_CPP_EXPORT void PrintTo(signed char c, std::basic_ostream<char> & stream);
/*UNIT_TEST_CPP_EXPORT*/ inline void PrintTo(char c, std::basic_ostream<char> & stream)
{
    PrintTo(static_cast<unsigned char>(c), stream);
}

UNIT_TEST_CPP_EXPORT void PrintTo(wchar_t wc, std::basic_ostream<char> & stream);

/*UNIT_TEST_CPP_EXPORT*/ inline void PrintTo(bool x, std::basic_ostream<char> & stream)
{
    stream << (x ? "true" : "false");
}

UNIT_TEST_CPP_EXPORT void PrintTo(const char* s, std::basic_ostream<char> & stream);
/*UNIT_TEST_CPP_EXPORT*/ inline void PrintTo(char* s, std::basic_ostream<char> & stream)
{
    PrintTo(ImplicitCast_<const char*>(s), stream);
}

// signed/unsigned char is often used for representing binary data, so
// we print pointers to it as void* to be safe.
/*UNIT_TEST_CPP_EXPORT*/ inline void PrintTo(const signed char * s, std::basic_ostream<char> & stream)
{
    PrintTo(ImplicitCast_<const void *>(s), stream);
}
/*UNIT_TEST_CPP_EXPORT*/ inline void PrintTo(signed char * s, std::basic_ostream<char> & stream)
{
    PrintTo(ImplicitCast_<const void *>(s), stream);
}
/*UNIT_TEST_CPP_EXPORT*/ inline void PrintTo(const unsigned char * s, std::basic_ostream<char> & stream)
{
    PrintTo(ImplicitCast_<const void *>(s), stream);
}
/*UNIT_TEST_CPP_EXPORT*/ inline void PrintTo(unsigned char * s, std::basic_ostream<char> & stream)
{
    PrintTo(ImplicitCast_<const void *>(s), stream);
}
UNIT_TEST_CPP_EXPORT void PrintTo(const wchar_t * s, std::basic_ostream<char> & stream);
/*UNIT_TEST_CPP_EXPORT*/ inline void PrintTo(wchar_t * s, std::basic_ostream<char> & stream)
{
    PrintTo(ImplicitCast_<const wchar_t*>(s), stream);
}

UNIT_TEST_CPP_EXPORT void PrintStringTo(const std::string & s, std::basic_ostream<char> & stream);
inline void PrintTo(const std::string & s, std::basic_ostream<char> & stream)
{
    PrintStringTo(s, stream);
}

UNIT_TEST_CPP_EXPORT void PrintWideStringTo(const std::wstring & s, std::basic_ostream<char> & stream);
/*UNIT_TEST_CPP_EXPORT*/ inline void PrintTo(const std::wstring & s, std::basic_ostream<char> & stream)
{
    PrintWideStringTo(s, stream);
}

/*UNIT_TEST_CPP_EXPORT*/ inline void PrintTo(std::nullptr_t UNUSED(p), std::basic_ostream<char> & stream)
{
    PrintStringTo("nullptr", stream);
}

template <typename T>
class UniversalPrinter
{
public:
    // Note: we deliberately don't call this PrintTo(), as that name
    // conflicts with ::testing::internal::PrintTo in the body of the
    // function.
    static void Print(const T& value, std::basic_ostream<char> & stream)
    {
        // By default, ::testing::internal::PrintTo() is used for printing
        // the _value.
        //
        // Thanks to Koenig look-up, if T is a class and has its own
        // PrintTo() function defined in its namespace, that function will
        // be visible here.  Since it is more specific than the generic ones
        // in ::testing::internal, it will be picked by the compiler in the
        // following statement - exactly what we want.
        PrintTo(value, stream);
    }
};

// Prints the given number of elements in an array, without printing
// the curly braces.
template <typename T>
void PrintRawArrayTo(const T a[], size_t count, std::basic_ostream<char> & stream)
{
    UniversalPrint(a[0], stream);
    for (size_t i = 1; i != count; i++)
    {
        stream << ", ";
        UniversalPrint(a[i], stream);
    }
}

// UniversalPrintArray(begin, len, os) prints an array of 'len'
// elements, starting at address 'begin'.
template <typename T>
void UniversalPrintArray(const T * begin, size_t len, std::basic_ostream<char> & stream)
{
    if (len == 0)
    {
        stream << "{}";
    }
    else
    {
        stream << "{ ";
        const size_t Threshold = 18;
        const size_t ChunkSize = 8;
        // If the array has more than Threshold elements, we'll have to
        // omit some details by printing only the first and the last
        // ChunkSize elements.
        // TODO(wan@google.com): let the user control the threshold using a flag.
        if (len <= Threshold)
        {
            PrintRawArrayTo(begin, len, stream);
        }
        else
        {
            PrintRawArrayTo(begin, ChunkSize, stream);
            stream << ", ..., ";
            PrintRawArrayTo(begin + len - ChunkSize, ChunkSize, stream);
        }
        stream << " }";
    }
}

// This overload prints a (const) char array compactly.
UNIT_TEST_CPP_EXPORT void UniversalPrintArray(const char * begin, size_t len, std::basic_ostream<char> & stream);

// This overload prints a (const) wchar_t array compactly.
UNIT_TEST_CPP_EXPORT void UniversalPrintArray(const wchar_t * begin, size_t len, std::basic_ostream<char> & stream);

// Implements printing an array type T[N].
template <typename T, size_t N>
class UniversalPrinter<T[N]>
{
public:
    // Prints the given array, omitting some elements when there are too
    // many.
    static void Print(const T (&a)[N], std::basic_ostream<char> & stream)
    {
        UniversalPrintArray(a, N, stream);
    }
};

// Implements printing a reference type T&.
template <typename T>
class UniversalPrinter<T&>
{
public:
    static void Print(const T& value, std::basic_ostream<char> & stream)
    {
        // Prints the address of the _value.  We use reinterpret_cast here
        // as static_cast doesn't compile when T is a function type.
        stream << "@" << reinterpret_cast<const void*>(&value) << " ";

        // Then prints the _value itself.
        UniversalPrint(value, stream);
    }
};

template <typename T>
void UniversalPrint(const T& value, std::basic_ostream<char> & stream)
{
    typedef T T1;
    UniversalPrinter<T1>::Print(value, stream);
}

template <typename T>
class ValuePrinter
{
public:
    static void Print(const T& value, std::basic_ostream<char> & stream)
    {
        UniversalPrint(value, stream);
    }
};

//template <typename T>
//class ValuePrinter<T&>
//{
//public:
//    static void Print(const T& _value, std::basic_ostream<char> & stream)
//    {
//        UniversalPrint(_value, stream);
//    }
//};

template <typename T, size_t N>
class ValuePrinter<T[N]>
{
public:
    static void Print(const T (&value)[N], std::basic_ostream<char> & stream)
    {
        UniversalPrinter<T[N]>::Print(value, stream);
    }
};

//template <>
//class ValuePrinter<const char*>
//{
//public:
//    static void Print(const char * str, std::basic_ostream<char> & stream)
//    {
//        if (str == nullptr)
//        {
//            stream << "nullptr";
//        }
//        else
//        {
//            UniversalPrint(std::string(str), stream);
//        }
//    }
//};
//template <>
//class ValuePrinter<char *>
//{
//public:
//    static void Print(char* str, std::basic_ostream<char> & stream)
//    {
//        ValuePrinter<const char *>::Print(str, stream);
//    }
//};
//template <>
//class ValuePrinter<const wchar_t *>
//{
//public:
//    static void Print(const wchar_t* str, std::basic_ostream<char> & stream)
//    {
//        if (str == nullptr)
//        {
//            stream << "nullptr";
//        }
//        else
//        {
//            UniversalPrint(std::wstring(str), stream);
//        }
//  }
//};
//template <>
//class ValuePrinter<wchar_t *>
//{
//public:
//    static void Print(wchar_t* str, std::basic_ostream<char> & stream)
//    {
//        ValuePrinter<const wchar_t *>::Print(str, stream);
//    }
//};
//
//template <typename T>
//void UniversalTersePrint(const T& _value, std::basic_ostream<char> & stream)
//{
//    ValuePrinter<T>::Print(_value, stream);
//}
//
template <typename T>
std::string PrintToString(const T& value)
{
    std::basic_stringstream<char> ss;
    ValuePrinter<T>::Print(value, ss);
    return ss.str();
}

} // namespace UnitTestCpp

WARNING_POP
