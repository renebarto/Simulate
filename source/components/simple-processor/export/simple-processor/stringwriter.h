#pragma once

#include "simple-processor/processor.h"

namespace Simulate
{

class StringWriter : public CharWriter
{
public:
    StringWriter()
        : contents()
    {
    }
    virtual ~StringWriter()
    {
    }

    void ClearContents() { contents.clear();  }

    std::string const & GetContents() { return contents;  }

    virtual void WriteChar(char data) override
    {
        contents += data;
    }

private:
    std::string contents;
};

} // namespace Simulate
