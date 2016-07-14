#include "unit-test-c++/PrintValue.h"

#include <osal.h>
#include "unit-test-c++/internal/UtString.h"

using namespace std;

namespace UnitTestCpp
{

string WideStringToUtf8(const wstring & value)
{
    return internal::WideStringToUtf8(&value[0], static_cast<int>(value.length()));
}

void PrintTo(unsigned char c, ostream & os)
{
    internal::PrintCharAndCodeTo<unsigned char>(c, os);
}
void PrintTo(signed char c, ostream & os)
{
    internal::PrintCharAndCodeTo<unsigned char>(c, os);
}

// Prints a wchar_t as a symbol if it is printable or as its internal
// code otherwise and also as its code.  L'\0' is printed as "L'\\0'".
void PrintTo(wchar_t wc, ostream & os)
{
    internal::PrintCharAndCodeTo<wchar_t>(wc, os);
}

void PrintTo(const char* s, std::ostream & stream)
{
    internal::PrintCharsAsStringTo(s, strlen(s), stream);
}

void PrintTo(const wchar_t * s, std::ostream & stream)
{
    internal::PrintCharsAsStringTo(s, wcslen(s), stream);
}

void PrintStringTo(const string& s, ostream & os)
{
    internal::PrintCharsAsStringTo(s.data(), s.size(), os);
}

void PrintWideStringTo(const wstring& s, ostream & os)
{
    internal::PrintCharsAsStringTo(s.data(), s.size(), os);
}

// Prints a (const) char/wchar_t array of 'len' elements, starting at address
// 'begin'.  CharType must be either char or wchar_t.
template <typename CharType>
static void UniversalPrintCharArray(const CharType * begin, size_t len, ostream & stream)
{
    // The code
    //   const char kFoo[] = "foo";
    // generates an array of 4, not 3, elements, with the last one being '\0'.
    //
    // Therefore when printing a char array, we don't print the last element if
    // it's '\0', such that the output matches the string literal as it's
    // written in the source code.
    if (len > 0 && begin[len - 1] == '\0')
    {
        internal::PrintCharsAsStringTo(begin, len - 1, stream);
        return;
    }

    // If, however, the last element in the array is not '\0', e.g.
    //    const char kFoo[] = { 'f', 'o', 'o' };
    // we must print the entire array.  We also print a message to indicate
    // that the array is not NUL-terminated.
    internal::PrintCharsAsStringTo(begin, len, stream);
    stream << " (no terminating NUL)";
}

// Prints a (const) char array of 'len' elements, starting at address 'begin'.
void UniversalPrintArray(const char * begin, size_t len, ostream & stream)
{
    UniversalPrintCharArray(begin, len, stream);
}

// Prints a (const) wchar_t array of 'len' elements, starting at address
// 'begin'.
void UniversalPrintArray(const wchar_t * begin, size_t len, ostream & stream)
{
    UniversalPrintCharArray(begin, len, stream);
}

}  // namespace UnitTestCpp
