#pragma once

#include <string>
#include <typeinfo>

namespace OSAL
{

// Symbol lookup
std::string ConvertTypeName(std::string mangledName);
template <class T>
inline std::string TypeName(const T & x)
{
#if defined(_MSC_VER)
	std::string name = typeid(x).name();
	const std::string classPrefix = "class ";
	size_t pos = name.find(classPrefix, 0);
	if (pos == 0)
		name = name.substr(classPrefix.length());
	return name;
#elif defined(__GNUC__)
	return OSAL::ConvertTypeName(typeid(x).name());
#endif
}

}  // namespace OSAL
