#include <ioport.h>

using namespace Simulate;

IOPort::IOPort(size_t base, size_t size)
    : base(base)
    , ports(size)
{

}

IOPort::~IOPort()
{

}

uint8_t IOPort::In8(size_t address)
{
    return 0;
}

void IOPort::Out8(size_t address, uint8_t data)
{
}

uint16_t IOPort::In16(size_t address)
{
    return 0;
}

void IOPort::Out16(size_t address, uint16_t data)
{
}

uint32_t IOPort::In32(size_t address)
{
    return 0;
}

void IOPort::Out32(size_t address, uint32_t data)
{
}

uint64_t IOPort::In64(size_t address)
{
    return 0;
}

void IOPort::Out64(size_t address, uint64_t data)
{
}
