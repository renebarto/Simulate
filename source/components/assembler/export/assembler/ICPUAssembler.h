#pragma once

#include "object-file/ObjectCode.h"

namespace Assembler
{

class ICPUAssembler
{
public:
	ICPUAssembler() {}
    virtual ~ICPUAssembler() {}

	virtual bool Generate(ObjectFile::ObjectCode & objectCode) = 0;
}; // ICPUAssembler

} // namespace Assembler
