#pragma once

#include <map>
#include "Exceptions.h"

namespace Assembler
{

template <class Key, class Base>
class StartStates
{
public:
    using Map = std::map<Key, Base>;

    StartStates()
        : map()
    {}
    void Set(Key key, Base val)
    {
        if (map.find(key) == map.end())
        {
            map.insert(std::pair<Key, Base>(key, val));
            return;
        }
        throw AssemblerException("Key already exists");
	}

	Base Get(Key key)
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
