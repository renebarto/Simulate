#include <rom.h>

using namespace Simulate;

ROM::ROM(size_t base, size_t size)
    : base(base)
    , contents(size)
{
}

ROM::~ROM()
{

}

void ROM::Load(std::initializer_list<uint8_t> data)
{
    size_t size = data.size();
    if (size > contents.size())
        size = contents.size();
    std::copy(data.begin(), data.begin() + size, contents.begin());
    if (size < contents.size())
    {
        std::fill(contents.begin() + size, contents.end(), 0);
    }
}

uint8_t ROM::Fetch8(size_t address)
{
    return 0;
}

uint16_t ROM::Fetch16(size_t address)
{
    return 0;
}

uint32_t ROM::Fetch32(size_t address)
{
    return 0;
}

uint64_t ROM::Fetch64(size_t address)
{
    return 0;
}
