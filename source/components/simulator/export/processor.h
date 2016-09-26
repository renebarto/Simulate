#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <vector>
//#include <memorymanager.h>
//#include <iomanager.h>
#include <core/Util.h>

namespace Simulate
{

class DisassemblerUnknownInstructionException : public std::runtime_error
{
public:
    DisassemblerUnknownInstructionException(uint8_t opcodeByte)
        : std::runtime_error("Unknown instruction: 0x" + Core::Util::ToString(opcodeByte, 16))
    {
    }
};

class AssemblerUnknownOpcodeException : public std::runtime_error
{
public:
    AssemblerUnknownOpcodeException(std::string const & opcodeString)
        : std::runtime_error("Unknown opcode: " + opcodeString)
    {

    }
};

class AssemblerInvalidOptionException : public std::runtime_error
{
public:
    AssemblerInvalidOptionException(std::string const & expected, std::string const & actual)
        : std::runtime_error("Invalid option: Expected " + expected + " have " + actual)
    {

    }
};

using Reg8 = uint8_t;
using Reg16 = uint16_t;
using Reg32 = uint32_t;
using Reg64 = uint64_t;

#pragma pack(push, 1)
typedef union
{
    struct { Reg8 l, h; } B;
    Reg16 W;
} Reg16LH;

typedef union
{
    struct { Reg16 l, h; } B;
    Reg32 W;
} Reg32LH;

typedef union
{
    struct { Reg32 l, h; } B;
    Reg64 W;
} Reg64LH;
#pragma pack(pop)

class Registers
{
public:
    Registers() {};
    virtual ~Registers() {};
};

struct RegisterInfo8
{
    Reg8 * data;
    std::string name;
};

struct RegisterInfo16
{
    Reg16 * data;
    std::string name;
};

class Flags
{
};

using picoseconds = std::chrono::duration<int64_t, std::pico>;

class Clock
{
public:
    Clock() = delete;
    Clock(picoseconds clockInterval)
        : clockintervalPS(clockInterval)
    {
    }
    virtual ~Clock() {};

protected:
    picoseconds clockintervalPS;
};

class Peripheral
{
public:
    Peripheral() {};
    virtual ~Peripheral() {};
};

class Processor
{
public:
    using DebugCallback = std::function<bool(int, Registers const &)>;

    Processor(double clockFreq,
              uint8_t addressBusWidth,
              uint8_t dataBusWidth);
    virtual ~Processor();

    virtual void Setup(MemoryManagerPtr memoryManager, IOManagerPtr ioManager) = 0;
    virtual void Reset() = 0;
    virtual void FetchInstruction() = 0;
    virtual void ExecuteInstruction() = 0;

    virtual bool RunInstruction() = 0;
    virtual void Run() = 0;

    virtual Registers & GetRegisters() = 0;

    virtual size_t DisassembleInstruction(std::vector<uint8_t> const & machineCode, std::string & mnemonic) = 0;
    virtual size_t AssembleInstruction(std::string const & mnemonic, std::vector<uint8_t> & machineCode) = 0;
    virtual void Disassemble(std::vector<uint8_t> const & machineCode, std::ostream & disassembledCode) = 0;
    virtual void Assemble(std::string const & disassembledCode, std::vector<uint8_t> & machineCode) = 0;

    virtual void PrintRegisterValues(std::ostream & stream) = 0;
    virtual std::vector<std::string> GetRegisterNames() = 0;

    MemoryManagerPtr GetMemoryManager() const { return memory; }
    IOManagerPtr GetIOManager() const { return io; }

    void SetupDebug(DebugCallback const & callback)
    {
        debugEnabled = true;
        debugCallback = callback;
    }
    void StopDebug()
    {
        debugEnabled = false;
        debugCallback = nullptr;
    }

protected:
    Clock clock;
    MemoryManagerPtr memory;
    IOManagerPtr io;
    uint8_t addressBusWidth;
    uint8_t dataBusWidth;
    bool debugEnabled;
    DebugCallback debugCallback;
};

} // namespace Simulate
