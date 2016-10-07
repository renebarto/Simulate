#pragma once

#include "assembler/ObjectCode.h"

namespace Assembler
{

class ICPUAssembler
{
public:
	ICPUAssembler() {}
    virtual ~ICPUAssembler() {}

	virtual bool Generate(ObjectCode & objectCode) = 0;
}; // ICPUAssembler

} // namespace Assembler
