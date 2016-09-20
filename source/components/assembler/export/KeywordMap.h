#pragma once

#include <map>
#include <string>
#include "core/String.h"
#include "Exceptions.h"

namespace Assembler
{

template<class Base>
class KeywordMap
{
public:
    using Map = std::map<std::wstring, Base>;

    KeywordMap()
        : map()
    {}
    void Set(std::wstring const & key, Base val)
    {
        std::wstring keyUpper = Core::String::ToUpper(key);
        if (map.find(keyUpper) == map.end())
        {
            map.insert(std::pair<std::wstring, Base>(keyUpper, val));
            return;
        }
        std::ostringstream stream;
        stream << "Key already exists: " << Core::String::ToString(key);
        throw AssemblerException(stream.str());
	}

	Base Get(std::wstring const & key, Base defaultVal)
    {
        Map::const_iterator it = map.find(Core::String::ToUpper(key));
        if (it == map.end())
        {
            return defaultVal;
        }
        return it->second;
	}
private:
    Map map;
};

} // namespace Assembler
