#include "CPUType.h"

namespace Assembler
{

std::ostream & operator << (std::ostream & stream, CPUType cpuType)
{
    switch(cpuType)
    {
    case CPUType::Intel4004:
        stream << "Intel 4004";
    case CPUType::Intel8008:
        stream << "Intel 8008";
    case CPUType::Intel8080:
        stream << "Intel 8080";
    case CPUType::Intel8085:
        stream << "Intel 8085";
    default:
        stream << "Undefined CPU type!";
    }
    return stream;
}

std::wostream & operator << (std::wostream & stream, CPUType cpuType)
{
    switch(cpuType)
    {
    case CPUType::Intel4004:
        stream << "Intel 4004";
    case CPUType::Intel8008:
        stream << "Intel 8008";
    case CPUType::Intel8080:
        stream << "Intel 8080";
    case CPUType::Intel8085:
        stream << "Intel 8085";
    default:
        stream << "Undefined CPU type!";
    }
    return stream;
}

} // namespace Assembler

