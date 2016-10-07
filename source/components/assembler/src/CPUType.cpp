#include "assembler/CPUType.h"

namespace Assembler
{

std::ostream & operator << (std::ostream & stream, CPUType cpuType)
{
    switch(cpuType)
    {
    case CPUType::Intel4004:
        stream << "Intel4004";
        break;
    case CPUType::Intel8008:
        stream << "Intel8008";
        break;
    case CPUType::Intel8080:
        stream << "Intel8080";
        break;
    case CPUType::Intel8085:
        stream << "Intel8085";
        break;
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
        stream << L"Intel4004";
        break;
    case CPUType::Intel8008:
        stream << L"Intel8008";
        break;
    case CPUType::Intel8080:
        stream << L"Intel8080";
        break;
    case CPUType::Intel8085:
        stream << L"Intel8085";
        break;
    default:
        stream << L"Undefined CPU type!";
    }
    return stream;
}

} // namespace Assembler

