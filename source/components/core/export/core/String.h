#pragma once

#include <locale>
#include <string>

#include "core/String/Deserialize.h"
#include "core/String/Serialize.h"

namespace Core
{
namespace String
{

bool IsEqual(const std::string & lhs, const std::string & rhs);
bool IsEqualIgnoreCase(const std::string & lhs, const std::string & rhs);
bool IsEqual(const char * lhs, const char * rhs);
bool IsEqualIgnoreCase(const char * lhs, const char * rhs);
bool IsEqual(const wchar_t * lhs, const wchar_t * rhs);
bool IsEqualIgnoreCase(const wchar_t * lhs, const wchar_t * rhs);
std::string ToLower(const std::string & text);
std::string ToUpper(const std::string & text);
std::string Quote(const std::string & text);
std::string UnQuote(const std::string & text);
std::string TrimStart(const std::string & text);
std::string TrimEnd(const std::string & text);
std::string Trim(const std::string & text);

} // namespace String
} // namespace Core
