#pragma once

#include <map>
#include <string>

namespace Assembler
{

class KeywordMap
{
public:
    using Map = std::map<std::wstring, size_t>;

    KeywordMap()
        : map()
    {}
    void Set(std::wstring const & key, size_t val)
    {
        if (map.find(key) == map.end())
        {
            map.insert(std::pair<std::wstring, size_t>(key, val));
            return;
        }
        throw std::runtime_error("Key already existent");
	}

	size_t Get(std::wstring const & key, size_t defaultVal)
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
