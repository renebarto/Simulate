#pragma once

#include <iomanip>
#include <iostream>
#include <sstream>
#include "simple-processor/imemory.h"
#include "osal/flagoperators.h"
#include "core/Observable.h"

namespace Simulate
{

class IllegalInstructionException : public std::runtime_error
{
public:
    IllegalInstructionException(uint8_t opcodeByte)
        : std::runtime_error("")
        , opcodeByte(opcodeByte)
        , message()
    {
    }
    char const * what() const override
    {
        std::ostringstream stream;
        stream << "Illegal instruction: " << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << int(opcodeByte);
        message = stream.str();
        return message.c_str();
    }
private:
    uint8_t opcodeByte;
    mutable std::string message;
};

enum class Flags : uint8_t
{
    None = 0x00,
    C = 0x01,
    Z = 0x02,
    P = 0x04, 
};
DEFINE_FLAG_OPERATORS(Flags, uint8_t);

enum class State : uint8_t
{
    Uninitialized,
    Running,
    Halted,
    NoData,
    BadData,
    BadOpcode,
};

class CharReader
{
public:
    virtual ~CharReader() {}
    virtual char ReadChar() = 0;
    virtual bool NoMoreData() = 0;
};

class CharWriter
{
public:
    virtual ~CharWriter() {}
    virtual void ClearContents() = 0;
    virtual void WriteChar(char data) = 0;
};

class IDebugger
{
public:
    virtual ~IDebugger() {}
    virtual void Reset() = 0;
    virtual void Trace() = 0;
};

class SimpleProcessor : public Core::Observable<IDebugger>
{
public:
    enum class Opcode : uint8_t
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
    enum InstructionOption
    {
        d8,
        a8,
    };
    struct Registers
    {
        uint8_t a;
        uint8_t sp;
        uint8_t x;
        uint8_t ir;
        uint8_t pc;
        Flags flags;
        uint64_t totalClockCount;
        State state;

        Registers()
            : a()
            , sp()
            , x()
            , ir()
            , pc()
            , flags()
            , totalClockCount()
            , state(State::Uninitialized)
        {
        }
        void Reset();
    };

    using InstructionOptions = std::vector<InstructionOption>;
    struct InstructionInfo
    {
        uint8_t opcodeByte;
        uint8_t cycleCount;
        uint8_t cycleCountConditionFailed;
        uint8_t machineCycleCount;
        uint8_t machineCycleCountConditionFailed;
        size_t instructionSize;
        InstructionOptions options;
        std::string instructionMnemonic;

        bool operator == (InstructionInfo const & other) const
        {
            return (opcodeByte == other.opcodeByte) &&
                   (cycleCount == other.cycleCount) &&
                   (cycleCountConditionFailed == other.cycleCountConditionFailed) &&
                   (machineCycleCount == other.machineCycleCount) &&
                   (machineCycleCountConditionFailed == other.machineCycleCountConditionFailed) &&
                   (instructionSize == other.instructionSize) &&
                   (options == other.options) &&
                   (instructionMnemonic == other.instructionMnemonic);
        }
    };
    SimpleProcessor(IMemory<uint8_t> & memory, CharReader & reader, CharWriter & writer);
    virtual ~SimpleProcessor();

    static const uint8_t InitialPC;

    void SimpleProcessor::Reset();
    bool IsHalted() { return registers.state == State::Halted; }
    static SimpleProcessor::Opcode LookupOpcode(std::string const & str);
    static SimpleProcessor::InstructionInfo LookupOpcode(uint8_t opcode);
    // Maps str to opcode, or to BAD_OPCODE (0FFH) if no match can be found 
    void FetchAndExecute();
    void FetchInstruction();
    void Execute(uint8_t opcodeByte);
    void SetTracing(bool tracingOn) { tracing = tracingOn; }

    void SetFlags(uint8_t value);
    void ClearMemory();
    IMemory<uint8_t> const & GetMemory() const { return memory;}
    uint8_t Fetch(uint8_t address) const;
    void Store(uint8_t address, uint8_t value);
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

protected:
    Registers registers;
    IMemory<uint8_t> & memory;
    CharReader & reader;
    CharWriter & writer;
    bool tracing;
};

static std::ostream & operator << (std::ostream & stream, Flags flags)
{
    std::string str;
    if ((flags & Flags::C) != Flags::None)
    {
        str += "C";
    }
    if ((flags & Flags::Z) != Flags::None)
    {
        if (!str.empty())
            str += "|";
        str += "Z";
    }
    if ((flags & Flags::P) != Flags::None)
    {
        if (!str.empty())
            str += "|";
        str += "P";
    }
    if (flags == Flags::None)
    {
        str = "-";
    }
    return stream << str;
}

static std::ostream & operator << (std::ostream & stream, SimpleProcessor::InstructionOption option)
{
    switch (option)
    {
    case SimpleProcessor::InstructionOption::d8:
        stream << "data8";
        break;
    case SimpleProcessor::InstructionOption::a8:
        stream << "address8";
        break;
    }
    return stream;
}

} // namespace Simulate
