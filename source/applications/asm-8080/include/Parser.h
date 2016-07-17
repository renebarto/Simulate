#pragma once
#include <Reader.h>

class Parser
{
public:
    virtual ~Parser() {}

    virtual void Attach(Reader & reader) = 0;
    virtual bool Parse() = 0;
};

