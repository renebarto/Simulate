#include "Printer.h"

namespace Assembler
{

static const size_t BytesPerInstructionMax = 3;
static const size_t AddressWidth = 5;
static const size_t ByteWidth = 3;
static const size_t LabelWidth = 20;
static const size_t OpcodeWidth = 20;
const size_t AddressColumn = 1;
const size_t CodeColumn = AddressColumn + AddressWidth;
const size_t LabelColumn = CodeColumn + BytesPerInstructionMax * ByteWidth;
const size_t OpcodeColumn = LabelColumn + LabelWidth;
const size_t CommentColumn = OpcodeColumn + OpcodeWidth;

} // namespace Assembler

