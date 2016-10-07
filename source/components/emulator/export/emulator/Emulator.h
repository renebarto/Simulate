#pragma once

#include "emulator/ICPUEmulator.h"
#include <memory>
#include "assembler/CPUType.h"
#include "assembler/ObjectCode.h"
#include "assembler/PrettyPrinter.h"
#include "emulator/CPUEmulatorIntel8080.h"

namespace Emulator
{

std::unique_ptr<ICPUEmulator> CreateEmulator(Assembler::CPUType cpuType, Assembler::ObjectCode const & objectCode, Assembler::PrettyPrinter<wchar_t> & printer)
{
    switch (cpuType)
    {
    case Assembler::CPUType::Intel8080:
        return std::unique_ptr<ICPUEmulator>(new CPUEmulatorIntel8080(objectCode, printer));
    default:
        {
            printer << L"Unsupported CPU type: " << cpuType;
            return std::unique_ptr<ICPUEmulator>();
        }
    }
}


} // namespace Emulator
