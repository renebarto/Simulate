#pragma once

#include <map>

namespace Assembler
{

template <class Base>
class StartStates
{
public:
    using Map = std::map<wchar_t, Base>;

    StartStates()
        : map()
    {}
    void Set(wchar_t key, Base val)
    {
        if (map.find(key) == map.end())
        {
            map.insert(std::pair<wchar_t, Base>(key, val));
            return;
        }
        throw std::invalid_argument("Key already existent");
	}

	Base Get(wchar_t key)
    {
        Map::const_iterator it = map.find(key);
        if (it == map.end())
        {
            return Base{};
        }
        return it->second;
	}
private:
    Map map;
};

} // namespace Assembler
