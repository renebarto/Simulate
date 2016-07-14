#pragma once
#include <type_traits>
#include <typeinfo>
#include <osal/unused.h>
#include <osal.h>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>

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

    no operator<<(std::ostream const&, any_t const&);

    yes& test(std::ostream&);
    no test(no);

    template<typename T>
    struct has_insertion_operator {
        static std::ostream &s;
        static T const &t;
        static bool const value = sizeof(test(s << t)) == sizeof(yes);
    };
}

template<typename T>
struct has_insertion_operator : has_insertion_operator_impl::has_insertion_operator<T>
{
};

template<typename To>
inline To ImplicitCast_(To x) { return x; }

// ImplicitlyConvertible<From, To>::value is a compile-time bool
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
    // The first version returns a value of size 1, and the second
    // version returns a value of size 2.  Therefore, by checking the
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
void DefaultPrintNonContainerTo(const T& value, std::ostream & stream)
{
    stream << value;
}

std::string WideStringToUtf8(const std::wstring & value);

template <typename T>
void UniversalPrint(const T& value, std::ostream & stream);

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
template <typename C>
void DefaultPrintTo(IsContainer /* dummy */,
                    std::false_type isClass,
                    std::false_type isEnum,
                    std::false_type isPointer,
                    const C& container,
                    std::ostream & stream)
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

//template <typename T>
//void DefaultPrintTo(IsContainer /* dummy */,
//                    std::false_type isClass,
//                    std::false_type isEnum,
//                    std::true_type isPointer,
//                    const T& value,
//                    std::ostream & stream)
//{
//    stream << "container pointer";
//}
//
//template <typename T>
//void DefaultPrintTo(IsContainer /* dummy */,
//                    std::false_type isClass,
//                    std::true_type isEnum,
//                    std::false_type isPointer,
//                    const T& value,
//                    std::ostream & stream)
//{
//    stream << "container enum";
//}
//
//template <typename T>
//void DefaultPrintTo(IsContainer /* dummy */,
//                    std::false_type isClass,
//                    std::true_type isEnum,
//                    std::true_type isPointer,
//                    const T& value,
//                    std::ostream & stream)
//{
//    stream << "container enum pointer";
//}
//
//template <typename T>
//void DefaultPrintTo(IsContainer /* dummy */,
//                    std::true_type UNUSED(isClass),
//                    std::false_type UNUSED(isEnum),
//                    std::false_type UNUSED(isPointer),
//                    const T& UNUSED(value),
//                    std::ostream & stream)
//{
//    stream << "container class";
//}
//
//template <typename T>
//void DefaultPrintTo(IsContainer /* dummy */,
//                    std::true_type isClass,
//                    std::false_type isEnum,
//                    std::true_type isPointer,
//                    const T& value,
//                    std::ostream & stream)
//{
//    stream << "container class pointer";
//}
//
//template <typename T>
//void DefaultPrintTo(IsContainer /* dummy */,
//                    std::true_type isClass,
//                    std::true_type isEnum,
//                    std::false_type isPointer,
//                    const T& value,
//                    std::ostream & stream)
//{
//    stream << "container enum class";
//}
//
//template <typename T>
//void DefaultPrintTo(IsContainer /* dummy */,
//                    std::true_type isClass,
//                    std::true_type isEnum,
//                    std::true_type isPointer,
//                    const T& value,
//                    std::ostream & stream)
//{
//    stream << "container enum class pointer";
//}

// Normal value
template <typename T>
void DefaultPrintTo(IsNotContainer /* dummy */,
                    std::false_type UNUSED(isClass),
                    std::false_type UNUSED(isEnum),
                    std::false_type UNUSED(isPointer),
                    const T& value,
                    std::ostream & stream)
{
    DefaultPrintNonContainerTo(value, stream);
}

// Normal value pointer
template <typename T>
void DefaultPrintTo(IsNotContainer /* dummy */,
                    std::false_type,
                    std::false_type,
                    std::true_type,
                    const T& value,
                    std::ostream & stream)
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

// Enum value
template <typename T>
typename std::enable_if<has_insertion_operator<T>::value, void>::type
DefaultPrintTo(IsNotContainer /* dummy */,
               std::false_type UNUSED(isClass),
               std::true_type UNUSED(isEnum),
               std::false_type UNUSED(isPointer),
               const T& value,
               std::ostream & stream)
{
    stream << value;
}

// Enum value
template <typename T>
typename std::enable_if<!has_insertion_operator<T>::value, void>::type
DefaultPrintTo(IsNotContainer /* dummy */,
               std::false_type UNUSED(isClass),
               std::true_type UNUSED(isEnum),
               std::false_type UNUSED(isPointer),
               const T& value,
               std::ostream & stream)
{
    stream << (int)value;
}

//// Enum pointer
//template <typename T>
//void DefaultPrintTo(IsNotContainer /* dummy */,
//                    std::false_type isClass,
//                    std::true_type isEnum,
//                    std::true_type isPointer,
//                    const T& value,
//                    std::ostream & stream)
//{
//    stream << "enum " << (int)*value;
//}

// Class or Struct value
template <typename T>
typename std::enable_if<has_insertion_operator<T>::value, void>::type
DefaultPrintTo(IsNotContainer /* dummy */,
               std::true_type,
               std::false_type,
               std::false_type,
               const T& value,
               std::ostream & stream)
{
    stream << value;
}

// Class or Struct value
template <typename T>
typename std::enable_if<!has_insertion_operator<T>::value, void>::type
DefaultPrintTo(IsNotContainer /* dummy */,
               std::true_type UNUSED(isClass),
               std::false_type UNUSED(isEnum),
               std::false_type UNUSED(isPointer),
               const T& UNUSED(value),
               std::ostream & stream)
{
#if defined(_MSC_VER)
    stream << typeid(T).name();
#elif defined(__GNUC__)
    stream << OSAL::ConvertTypeName(typeid(T).name());
#endif
}

//// Class or Struct value
//template <typename T>
//void DefaultPrintTo(IsNotContainer /* dummy */,
//                    std::true_type isClass,
//                    std::false_type isEnum,
//                    std::true_type isPointer,
//                    const T& value,
//                    std::ostream & stream)
//{
//    stream << "class pointer " << ConvertTypeName(typeid(T).name());
//}
//
//template <typename T>
//void DefaultPrintTo(IsNotContainer /* dummy */,
//                    std::true_type isClass,
//                    std::true_type isEnum,
//                    std::false_type isPointer,
//                    const T& value,
//                    std::ostream & stream)
//{
//    stream << "class enum " << ConvertTypeName(typeid(T).name());
//}
//
//template <typename T>
//void DefaultPrintTo(IsNotContainer /* dummy */,
//                    std::true_type isClass,
//                    std::true_type isEnum,
//                    std::true_type isPointer,
//                    const T& value,
//                    std::ostream & stream)
//{
//    stream << "class enum pointer " << ConvertTypeName(typeid(T).name());
//}

template <typename T>
void PrintTo(const T& value, std::ostream & stream)
{
    DefaultPrintTo(IsContainerTest<T>(0), std::is_class<T>(), std::is_enum<T>(), std::is_pointer<T>(), value, stream);
}

void PrintTo(unsigned char c, std::ostream & stream);
void PrintTo(signed char c, std::ostream & stream);
inline void PrintTo(char c, std::ostream & stream)
{
    PrintTo(static_cast<unsigned char>(c), stream);
}

void PrintTo(wchar_t wc, std::ostream & stream);

inline void PrintTo(bool x, std::ostream & stream)
{
    stream << (x ? "true" : "false");
}

void PrintTo(const char* s, std::ostream & stream);
inline void PrintTo(char* s, std::ostream & stream)
{
    PrintTo(ImplicitCast_<const char*>(s), stream);
}

// signed/unsigned char is often used for representing binary data, so
// we print pointers to it as void* to be safe.
inline void PrintTo(const signed char * s, std::ostream & stream)
{
    PrintTo(ImplicitCast_<const void *>(s), stream);
}
inline void PrintTo(signed char * s, std::ostream & stream)
{
    PrintTo(ImplicitCast_<const void *>(s), stream);
}
inline void PrintTo(const unsigned char * s, std::ostream & stream)
{
    PrintTo(ImplicitCast_<const void *>(s), stream);
}
inline void PrintTo(unsigned char * s, std::ostream & stream)
{
    PrintTo(ImplicitCast_<const void *>(s), stream);
}
void PrintTo(const wchar_t * s, std::ostream & stream);
inline void PrintTo(wchar_t * s, std::ostream & stream)
{
    PrintTo(ImplicitCast_<const wchar_t*>(s), stream);
}

void PrintStringTo(const std::string & s, std::ostream & stream);
inline void PrintTo(const std::string & s, std::ostream & stream)
{
    PrintStringTo(s, stream);
}

void PrintWideStringTo(const std::wstring & s, std::ostream & stream);
inline void PrintTo(const std::wstring & s, std::ostream & stream)
{
    PrintWideStringTo(s, stream);
}

inline void PrintTo(std::nullptr_t UNUSED(p), std::ostream & stream)
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
    static void Print(const T& value, std::ostream & stream)
    {
        // By default, ::testing::internal::PrintTo() is used for printing
        // the value.
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
void PrintRawArrayTo(const T a[], size_t count, std::ostream & stream)
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
void UniversalPrintArray(const T * begin, size_t len, std::ostream & stream)
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
void UniversalPrintArray(const char * begin, size_t len, std::ostream & stream);

// This overload prints a (const) wchar_t array compactly.
void UniversalPrintArray(const wchar_t * begin, size_t len, std::ostream & stream);

// Implements printing an array type T[N].
template <typename T, size_t N>
class UniversalPrinter<T[N]>
{
public:
    // Prints the given array, omitting some elements when there are too
    // many.
    static void Print(const T UNUSED((&a)[N]), std::ostream & UNUSED(stream))
    {
        UniversalPrintArray(a, N, stream);
    }
};

// Implements printing a reference type T&.
template <typename T>
class UniversalPrinter<T&>
{
public:
    static void Print(const T& value, std::ostream & stream)
    {
        // Prints the address of the value.  We use reinterpret_cast here
        // as static_cast doesn't compile when T is a function type.
        stream << "@" << reinterpret_cast<const void*>(&value) << " ";

        // Then prints the value itself.
        UniversalPrint(value, stream);
    }
};

template <typename T>
void UniversalPrint(const T& value, std::ostream & stream)
{
    typedef T T1;
    UniversalPrinter<T1>::Print(value, stream);
}

template <typename T>
class ValuePrinter
{
public:
    static void Print(const T& value, std::ostream & stream)
    {
        UniversalPrint(value, stream);
    }
};

//template <typename T>
//class ValuePrinter<T&>
//{
//public:
//    static void Print(const T& value, std::ostream & stream)
//    {
//        UniversalPrint(value, stream);
//    }
//};

template <typename T, size_t N>
class ValuePrinter<T[N]>
{
public:
    static void Print(const T (&value)[N], std::ostream & stream)
    {
        UniversalPrinter<T[N]>::Print(value, stream);
    }
};

//template <>
//class ValuePrinter<const char*>
//{
//public:
//    static void Print(const char * str, std::ostream & stream)
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
//    static void Print(char* str, std::ostream & stream)
//    {
//        ValuePrinter<const char *>::Print(str, stream);
//    }
//};
//template <>
//class ValuePrinter<const wchar_t *>
//{
//public:
//    static void Print(const wchar_t* str, std::ostream & stream)
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
//    static void Print(wchar_t* str, std::ostream & stream)
//    {
//        ValuePrinter<const wchar_t *>::Print(str, stream);
//    }
//};
//
//template <typename T>
//void UniversalTersePrint(const T& value, std::ostream & stream)
//{
//    ValuePrinter<T>::Print(value, stream);
//}
//
template <typename T>
std::string PrintToString(const T& value)
{
    std::stringstream ss;
    ValuePrinter<T>::Print(value, ss);
    return ss.str();
}

} // namespace UnitTestCpp


