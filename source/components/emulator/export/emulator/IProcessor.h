#pragma once

#include <functional>
#include "emulator/IOManager.h"
#include "emulator/MemoryManager.h"

namespace Emulator
{

template<class Registers, class MemoryAddressType, class IOAddressType>
class IProcessor
{
public:
    using DebugCallback = std::function<bool(Registers const &)>;

    IProcessor() {}
    virtual ~IProcessor() {}

    virtual void Setup(MemoryManagerPtr memoryManager, IOManagerPtr ioManager) = 0;

    virtual void Reset() = 0;
    virtual void FetchInstruction() = 0;
    virtual void ExecuteInstruction() = 0;

    virtual bool RunInstruction() = 0;
    virtual void Run() = 0;

    virtual Registers & GetRegisters() = 0;
    virtual MemoryManagerPtr GetMemoryManager() const = 0;
    virtual IOManagerPtr GetIOManager() const = 0;

    virtual void SetupDebug(DebugCallback const & callback) = 0;
    virtual void StopDebug() = 0;

}; // IProcessor

} // namespace Emulator
