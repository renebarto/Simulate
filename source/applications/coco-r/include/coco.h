#pragma once

#include <string>

namespace Coco
{

namespace String
{

//wchar_t * Create(const wchar_t *value);
//wchar_t * Create(const wchar_t *value, int startIndex);
//wchar_t * Create(const wchar_t *value, int startIndex, int length);
std::string Create(std::string const & value);
std::string Create(std::string const & value, size_t startIndex);
std::string Create(std::string const & value, size_t startIndex, size_t length);
std::wstring Create(std::wstring const & value);
std::wstring Create(std::wstring const & value, size_t startIndex);
std::wstring Create(std::wstring const & value, size_t startIndex, size_t length);
//wchar_t * CreateAppend(wchar_t const * data1, wchar_t const * data2);
//wchar_t * CreateAppend(wchar_t const * data, wchar_t const value);
std::string CreateAppend(std::string const & data1, std::string const & data2);
std::string CreateAppend(std::string const & data, char const value);
std::wstring CreateAppend(std::wstring const & data1, std::wstring const & data2);
std::wstring CreateAppend(std::wstring const & data, wchar_t const value);
//size_t LastIndexOf(wchar_t const * data, wchar_t const value);
size_t LastIndexOf(std::string const & data, char const value);
size_t LastIndexOf(std::wstring const & data, wchar_t const value);
size_t IndexOf(std::string const & data, char const value);
size_t IndexOf(std::wstring const & data, wchar_t const value);
size_t IndexOf(std::string const & data, size_t startIndex, char const value);
size_t IndexOf(std::wstring const & data, size_t startIndex, wchar_t const value);
//char * CreateChar(wchar_t const * value);
//char * CreateChar(std::string const & value);
//char * CreateChar(std::wstring const & value);
//void  Delete(wchar_t * &data);
//void  Delete(char * &data);
void Merge(std::wstring & target, std::wstring const & appendix);
bool Equal(std::wstring const & data1, std::wstring const & data2);
int CompareTo(std::wstring const & data1, std::wstring const & data2);
int Hash(std::wstring const & data);
std::wstring CreateUpper(std::wstring const & data);
std::wstring CreateLower(std::wstring const & data);
std::wstring CreateLower(std::wstring const & data, size_t startIndex, size_t dataLen);

} // namespace String
} // namespace Coco