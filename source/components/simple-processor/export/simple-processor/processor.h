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

class UnassignedMemoryException : public std::runtime_error
{
public:
    UnassignedMemoryException()
        : std::runtime_error("Memory has not been assigned")
    {
    }
};

enum class DebugMode : uint8_t
{
    None = 0x00,
    Trace = 0x01,
    NonRealTime = 0x02,
};
DEFINE_FLAG_OPERATORS(DebugMode, uint8_t);

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

template <class InstructionInfo, class Registers>
class IDebugger
{
public:
    virtual ~IDebugger() {}
    virtual void Reset() = 0;
    virtual void Trace(InstructionInfo const & info, Registers const & registers) = 0;
};

enum class State : uint8_t
{
    Uninitialized,
    Running,
    Halted,
    NoData,
    BadData,
    BadOpcode,
};

using ClockCount = uint64_t;

template <class AddressType>
class Registers
{
public:
    ClockCount totalClockCount;
    State state;

    Registers()
        : totalClockCount()
        , state(State::Uninitialized)
    {
    }
    virtual void Reset();
};

template <class AddressType>
void Registers<AddressType>::Reset()
{
    totalClockCount = 0;
    state = State::Uninitialized;
}

template<class InstructionOption>
class InstructionInfo
{
public:
    using InstructionOptions = std::vector<InstructionOption>;
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

template <class AddressType, class Opcode, class Registers, class InstructionInfo>
class Processor : public Core::Observable<IDebugger<InstructionInfo, Registers>>
{
public:
    using picoseconds = std::chrono::duration<int64_t, std::pico>;

    class Clock
    {
    public:
        Clock() = delete;
        Clock(picoseconds clockInterval);
        virtual ~Clock();
        virtual void Reset();
        void Wait(uint64_t clockCount);

    protected:
        picoseconds clockintervalPS;
        uint64_t lastClockCount;
        std::chrono::high_resolution_clock clock;
        std::chrono::high_resolution_clock::time_point lastTime;
    };

    Processor(double clockFreq,
              CharReader & reader, 
              CharWriter & writer);
    virtual ~Processor();

    virtual void Processor::Reset();
    bool IsHalted() { return registers.state == State::Halted; }
    virtual Opcode LookupOpcode(std::string const & str) = 0;
    virtual InstructionInfo LookupOpcode(uint8_t opcode) = 0;
    virtual void FetchAndExecute() = 0;
    virtual void FetchInstruction() = 0;
    virtual void Execute(uint8_t opcodeByte) = 0;
    void SetDebugMode(DebugMode value) { debugMode = value; }

    void ClearMemory()
    {
        if (memory == nullptr)
            throw UnassignedMemoryException();
        memory->Clear();
    }
    IMemory<uint8_t> const & GetMemory() const
    {
        if (memory == nullptr)
            throw UnassignedMemoryException();
        return *memory;
    }
    uint8_t Fetch(AddressType address) const
    {
        if (memory == nullptr)
            throw UnassignedMemoryException();
        return memory->Fetch(address);
    }
    void Store(AddressType address, uint8_t value)
    {
        if (memory == nullptr)
            throw UnassignedMemoryException();
        memory->Store(address, value);
    }

protected:
    Clock clock;
    Registers registers;
    IMemory<uint8_t> * memory;
    CharReader & reader;
    CharWriter & writer;
    DebugMode debugMode;

    void SetMemory(IMemory<uint8_t> * memory)
    {
        this->memory = memory;
    }
};


template <class AddressType, class Opcode, class Registers, class InstructionInfo>
Processor<AddressType, Opcode, Registers, InstructionInfo>::Clock::Clock(picoseconds clockInterval)
    : clockintervalPS(clockInterval)
    , clock()
    , lastClockCount()
    , lastTime()
{
    lastClockCount = 0;
    lastTime = clock.now();
}

template <class AddressType, class Opcode, class Registers, class InstructionInfo>
Processor<AddressType, Opcode, Registers, InstructionInfo>::Clock::~Clock()
{
}

template <class AddressType, class Opcode, class Registers, class InstructionInfo>
void Processor<AddressType, Opcode, Registers, InstructionInfo>::Clock::Reset()
{
    lastClockCount = 0;
    lastTime = clock.now();
}

template <class AddressType, class Opcode, class Registers, class InstructionInfo>
void Processor<AddressType, Opcode, Registers, InstructionInfo>::Clock::Wait(uint64_t clockCount)
{
    chrono::high_resolution_clock::time_point deadline = lastTime + chrono::duration_cast<chrono::nanoseconds>((clockCount - lastClockCount) * clockintervalPS);
    chrono::high_resolution_clock::time_point now = clock.now();
    if (now < deadline)
    {
        this_thread::sleep_for(chrono::duration_cast<chrono::nanoseconds>(deadline - now));
    }
    lastTime = clock.now();
    lastClockCount = clockCount;
}

template <class AddressType, class Opcode, class Registers, class InstructionInfo>
Processor<AddressType, Opcode, Registers, InstructionInfo>::Processor(double clockFreq,
                                          CharReader & reader, 
                                          CharWriter & writer)
    : clock(picoseconds(int64_t(tera::num / clockFreq + 0.5)))
    , registers()
    , memory(nullptr)
    , reader(reader)
    , writer(writer)
    , debugMode(DebugMode::None)
{
}

template <class AddressType, class Opcode, class Registers, class InstructionInfo>
Processor<AddressType, Opcode, Registers, InstructionInfo>::~Processor()
{
}

template <class AddressType, class Opcode, class Registers, class InstructionInfo>
void Processor<AddressType, Opcode, Registers, InstructionInfo>::Reset()
{
    registers.Reset();
    clock.Reset();
    if ((debugMode & DebugMode::Trace) != 0)
    {
        for (auto observer : observers)
        {
            observer->Reset();
        }
    }
}

} // namespace Simulate
