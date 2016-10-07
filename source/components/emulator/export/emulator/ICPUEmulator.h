#pragma once

#include "osal.h"
#include "osal/flagoperators.h"

namespace Emulator
{

enum class Options : uint8_t
{
    Step = 1 << 1,
    ShowInstructionResults = 1 << 2,
    ShowFinalResults = 1 << 3,
    None = 0x00,
};

DEFINE_FLAG_OPERATORS(Options, uint8_t);

class ICPUEmulator
{
public:
	ICPUEmulator() {}
    virtual ~ICPUEmulator() {}

	virtual bool Run(Options options) = 0;
}; // ICPUEmulator

} // namespace Emulator
