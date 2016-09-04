#pragma once

#include <map>

namespace Assembler
{

class StartStates
{
public:
    using Map = std::map<wchar_t, size_t>;

    StartStates()
        : map()
    {}
    void Set(wchar_t key, size_t val)
    {
        if (map.find(key) == map.end())
        {
            map.insert(std::pair<wchar_t, size_t>(key, val));
            return;
        }
        throw std::runtime_error("Key already existent");
	}

	size_t Get(wchar_t key, size_t defaultVal)
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
