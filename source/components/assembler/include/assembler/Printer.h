#pragma once

#include "assembler/PrettyPrinter.h"

namespace Assembler
{

extern const size_t AddressColumn;
extern const size_t CodeColumn;
extern const size_t LabelColumn;
extern const size_t OpcodeColumn;
extern const size_t CommentColumn;

template<typename AddressType>
inline void PrintAddress(PrettyPrinter<wchar_t> & stream, AddressType address)
{
    stream << column(AddressColumn) << std::hex << std::uppercase << std::setw(4) << std::setfill(L'0') << address << L" " << std::dec;
}

template<typename MachineCode>
inline void PrintCode(PrettyPrinter<wchar_t> & stream, MachineCode const & code)
{
    stream << column(CodeColumn);
    for (size_t i = 0; i < code.size(); ++i)
    {
        stream << std::hex << std::uppercase << std::setw(2) << int(code[i]) << L" " << std::dec;
    }
}

inline void PrintLabel(PrettyPrinter<wchar_t> & stream, std::wstring const & label = L"")
{
    stream << column(LabelColumn) << label;
}

} // namespace Assembler
