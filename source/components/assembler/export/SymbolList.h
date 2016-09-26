#pragma once

#include <algorithm>
#include <vector>
#include <string>
#include "core/String.h"
#include "Exceptions.h"

namespace Assembler
{

template<class Value>
class SymbolList
{
public:
    using Element = std::pair<std::wstring, Value>;
    using List = std::vector<Element>;
    typedef typename List::const_iterator ConstIterator;
    typedef typename List::iterator Iterator;

    SymbolList()
        : list()
    {}
    void Add(std::wstring const & name, Value val)
    {
        std::wstring nameUpper = Core::String::ToUpper(name);
        list.push_back(Element(nameUpper, val));
	}

    ConstIterator begin() const
    {
        return list.begin();
    }

    Iterator begin()
    {
        return list.begin();
    }

    ConstIterator end() const
    {
        return list.end();
    }

    Iterator end()
    {
        return list.end();
    }

    typename ConstIterator Find(std::wstring const & name) const
    {
        ConstIterator it = list.begin();
        while (it != list.end())
        {
            if (it->first == name)
            {
                return it;
            }
            ++it;
        }
        return list.end();
    }

    typename ConstIterator Find(std::wstring const & name, ConstIterator it) const
    {
        if (it == list.end())
            return it;
        ++it;
        while (it != list.end())
        {
            if (it->first == name)
            {
                return it;
            }
            ++it;
        }
        return list.end();
    }

    typename Iterator Find(std::wstring const & name)
    {
        Iterator it = list.begin();
        while (it != list.end())
        {
            if (it->first == name)
            {
                return it;
            }
            ++it;
        }
        return list.end();
    }

    typename Iterator Find(std::wstring const & name, Iterator it)
    {
        if (it == list.end())
            return it;
        ++it;
        while (it != list.end())
        {
            if (it->first == name)
            {
                return it;
            }
            ++it;
        }
        return list.end();
    }

    bool Exists(std::wstring const & name) const
    {
        ConstIterator it = Find(Core::String::ToUpper(name));
        return (it != list.end());
    }

	Value const & Lookup(std::wstring const & name) const
    {
        ConstIterator it = Find(Core::String::ToUpper(name));
        if (it != list.end())
        {
            return it->second;
        }
        std::ostringstream stream;
        stream << "Symbol does not exist: " << Core::String::ToString(name);
        throw AssemblerException(stream.str());
	}
	Value & Lookup(std::wstring const & name)
    {
        Iterator it = Find(Core::String::ToUpper(name));
        if (it != list.end())
        {
            return it->second;
        }
        std::ostringstream stream;
        stream << "Symbol does not exist: " << Core::String::ToString(name);
        throw AssemblerException(stream.str());
	}

    void Sort()
    {
        auto less = [](Element const & x, Element const & y ) { return x.first < y.first; } ;
        std::sort(begin(), end(), less);
    }

private:
    List list;
};

} // namespace Assembler
