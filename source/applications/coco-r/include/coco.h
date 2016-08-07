#pragma once

#include <string>

namespace Coco
{

namespace String
{

wchar_t * Create(const wchar_t *value);
wchar_t * Create(const wchar_t *value, int startIndex);
wchar_t * Create(const wchar_t *value, int startIndex, int length);
wchar_t * Create(std::string const & value);
wchar_t * Create(std::string const & value, int startIndex);
wchar_t * Create(std::string const & value, int startIndex, int length);
wchar_t * Create(std::wstring const & value);
wchar_t * Create(std::wstring const & value, int startIndex);
wchar_t * Create(std::wstring const & value, int startIndex, int length);
wchar_t * CreateAppend(wchar_t const * data1, wchar_t const * data2);
wchar_t * CreateAppend(wchar_t const * data, wchar_t const value);
wchar_t * CreateAppend(std::string const & data1, std::string const & data2);
wchar_t * CreateAppend(std::string const & data, char const value);
wchar_t * CreateAppend(std::wstring const & data1, std::wstring const & data2);
wchar_t * CreateAppend(std::wstring const & data, wchar_t const value);
size_t LastIndexOf(wchar_t const * data, wchar_t const value);
size_t LastIndexOf(std::string const & data, char const value);
size_t LastIndexOf(std::wstring const & data, wchar_t const value);
char * CreateChar(wchar_t const * value);
char * CreateChar(std::string const & value);
char * CreateChar(std::wstring const & value);
void  Delete(wchar_t * &data);
void  Delete(char * &data);

} // namespace String
} // namespace Coco