#pragma once

#include <ostream>

namespace Assembler
{

enum class CPUType
{
    Undefined = 0,
    Intel4004,
    Intel8008,
    Intel8080,
    Intel8085,
};
std::ostream & operator << (std::ostream & stream, CPUType cpuType);
std::wostream & operator << (std::wostream & stream, CPUType cpuType);

} // namespace Assembler
