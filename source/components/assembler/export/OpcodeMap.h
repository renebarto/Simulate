#pragma once

#include <map>
#include "Exceptions.h"

namespace Assembler
{

template <class Key, class Base>
class OpcodeMap
{
public:
    using Map = std::map<Key, Base>;

    OpcodeMap()
        : map()
    {}
    void Set(Key opcode, Base val)
    {
        if (map.find(opcode) == map.end())
        {
            map.insert(std::pair<Key, Base>(opcode, val));
            return;
        }
        std::ostringstream stream;
        stream << "Opcode already exists: " << int(opcode);
        throw AssemblerException(stream.str());
	}

	Base const & Get(Key opcode) const
    {
        Map::const_iterator it = map.find(opcode);
        if (it != map.end())
        {
            return it->second;
        }
        std::ostringstream stream;
        stream << "Opcode does not exist: " << int(opcode);
        throw AssemblerException(stream.str());
	}
private:
    Map map;
};

} // namespace Assembler
