#include <processor8088.h>

#include <core/Util.h>

using namespace Simulate;

static const uint8_t OpcodeMask = 0xF0;
static const uint8_t WordMask = 0x08;
static const uint8_t RegisterMask = 0x07;

uint32_t CombineAddress(Reg16 section, Reg16 offset)
{
    return section << 4 | offset;
}

uint32_t CombineAddress(Reg16 section, Reg16LH offset)
{
    return CombineAddress(section, offset.h << 8 | offset.l);
}

Processor8088::Processor8088()
    : Processor(20, 8, 5)
{
    registers8 = new RegisterInfo8[8]
    {
        { &registers.ax.l, "AL" },
        { &registers.cx.l, "CL" },
        { &registers.dx.l, "DL" },
        { &registers.bx.l, "BL" },
        { &registers.ax.h, "AH" },
        { &registers.cx.h, "CH" },
        { &registers.dx.h, "DH" },
        { &registers.bx.h, "BH" },
    };
    registers16 = new RegisterInfo16[8]
    {
        { (Reg16 *)&registers.ax, "AX" },
        { (Reg16 *)&registers.cx, "CX" },
        { (Reg16 *)&registers.dx, "DX" },
        { (Reg16 *)&registers.bx, "BX" },
        { &registers.sp, "SP" },
        { &registers.bp, "BP" },
        { &registers.si, "SI" },
        { &registers.di, "DI" },
    };
    registersSeg = new RegisterInfo16[4]
    {
        { &registers.cs, "CS" },
        { &registers.ds, "DS" },
        { &registers.ss, "SS" },
        { &registers.es, "ES" },
    };
}

Processor8088::~Processor8088()
{
    if (registers16)
    {
        delete[] registers16;
        registers16 = nullptr;
    }
    if (registers8)
    {
        delete[] registers8;
        registers8 = nullptr;
    }
}

void Processor8088::Setup(MemoryManagerPtr memoryManager, IOManagerPtr ioManager)
{
    memory = memoryManager;
    io = ioManager;
}

void Processor8088::Reset()
{
    registers.cs = 0xFFFF;
    registers.ip = 0x0000;
}

uint8_t Processor8088::FetchInstructionByte()
{
    uint8_t instructionByte = memory->Fetch8(CombineAddress(registers.cs, registers.ip));
    registers.ip++;
    return instructionByte;
}

void Processor8088::FetchInstruction()
{
    instruction.clear();
    instruction.push_back(FetchInstructionByte());
    registers.ip++;

    switch (instruction[0] & OpcodeMask)
    {
    case 0xB0:
        instruction.push_back(FetchInstructionByte());
        if (instruction[0] & WordMask)
        {
            instruction.push_back(FetchInstructionByte());
        }
    }
}

void Processor8088::DecodeInstruction()
{
    switch (instruction[0] & OpcodeMask)
    {
    case 0xB0:
        if (instruction[0] & WordMask)
        {
            opcode = Opcode8088::MOV_Direct16;
            operand16 = &registers16[instruction[0] & RegisterMask];
        }
        else
        {
            opcode = Opcode8088::MOV_Direct8;
            operand8 = &registers8[instruction[0] & RegisterMask];
        }
        break;

    }
}

void Processor8088::ExecuteInstruction()
{
}

void Processor8088::DisassembleInstruction(std::vector<uint8_t> const & machineCode, std::string & mnemonic)
{
    instruction = machineCode;
    DecodeInstruction();
    mnemonic = CreateMnemonic();
}

void Processor8088::AssembleInstruction(std::vector<uint8_t> & machineCode, std::string const & mnemonic)
{

}

uint16_t Processor8088::PopStack16()
{
    return 0;
}

void Processor8088::PushStack16(uint16_t value)
{
}

void Processor8088::PrintRegisterValues(std::ostream & stream)
{

}

std::vector<std::string> Processor8088::GetRegisterNames()
{
    std::vector<std::string> names;
    for (int i = 0; i < 8; i++)
    {
        names.push_back(registers8[i].name);
    }
    for (int i = 0; i < 8; i++)
    {
        names.push_back(registers16[i].name);
    }
    for (int i = 0; i < 4; i++)
    {
        names.push_back(registersSeg[i].name);
    }
    names.push_back("IP");
    names.push_back("IR");
    return names;
}

std::string Processor8088::CreateMnemonic()
{
    std::string mnemonic;
    switch (opcode)
    {
    case Opcode8088::MOV_Direct8:
        mnemonic = "MOV ";
        mnemonic += operand8->name;
        mnemonic += ",";
        mnemonic += Core::Util::ToString(instruction[1], 16);
        break;
    case Opcode8088::MOV_Direct16:
        mnemonic = "MOV ";
        mnemonic += operand16->name;
        mnemonic += ",";
        mnemonic += Core::Util::ToString(instruction[2], 16);
        mnemonic += Core::Util::ToString(instruction[1], 16);
        break;
    }
    return mnemonic;
}