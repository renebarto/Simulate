#include <ram.h>

using namespace Simulate;

RAM::RAM(size_t base, size_t size)
    : base(base)
    , contents(size)
{

}

RAM::~RAM()
{

}

uint8_t RAM::Fetch8(size_t address)
{
    return 0;
}

void RAM::Store8(size_t address, uint8_t data)
{
}

uint16_t RAM::Fetch16(size_t address)
{
    return 0;
}

void RAM::Store16(size_t address, uint16_t data)
{
}

uint32_t RAM::Fetch32(size_t address)
{
    return 0;
}

void RAM::Store32(size_t address, uint32_t data)
{
}

uint64_t RAM::Fetch64(size_t address)
{
    return 0;
}

void RAM::Store64(size_t address, uint64_t data)
{
}
