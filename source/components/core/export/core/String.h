#pragma once

#include <locale>
#include <string>

#include "core/String/Deserialize.h"
#include "core/String/Serialize.h"

namespace Core
{
namespace String
{

bool IsEqual(std::string const & lhs, std::string const & rhs);
bool IsEqualIgnoreCase(std::string const & lhs, std::string const & rhs);
bool IsEqual(std::wstring const & lhs, std::wstring const & rhs);
bool IsEqualIgnoreCase(std::wstring const & lhs, std::wstring const & rhs);
bool IsEqual(char const * lhs, char const * rhs);
bool IsEqualIgnoreCase(char const * lhs, char const * rhs);
bool IsEqual(const wchar_t * lhs, const wchar_t * rhs);
bool IsEqualIgnoreCase(const wchar_t * lhs, const wchar_t * rhs);
std::string ToLower(std::string const & text);
std::string ToUpper(std::string const & text);
std::wstring ToLower(std::wstring const & text);
std::wstring ToUpper(std::wstring const & text);
std::string Quote(std::string const & text);
std::string UnQuote(std::string const & text);
std::wstring Quote(std::wstring const & text);
std::wstring UnQuote(std::wstring const & text);
std::string TrimStart(std::string const & text);
std::string TrimEnd(std::string const & text);
std::string Trim(std::string const & text);
std::wstring TrimStart(std::wstring const & text);
std::wstring TrimEnd(std::wstring const & text);
std::wstring Trim(std::wstring const & text);

std::string ToString(std::wstring const & str);
std::wstring ToWString(std::string const & str);

} // namespace String
} // namespace Core
