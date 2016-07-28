#pragma once

#include "simple-processor/processor.h"

namespace Simulate
{

class StringReader : public CharReader
{
public:
    StringReader()
        : contents()
        , index(0)
    {
    }
    StringReader(std::string & value)
        : contents(value)
        , index(0)
    {
    }
    virtual ~StringReader()
    {
    }

    void SetContents(std::string const & value) { contents = value; index = 0;  }

    virtual bool NoMoreData() override
    {
        return (index >= contents.size());
    }
    virtual char ReadChar() override
    {
        if (!NoMoreData())
        {
            return contents[index++];
        }
        return '\0';
    }

private:
    std::string contents;
    size_t index;
};

} // namespace Simulate
