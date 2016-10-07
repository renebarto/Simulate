#include "assembler/Exceptions.h"

#include "core/String.h"

namespace Assembler
{

AssemblerException::AssemblerException(std::wstring const & message)
    : std::runtime_error(Core::String::ToString(message))
{
}

} // namespace Assembler
