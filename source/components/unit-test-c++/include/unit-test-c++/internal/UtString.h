#include "unit-test-c++/PrintValue.h"
#include <osal.h>

using namespace std;

namespace UnitTestCpp
{

namespace internal
{

class String
{
public:
    // Clones a 0-terminated C string, allocating memory using new.  The
    // caller is responsible for deleting the return value using
    // delete[].  Returns the cloned string, or nullptr if the input is
    // nullptr.
    //
    // This is different from strdup() in string.h, which allocates
    // memory using malloc().
    static const char * CloneCString(const char * c_str);

    // Compares two C strings.  Returns true iff they have the same content.
    //
    // Unlike strcmp(), this function can handle nullptr argument(s).  A
    // nullptr C string is considered different to any non-nullptr C string,
    // including the empty string.
    static bool CStringEquals(const char* lhs, const char* rhs);

    // Converts a wide C string to a String using the UTF-8 encoding.
    // nullptr will be converted to "(null)".  If an error occurred during
    // the conversion, "(failed to convert from wide string)" is
    // returned.
    static string ShowWideCString(const wchar_t* wide_c_str);

    // Compares two wide C strings.  Returns true iff they have the same
    // content.
    //
    // Unlike wcscmp(), this function can handle nullptr argument(s).  A
    // nullptr C string is considered different to any non-nullptr C string,
    // including the empty string.
    static bool WideCStringEquals(const wchar_t* lhs, const wchar_t* rhs);

    // Compares two C strings, ignoring case.  Returns true iff they
    // have the same content.
    //
    // Unlike strcasecmp(), this function can handle nullptr argument(s).
    // A nullptr C string is considered different to any non-nullptr C string,
    // including the empty string.
    static bool CaseInsensitiveCStringEquals(const char* lhs, const char* rhs);

    // Compares two wide C strings, ignoring case.  Returns true iff they
    // have the same content.
    //
    // Unlike wcscasecmp(), this function can handle nullptr argument(s).
    // A nullptr C string is considered different to any non-nullptr wide C string,
    // including the empty string.
    // NB: The implementations on different platforms slightly differ.
    // On windows, this method uses _wcsicmp which compares according to LC_CTYPE
    // environment variable. On GNU platform this method uses wcscasecmp
    // which compares according to LC_CTYPE category of the current locale.
    // On MacOS X, it uses towlower, which also uses LC_CTYPE category of the
    // current locale.
    static bool CaseInsensitiveWideCStringEquals(const wchar_t* lhs, const wchar_t* rhs);

    // Returns true iff the given string ends with the given suffix, ignoring
    // case. Any string is considered to end with an empty suffix.
    static bool EndsWithCaseInsensitive(const string& str, const string& suffix);

    // Formats an int value as "%02d".
    static string FormatIntWidth2(int value);  // "%02d" for width == 2

    // Formats an int value as "%X".
    static string FormatHexInt(int value);

    // Formats a byte as "%02X".
    static string FormatByte(unsigned char value);

private:
    String();  // Not meant to be instantiated.
};  // class String

// The following two functions only make sense if the the system
// uses UTF-16 for wide string encoding. All supported systems
// with 16 bit wchar_t (Windows, Cygwin, Symbian OS) do use UTF-16.

// Determines if the arguments constitute UTF-16 surrogate pair
// and thus should be combined into a single Unicode code point
// using CreateCodePointFromUtf16SurrogatePair.
inline bool IsUtf16SurrogatePair(wchar_t first, wchar_t second)
{
    return sizeof(wchar_t) == 2 &&
           (first & 0xFC00) == 0xD800 && (second & 0xFC00) == 0xDC00;
}

// Creates a Unicode code point from UTF16 surrogate pair.
inline uint32_t CreateCodePointFromUtf16SurrogatePair(wchar_t first,
        wchar_t second)
{
    const uint32_t mask = (1 << 10) - 1;
    return (sizeof(wchar_t) == 2) ?
           (((first & mask) << 10) | (second & mask)) + 0x10000 :
           // This function should not be called when the condition is
           // false, but we provide a sensible default in case it is.
           static_cast<uint32_t>(first);
}

// Converts the buffer in a stringstream to an string, converting NUL
// bytes to "\\0" along the way.
string StringStreamToString(stringstream* ss)
{
    const string& str = ss->str();
    const char* const start = str.c_str();
    const char* const end = start + str.length();

    string result;
    result.reserve(2 * (end - start));
    for (const char* ch = start; ch != end; ++ch)
    {
        if (*ch == '\0')
        {
            result += "\\0";  // Replaces NUL with "\\0";
        }
        else
        {
            result += *ch;
        }
    }

    return result;
}

// Utility functions for encoding Unicode text (wide strings) in
// UTF-8.

// A Unicode code-point can have upto 21 bits, and is encoded in UTF-8
// like this:
//
// Code-point length   Encoding
//   0 -  7 bits       0xxxxxxx
//   8 - 11 bits       110xxxxx 10xxxxxx
//  12 - 16 bits       1110xxxx 10xxxxxx 10xxxxxx
//  17 - 21 bits       11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

// The maximum code-point a one-byte UTF-8 sequence can represent.
const uint32_t kMaxCodePoint1 = (static_cast<uint32_t>(1) <<  7) - 1;

// The maximum code-point a two-byte UTF-8 sequence can represent.
const uint32_t kMaxCodePoint2 = (static_cast<uint32_t>(1) << (5 + 6)) - 1;

// The maximum code-point a three-byte UTF-8 sequence can represent.
const uint32_t kMaxCodePoint3 = (static_cast<uint32_t>(1) << (4 + 2*6)) - 1;

// The maximum code-point a four-byte UTF-8 sequence can represent.
const uint32_t kMaxCodePoint4 = (static_cast<uint32_t>(1) << (3 + 3*6)) - 1;

// Chops off the n lowest bits from a bit pattern.  Returns the n
// lowest bits.  As a side effect, the original bit pattern will be
// shifted to the right by n bits.
inline uint32_t ChopLowBits(uint32_t* bits, int n)
{
    const uint32_t low_bits = *bits & ((static_cast<uint32_t>(1) << n) - 1);
    *bits >>= n;
    return low_bits;
}

// Converts a Unicode code point to a narrow string in UTF-8 encoding.
// code_point parameter is of type uint32_t because wchar_t may not be
// wide enough to contain a code point.
// If the code_point is not a valid Unicode code point
// (i.e. outside of Unicode range U+0 to U+10FFFF) it will be converted
// to "(Invalid Unicode 0xXXXXXXXX)".
string CodePointToUtf8(uint32_t code_point)
{
    if (code_point > kMaxCodePoint4)
    {
        return "(Invalid Unicode 0x" + String::FormatHexInt(code_point) + ")";
    }

    char str[5];  // Big enough for the largest valid code point.
    if (code_point <= kMaxCodePoint1)
    {
        str[1] = '\0';
        str[0] = static_cast<char>(code_point);                          // 0xxxxxxx
    }
    else if (code_point <= kMaxCodePoint2)
    {
        str[2] = '\0';
        str[1] = static_cast<char>(0x80 | ChopLowBits(&code_point, 6));  // 10xxxxxx
        str[0] = static_cast<char>(0xC0 | code_point);                   // 110xxxxx
    }
    else if (code_point <= kMaxCodePoint3)
    {
        str[3] = '\0';
        str[2] = static_cast<char>(0x80 | ChopLowBits(&code_point, 6));  // 10xxxxxx
        str[1] = static_cast<char>(0x80 | ChopLowBits(&code_point, 6));  // 10xxxxxx
        str[0] = static_cast<char>(0xE0 | code_point);                   // 1110xxxx
    }
    else
    {
        // code_point <= kMaxCodePoint4
        str[4] = '\0';
        str[3] = static_cast<char>(0x80 | ChopLowBits(&code_point, 6));  // 10xxxxxx
        str[2] = static_cast<char>(0x80 | ChopLowBits(&code_point, 6));  // 10xxxxxx
        str[1] = static_cast<char>(0x80 | ChopLowBits(&code_point, 6));  // 10xxxxxx
        str[0] = static_cast<char>(0xF0 | code_point);                   // 11110xxx
    }
    return str;
}

// Converts a wide string to a narrow string in UTF-8 encoding.
// The wide string is assumed to have the following encoding:
//   UTF-16 if sizeof(wchar_t) == 2 (on Windows, Cygwin, Symbian OS)
//   UTF-32 if sizeof(wchar_t) == 4 (on Linux)
// Parameter str points to a null-terminated wide string.
// Parameter num_chars may additionally limit the number
// of wchar_t characters processed. -1 is used when the entire string
// should be processed.
// If the string contains code points that are not valid Unicode code points
// (i.e. outside of Unicode range U+0 to U+10FFFF) they will be output
// as '(Invalid Unicode 0xXXXXXXXX)'. If the string is in UTF16 encoding
// and contains invalid UTF-16 surrogate pairs, values in those pairs
// will be encoded as individual Unicode characters from Basic Normal Plane.
string WideStringToUtf8(const wchar_t* str, int num_chars)
{
    if (num_chars == -1)
        num_chars = static_cast<int>(wcslen(str));

    stringstream stream;
    for (int i = 0; i < num_chars; ++i)
    {
        uint32_t unicode_code_point;

        if (str[i] == L'\0')
        {
            break;
        }
        else if (i + 1 < num_chars && IsUtf16SurrogatePair(str[i], str[i + 1]))
        {
            unicode_code_point = CreateCodePointFromUtf16SurrogatePair(str[i], str[i + 1]);
            i++;
        }
        else
        {
            unicode_code_point = static_cast<uint32_t>(str[i]);
        }

        stream << CodePointToUtf8(unicode_code_point);
    }
    return StringStreamToString(&stream);
}

// Compares two C strings.  Returns true iff they have the same content.
//
// Unlike strcmp(), this function can handle nullptr argument(s).  A nullptr
// C string is considered different to any non-nullptr C string,
// including the empty string.
bool String::CStringEquals(const char * lhs, const char * rhs)
{
    if (lhs == nullptr)
        return rhs == nullptr;

    if (rhs == nullptr)
        return false;

    return strcmp(lhs, rhs) == 0;
}

// Converts a wide C string to an string using the UTF-8 encoding.
// nullptr will be converted to "(null)".
string String::ShowWideCString(const wchar_t * wide_c_str)
{
    if (wide_c_str == nullptr)  return "(null)";

    return internal::WideStringToUtf8(wide_c_str, -1);
}

// Compares two wide C strings.  Returns true iff they have the same
// content.
//
// Unlike wcscmp(), this function can handle nullptr argument(s).  A nullptr
// C string is considered different to any non-nullptr C string,
// including the empty string.
bool String::WideCStringEquals(const wchar_t * lhs, const wchar_t * rhs)
{
    if (lhs == nullptr)
        return rhs == nullptr;

    if (rhs == nullptr)
        return false;

    return wcscmp(lhs, rhs) == 0;
}

// Compares two C strings, ignoring case.  Returns true iff they have
// the same content.
//
// Unlike strcasecmp(), this function can handle nullptr argument(s).  A
// nullptr C string is considered different to any non-nullptr C string,
// including the empty string.
bool String::CaseInsensitiveCStringEquals(const char * lhs, const char * rhs)
{
    if (lhs == nullptr)
        return rhs == nullptr;
    if (rhs == nullptr)
        return false;
    return OSAL::strcasecmp(lhs, rhs) == 0;
}

// Compares two wide C strings, ignoring case.  Returns true iff they
// have the same content.
//
// Unlike wcscasecmp(), this function can handle nullptr argument(s).
// A nullptr C string is considered different to any non-nullptr wide C string,
// including the empty string.
// NB: The implementations on different platforms slightly differ.
// On windows, this method uses _wcsicmp which compares according to LC_CTYPE
// environment variable. On GNU platform this method uses wcscasecmp
// which compares according to LC_CTYPE category of the current locale.
// On MacOS X, it uses towlower, which also uses LC_CTYPE category of the
// current locale.
bool String::CaseInsensitiveWideCStringEquals(const wchar_t* lhs, const wchar_t* rhs)
{
    if (lhs == nullptr)
        return rhs == nullptr;

    if (rhs == nullptr)
        return false;

    return OSAL::wcscasecmp(lhs, rhs) == 0;
}

// Returns true iff str ends with the given suffix, ignoring case.
// Any string is considered to end with an empty suffix.
bool String::EndsWithCaseInsensitive(const string& str, const string& suffix)
{
    const size_t str_len = str.length();
    const size_t suffix_len = suffix.length();
    return (str_len >= suffix_len) &&
           CaseInsensitiveCStringEquals(str.c_str() + str_len - suffix_len, suffix.c_str());
}

// Formats an int value as "%02d".
string String::FormatIntWidth2(int value)
{
    stringstream ss;
    ss << setfill('0') << setw(2) << value;
    return ss.str();
}

// Formats an int value as "%X".
string String::FormatHexInt(int value)
{
    stringstream ss;
    ss << hex << uppercase << value;
    return ss.str();
}

// Formats a byte as "%02X".
string String::FormatByte(unsigned char value)
{
    stringstream ss;
    ss << setfill('0') << setw(2) << hex << uppercase
       << static_cast<unsigned int>(value);
    return ss.str();
}


// Depending on the value of a char (or wchar_t), we print it in one
// of three formats:
//   - as is if it's a printable ASCII (e.g. 'a', '2', ' '),
//   - as a hexidecimal escape sequence (e.g. '\x7F'), or
//   - as a special escape sequence (e.g. '\r', '\n').
enum CharFormat
{
    kAsIs,
    kHexEscape,
    kSpecialEscape
};

// Returns true if c is a printable ASCII character.  We test the
// value of c directly instead of calling isprint(), which is buggy on
// Windows Mobile.
inline bool IsPrintableAscii(wchar_t c)
{
    return 0x20 <= c && c <= 0x7E;
}

inline bool IsXDigit(char ch)
{
    return isxdigit(static_cast<unsigned char>(ch)) != 0;
}
inline bool IsXDigit(wchar_t ch)
{
    const unsigned char low_byte = static_cast<unsigned char>(ch);
    return ch == low_byte && isxdigit(low_byte) != 0;
}

// Prints a wide or narrow char c as a character literal without the
// quotes, escaping it when necessary; returns how c was formatted.
// The template argument UnsignedChar is the unsigned version of Char,
// which is the type of c.
template <typename UnsignedChar, typename Char>
static CharFormat PrintAsCharLiteralTo(Char c, ostream & os)
{
    switch (static_cast<wchar_t>(c))
    {
    case L'\0':
        os << "\\0";
        break;
    case L'\'':
        os << "\\'";
        break;
    case L'\\':
        os << "\\\\";
        break;
    case L'\a':
        os << "\\a";
        break;
    case L'\b':
        os << "\\b";
        break;
    case L'\f':
        os << "\\f";
        break;
    case L'\n':
        os << "\\n";
        break;
    case L'\r':
        os << "\\r";
        break;
    case L'\t':
        os << "\\t";
        break;
    case L'\v':
        os << "\\v";
        break;
    default:
        if (IsPrintableAscii(c))
        {
            os << static_cast<char>(c);
            return kAsIs;
        }
        else
        {
            os << "\\x" + String::FormatHexInt(static_cast<UnsignedChar>(c));
            return kHexEscape;
        }
    }
    return kSpecialEscape;
}

// Prints a wchar_t c as if it's part of a string literal, escaping it when
// necessary; returns how c was formatted.
static CharFormat PrintAsStringLiteralTo(wchar_t c, ostream & os)
{
    switch (c)
    {
    case L'\'':
        os << "'";
        return kAsIs;
    case L'"':
        os << "\\\"";
        return kSpecialEscape;
    default:
        return PrintAsCharLiteralTo<wchar_t>(c, os);
    }
}

// Prints a char c as if it's part of a string literal, escaping it when
// necessary; returns how c was formatted.
static CharFormat PrintAsStringLiteralTo(char c, ostream & os)
{
    return PrintAsStringLiteralTo(static_cast<wchar_t>(static_cast<unsigned char>(c)), os);
}

// Prints a wide or narrow character c and its code.  '\0' is printed
// as "'\\0'", other unprintable characters are also properly escaped
// using the standard C++ escape sequence.  The template argument
// UnsignedChar is the unsigned version of Char, which is the type of c.
template <typename UnsignedChar, typename Char>
void PrintCharAndCodeTo(Char c, ostream & os)
{
    // First, print c as a literal in the most readable form we can find.
    os << ((sizeof(c) > 1) ? "L'" : "'");
    const CharFormat format = PrintAsCharLiteralTo<UnsignedChar>(c, os);
    os << "'";

    // To aid user debugging, we also print c's code in decimal, unless
    // it's 0 (in which case c was printed as '\\0', making the code
    // obvious).
    if (c == 0)
        return;
    os << " (" << static_cast<int>(c);

    // For more convenience, we print c's code again in hexidecimal,
    // unless c was already printed in the form '\x##' or the code is in
    // [1, 9].
    if (format == kHexEscape || (1 <= c && c <= 9))
    {
        // Do nothing.
    }
    else
    {
        os << ", 0x" << String::FormatHexInt(static_cast<UnsignedChar>(c));
    }
    os << ")";
}

// Prints the given array of characters to the ostream.  CharType must be either
// char or wchar_t.
// The array starts at begin, the length is len, it may include '\0' characters
// and may not be NUL-terminated.
template <typename CharType>
static void PrintCharsAsStringTo(const CharType* begin, size_t len, ostream & os)
{
    const char* const kQuoteBegin = sizeof(CharType) == 1 ? "\"" : "L\"";
    os << kQuoteBegin;
    bool is_previous_hex = false;
    for (size_t index = 0; index < len; ++index)
    {
        const CharType cur = begin[index];
        if (is_previous_hex && IsXDigit(cur))
        {
            // Previous character is of '\x..' form and this character can be
            // interpreted as another hexadecimal digit in its number. Break string to
            // disambiguate.
            os << "\" " << kQuoteBegin;
        }
        is_previous_hex = PrintAsStringLiteralTo(cur, os) == kHexEscape;
    }
    os << "\"";
}

} // internal

} // namespace UnitTestCpp
