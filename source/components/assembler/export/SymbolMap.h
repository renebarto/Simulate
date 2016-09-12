#pragma once

#include <map>
#include <string>
#include "core/String.h"

namespace Assembler
{

template<class Value>
class SymbolMap
{
public:
    using Map = std::map<std::wstring, Value>;

    SymbolMap()
        : map()
    {}
    void Add(std::wstring const & name, Value val)
    {
        std::wstring nameUpper = Core::String::ToUpper(name);
        if (map.find(nameUpper) == map.end())
        {
            map.insert(std::pair<std::wstring, Value>(nameUpper, val));
            return;
        }
        std::ostringstream stream;
        stream << "Symbol already exists: " << Core::String::ToString(name);
        throw std::invalid_argument(stream.str());
	}

    bool Exists(std::wstring const & name) const
    {
        Map::const_iterator it = map.find(Core::String::ToUpper(name));
        return (it != map.end());
    }

	Value const & Lookup(std::wstring const & name) const
    {
        Map::const_iterator it = map.find(Core::String::ToUpper(name));
        if (it != map.end())
        {
            return it->second;
        }
        std::ostringstream stream;
        stream << "Symbol does not exist: " << Core::String::ToString(name);
        throw std::invalid_argument(stream.str());
	}
private:
    Map map;
};

} // namespace Assembler
