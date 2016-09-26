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
    virtual void PrintSymbolTable() = 0;
    virtual void PrintSymbolCrossReference() = 0;
    virtual void DumpAST(std::wostream & stream, size_t startColumn) = 0;
    virtual ASTree const & GetAST() const = 0;
}; // ICPUParser

} // namespace Assembler
