#include <iomanager.h>

using namespace Simulate;

IOManager::IOManager(std::vector<std::shared_ptr<IIO>> ioPorts)
    : ioPorts(ioPorts)
{

}

IOManager::~IOManager()
{

}

uint8_t IOManager::In8(size_t address)
{
    return 0;
}

void IOManager::Out8(size_t address, uint8_t data)
{
}

uint16_t IOManager::In16(size_t address)
{
    return 0;
}

void IOManager::Out16(size_t address, uint16_t data)
{
}

uint32_t IOManager::In32(size_t address)
{
    return 0;
}

void IOManager::Out32(size_t address, uint32_t data)
{
}

uint64_t IOManager::In64(size_t address)
{
    return 0;
}

void IOManager::Out64(size_t address, uint64_t data)
{
}
