#pragma once

#include "object-file/ObjectCode.h"
#include "assembler/PrettyPrinter.h"
#include "emulator/ICPUEmulator.h"
#include "emulator/ProcessorIntel8080.h"

namespace Emulator
{

class CPUEmulatorIntel8080 : public ICPUEmulator
{
public:
    CPUEmulatorIntel8080(ObjectFile::ObjectCode const & objectCode, Assembler::PrettyPrinter<wchar_t> & printer);
    virtual ~CPUEmulatorIntel8080();

	bool Run(Options options) override;

private:
    ObjectFile::ObjectCode const & objectCode;
    Assembler::PrettyPrinter<wchar_t> & printer;
    ProcessorIntel8080 processor;

    bool OnCallback(RegistersIntel8080 const &);
    void PrintRegisterValues(RegistersIntel8080 const & registers);
    void PrintRegister8(std::wstring const & name, Reg8 const & reg);
    void PrintRegister16(std::wstring const & name, Reg16LH const & reg);
    void PrintRegister16(std::wstring const & name, Reg16 const & reg);
}; // CPUEmulatorIntel8080

} // namespace Emulator
