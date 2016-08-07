#pragma once

#include <iomanip>
#include <iostream>
#include <sstream>
#include "simple-processor/processor.h"
#include "osal/flagoperators.h"
#include "core/Observable.h"

namespace Simulate
{

enum class SimpleProcessorFlags : uint8_t
{
    None = 0x00,
    C = 0x01,
    Z = 0x02,
    P = 0x04, 
};
DEFINE_FLAG_OPERATORS(SimpleProcessorFlags, uint8_t);

enum class SimpleProcessorOpcode : uint8_t
{
    NOP = 0x00,
    CLA = 0x01,
    CLC = 0x02,
    CLX = 0x03,
    CMC = 0x04,
    INC = 0x05,
    DEC = 0x06,
    INX = 0x07,
    DEX = 0x08,
    TAX = 0x09,
    INI = 0x0A,
    INH = 0x0B,
    INB = 0x0C,
    INA = 0x0D,
    OTI = 0x0E,
    OTC = 0x0F,
    OTH = 0x10,
    OTB = 0x11,
    OTA = 0x12,
    PSH = 0x13,
    POP = 0x14,
    SHL = 0x15,
    SHR = 0x16,
    RET = 0x17,
    HLT = 0x18,
    LDA = 0x19,
    LDX = 0x1A,
    LDI = 0x1B,
    LSP = 0x1C,
    LSI = 0x1D,
    STA = 0x1E,
    STX = 0x1F,
    ADD = 0x20,
    ADX = 0x21,
    ADI = 0x22,
    ADC = 0x23,
    ACX = 0x24,
    ACI = 0x25,
    SUB = 0x26,
    SBX = 0x27,
    SBI = 0x28,
    SBC = 0x29,
    SCX = 0x2A,
    SCI = 0x2B,
    CMP = 0x2C,
    CPX = 0x2D,
    CPI = 0x2E,
    ANA = 0x2F,
    ANX = 0x30,
    ANI = 0x31,
    ORA = 0x32,
    ORX = 0x33,
    ORI = 0x34,
    BRN = 0x35,
    BZE = 0x36,
    BNZ = 0x37,
    BPZ = 0x38,
    BNG = 0x39,
    BCC = 0x3A,
    BCS = 0x3B,
    JSR = 0x3C,
    BAD_OPCODE = 0xFF,
};

enum SimpleProcessorInstructionOption
{
    d8,
    a8,
};

using SimpleProcessorAddressType = uint8_t;

class SimpleProcessorRegisters : public Registers<SimpleProcessorAddressType>
{
public:
    uint8_t a;
    SimpleProcessorAddressType sp;
    SimpleProcessorAddressType x;
    uint8_t ir;
    SimpleProcessorAddressType pc;
    SimpleProcessorAddressType pcLast;
    SimpleProcessorFlags flags;
    uint8_t operand;

    SimpleProcessorRegisters()
        : a()
        , sp()
        , x()
        , ir()
        , pc()
        , pcLast()
        , flags()
        , operand()
    {
    }
    void Reset() override;
};

class SimpleProcessor : public Processor<SimpleProcessorAddressType, SimpleProcessorOpcode, SimpleProcessorRegisters, InstructionInfo<SimpleProcessorInstructionOption>>
{
public:
    using AddressType = uint8_t;
    using Opcode = SimpleProcessorOpcode;
    using Flags = SimpleProcessorFlags;
    using InstructionOption = SimpleProcessorInstructionOption;
    using Registers = SimpleProcessorRegisters;
    using InstructionInfo = InstructionInfo<SimpleProcessorInstructionOption>;

    SimpleProcessor(double clockFreq,
                    CharReader & reader, 
                    CharWriter & writer);
    virtual ~SimpleProcessor();

    static const uint8_t InitialPC;

    void SimpleProcessor::Reset() override;
    Opcode LookupOpcode(std::string const & str) override;
    InstructionInfo LookupOpcode(uint8_t opcode) override;
    // Maps str to opcode, or to BAD_OPCODE (0FFH) if no match can be found 
    void FetchAndExecute() override;
    void FetchInstruction() override;
    void Execute(uint8_t opcodeByte) override;

    using Processor::Fetch;
    using Processor::Store;

    void SetFlags(uint8_t value);
    void Load(uint8_t value);
    void Store(uint8_t value);
    void Add(uint8_t value);
    void AddC(uint8_t value);
    void Sub(uint8_t value);
    void SubC(uint8_t value);
    void Cmp(uint8_t value);
    void And(uint8_t value);
    void Or(uint8_t value);
    uint8_t Index(uint8_t index);
    uint8_t GetNumber(int radix);
    uint8_t GetChar();
    void OutputNumber(int value, int radix);
    void OutputNumber(uint8_t value, int radix);
    void OutputChar(char value);
};

static std::ostream & operator << (std::ostream & stream, SimpleProcessor::Flags flags)
{
    std::string str;
    if ((flags & SimpleProcessor::Flags::C) != SimpleProcessor::Flags::None)
    {
        str += "C";
    }
    if ((flags & SimpleProcessor::Flags::Z) != SimpleProcessor::Flags::None)
    {
        if (!str.empty())
            str += "|";
        str += "Z";
    }
    if ((flags & SimpleProcessor::Flags::P) != SimpleProcessor::Flags::None)
    {
        if (!str.empty())
            str += "|";
        str += "P";
    }
    if (flags == SimpleProcessor::Flags::None)
    {
        str = "-";
    }
    return stream << str;
}

static std::ostream & operator << (std::ostream & stream, SimpleProcessorInstructionOption option)
{
    switch (option)
    {
    case SimpleProcessorInstructionOption::d8:
        stream << "data8";
        break;
    case SimpleProcessorInstructionOption::a8:
        stream << "address8";
        break;
    }
    return stream;
}

static std::ostream & operator << (std::ostream & stream, SimpleProcessor::Registers const & registers)
{
    stream << "  A    =" << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << int(registers.a) << std::endl
           << "  X    =" << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << int(registers.x) << std::endl
           << "  IR   =" << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << int(registers.ir) << std::endl
           << "  PC   =" << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << int(registers.pc) << std::endl
           << "  SP   =" << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << int(registers.sp) << std::endl
           << "  F    =" << registers.flags << std::endl
           << "  State=" << registers.state << std::endl
           << "  Clk  =" << std::dec << registers.totalClockCount << std::endl;
    return stream;
}

} // namespace Simulate
