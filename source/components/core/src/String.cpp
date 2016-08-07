#include "core/String.h"

#include <cstring>
#include "osal.h"

namespace Core
{
namespace String
{

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
    return OSAL::strcasecmp(lhs, rhs) == 0;
}

bool IsEqual(const wchar_t * lhs, const wchar_t * rhs)
{
    return wcscmp(lhs, rhs) == 0;
}

bool IsEqualIgnoreCase(const wchar_t * lhs, const wchar_t * rhs)
{
    return OSAL::wcscasecmp(lhs, rhs) == 0;
}

std::string ToLower(const std::string & text)
{
    std::string result;
    for (size_t i = 0; i < text.length(); i++)
    {
        result += tolower(text[i]);
    }
    return result;
}

std::string ToUpper(const std::string & text)
{
    std::string result;
    for (size_t i = 0; i < text.length(); i++)
    {
        result += toupper(text[i]);
    }
    return result;
}

std::string Quote(const std::string & text)
{
    if ((text[0] == '\"') && (text[text.length() - 1] == '\"'))
    {
        return text;
    }
    return '\"' + text + '\"';
}

std::string UnQuote(const std::string & text)
{
    if ((text.length() >= 2) && (text[0] == '\"') && (text[text.length() - 1] == '\"'))
    {
        return text.substr(1, text.length() - 2);
    }
    return text;
}

std::string TrimStart(const std::string & text)
{
    size_t firstNonSpace = 0;
    while (isspace(text[firstNonSpace]))
        firstNonSpace++;
    return text.substr(firstNonSpace);
}

std::string TrimEnd(const std::string & text)
{
    size_t lastNonSpace = text.length();
    while (isspace(text[lastNonSpace - 1]))
        lastNonSpace--;
    return text.substr(0, lastNonSpace);
}

std::string Trim(const std::string & text)
{
    return TrimStart(TrimEnd(text));
}

std::string ToString(std::wstring const & str)
{
    size_t sz = str.length();
    #if defined(_MSC_VER)
        int nd = WideCharToMultiByte(CP_UTF8, 0, &str[0], int(sz), NULL, 0, NULL, NULL);
        std::string ret(nd, 0);
        int w = WideCharToMultiByte(CP_UTF8, 0, &str[0], int(sz), &ret[0], nd, NULL, NULL);
        if (w != sz)
            throw std::runtime_error("Invalid size written");
        return ret;
    #else
        const wchar_t* p = str.c_str();
        char* tp = new char[sz];
        size_t w = wcstombs(tp, p, sz);
        if (w != sz)
        {
            delete[] tp;
            throw std::runtime_error("Invalid size written");
        }
        std::string ret(tp);
        delete[] tp;
        return ret;
    #endif
}

std::wstring ToWString(std::string const & str)
{
    #if defined(_MSC_VER)
        size_t sz = str.length();
        int nd = MultiByteToWideChar(CP_UTF8, 0, &str[0], int(sz), NULL, 0);
        std::wstring ret(nd, 0);
        int w = MultiByteToWideChar(CP_UTF8, 0, &str[0], int(sz), &ret[0], nd);
        if (w != sz)
            throw std::runtime_error("Invalid size written");
        return ret;
    #else
        const char* p = str.c_str();
        size_t len = str.length();
        size_t sz = len * sizeof(wchar_t);
        wchar_t* tp = new wchar_t[sz];
        size_t w = mbstowcs(tp, p, sz);
        if (w != len) 
        {
            delete[] tp;
            throw std::runtime_error("Invalid size written");
        }
        std::wstring ret(tp);
        delete[] tp;
        return ret;
    #endif
}

} // namespace String
} // namespace Core
