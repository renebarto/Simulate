#pragma once

#include "AbstractSyntaxTree.h"

namespace Assembler
{

class ICPUAssembler;

class ICPUParser
{
public:
	ICPUParser() {}
    virtual ~ICPUParser() {}

	virtual void Parse() = 0;
	virtual void Generate(ICPUAssembler & assembler) = 0;
    virtual void PrintWithErrors() = 0;
    virtual ASTree const & GetAST() const = 0;
}; // ICPUParser

} // namespace Assembler
