#pragma once

#include<stddef.h>
#include <string>

namespace Coco
{

class StringBuilder  
{
public:
	StringBuilder(size_t capacity = 32);
	StringBuilder(std::wstring const & val);
	
	virtual ~StringBuilder();
	void Append(const wchar_t val);
	void Append(std::wstring const & val);
	std::wstring ToString();
	size_t GetLength() { return length; };

private:
	void Init(size_t capacity);
	std::wstring data;
	size_t capacity;
	size_t length;
};

} // namespace Coco
