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

} // namespace String
} // namespace Core
