#pragma once

#include <memory>
#include <core/ActiveObject.h>
#include <iomanager.h>
#include <processor.h>
#include <memorymanager.h>

namespace Simulate
{

class System : public Core::ActiveObject
{
public:
    System();
    virtual ~System();

    void Start();
    void Stop();

    void InitThread() override;
    void Run() override;
    void ExitThread() override;
    void FlushThread() override;

    virtual void ExecuteCyclicTask() = 0;
    virtual void Wait() = 0;


private:
    std::shared_ptr<Processor> processor;
    std::shared_ptr<MemoryManager> rom;
    std::shared_ptr<IOManager> io;
    std::vector<std::unique_ptr<Peripheral>> peripherals;
};

} // namespace Simulate
