#pragma once
#include <Reader.h>

namespace ASM8080
{

class Parser
{
public:
    virtual ~Parser() {}

    virtual void Attach(Reader & reader) = 0;
    virtual bool Parse() = 0;
};

} // namespace ASM8080
