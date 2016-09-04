#pragma once

#include <map>
#include <string>

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
        if (map.find(key) == map.end())
        {
            map.insert(std::pair<std::wstring, Base>(key, val));
            return;
        }
        throw std::invalid_argument("Key already existent");
	}

	Base Get(std::wstring const & key, Base defaultVal)
    {
        Map::const_iterator it = map.find(key);
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
