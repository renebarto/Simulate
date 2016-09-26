#pragma once

#include "CPUParserIntel8080_8085.h"
#include "ICPUAssembler.h"
#include "PrettyPrinter.h"

namespace Assembler
{

enum class InstructionOperandIntel8080_8085
{
    Invalid,
    None,               // No operands
    RegM8_D3_RegM8_S0,  // ddd @ bit 3, sss @ bit 0
    RegM8_D0,           // ddd @ bit 0
    RegM8_D3,           // ddd @ bit 3
    RegM8_D3_I8,        // ddd @ bit 3 + 8 bit immediate
    Reg16_D4_I16,       // rp @ bit 4 + 16 bit immediate
    Reg16_D4_BD,        // rp (BC/DE only) @ bit 4
    Reg16_D4_BDHPSW,    // rp (BC/DE/HL/PSW) @ bit 4
    Reg16_D4_BDHSP,     // rp (BC/DE/HL/SP) @ bit 4
    I8,                 // 8 bit immediate
    I16,                // 16 bit immediate
    Rst3,               // nnn @ bit 3
    Simple,
    R8,         // ddd / sss
    R8_D8,       // ddd / sss + 8 bit immediate
    R16_BD,     // rp: BC or DE
    R16_BDH_PSW,// rp: BC, DE, HL or PSW (PUSH/POP)
    R16_BDH_SP, // rp: BC, DE, HL or SP
    R16_BDH_SP_D16,// rp: BC, DE, HL or SP + 16 bit immediate
    D8,         // 8 bit immediate
    P8,         // 8 bit immediate port number
    D16,        // 16 bit immediate
    A16,        // 16 bit immediate address
    N,          // Restart code (0-7)
};

template<class OperandType, class AddressType>
struct InstructionData
{
    uint8_t opcodeByte;
    OperandType operandType;
    uint8_t cycleCount;
    uint8_t cycleCountConditionFailed;
    uint8_t machineCycleCount;
    uint8_t machineCycleCountConditionFailed;
    AddressType instructionSize;
    std::wstring instructionMnemonic;

    bool operator == (InstructionData const & other) const
    {
        return (opcodeByte == other.opcodeByte) &&
               (operandType == other.operandType) &&
               (cycleCount == other.cycleCount) &&
               (cycleCountConditionFailed == other.cycleCountConditionFailed) &&
               (machineCycleCount == other.machineCycleCount) &&
               (machineCycleCountConditionFailed == other.machineCycleCountConditionFailed) &&
               (instructionSize == other.instructionSize) &&
               (instructionMnemonic == other.instructionMnemonic);
    }
};

class CPUAssemblerIntel8080_8085 : public ICPUAssembler
{
public:
	CPUAssemblerIntel8080_8085(std::shared_ptr<ICPUParser> parser, ErrorHandler & errorHandler, PrettyPrinter<wchar_t> & printer);
	virtual ~CPUAssemblerIntel8080_8085();

    bool Generate(MachineCode & machineCode) override;

private:
    using AddressType = uint16_t;
    using InstructionData8080 = InstructionData<InstructionOperandIntel8080_8085, AddressType>;
    void Init();

    CPUType cpuType;
    std::shared_ptr<CPUParserIntel8080_8085> parser;
    AssemblerMessages localErrors;
	ErrorHandler & errorHandler;
    PrettyPrinter<wchar_t> & printer;
	OpcodeMap<OpcodeType, InstructionData8080> instructionData;

    Register8Type ExpectOperandRegister8(ASTNode::Ptr node);
    Register16Type ExpectOperandRegister16(ASTNode::Ptr node);
    uint8_t ExpectOperandData8(ASTNode::Ptr node);
    uint16_t ExpectOperandData16(ASTNode::Ptr node);
    uint8_t ExpectOperandLiteralRst(ASTNode::Ptr node);
    void Error(Location const & location, std::wstring const & message);
}; // CPUAssemblerIntel8080_8085

} // namespace Assembler
