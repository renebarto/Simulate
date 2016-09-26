#pragma once

#include "CommandLineOptionsParser.h"

namespace ASM
{

class ASM_8080
{
public:
    ASM_8080(CommandLineOptionsParser const & options);
    virtual ~ASM_8080();

    bool Run();

protected:
    CommandLineOptionsParser const & options;
};

} // namespace ASM