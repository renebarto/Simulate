#include "assembler/CharClass.h"

namespace Assembler
{

CharClass::CharClass(std::wstring const & name, CharSet const & set, size_t id)
    : name(name)
    , set(set)
    , n(id)
{
}

} // namespace Assembler
