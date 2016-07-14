#include <system.h>

using namespace Simulate;

System::System()
    : ActiveObject("System")
{

}

System::~System()
{

}

void System::Start()
{
    Create();
}

void System::Stop()
{
    Kill();
}

void System::InitThread()
{
}

void System::Run()
{
    while (!IsDying())
    {
        processor->FetchInstruction();
        processor->ExecuteInstruction();

        ExecuteCyclicTask();
        Wait();
    }
}

void System::ExitThread()
{
}

void System::FlushThread()
{
}

