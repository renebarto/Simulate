#pragma once

#include "AbstractSyntaxTree.h"
#include "MachineCode.h"

namespace Assembler
{

class ICPUAssembler
{
public:
	ICPUAssembler() {}
    virtual ~ICPUAssembler() {}

	virtual bool Generate(ASTree const & ast, MachineCode & machineCode) = 0;
}; // ICPUAssembler

} // namespace Assembler
