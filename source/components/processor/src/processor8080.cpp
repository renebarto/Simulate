#include <processor8080.h>

#include <iomanip>
#include <iostream>
#include <sstream>

namespace Simulate
{

static const Reg16 InitialPC = 0;

enum InstructionOption8080
{
    reg16_4,
    reg16_psw_4,
    regm8_s0,
    regm8_d0,
    regm8_d3,
    n3,
    d8,
    p8,
    d16,
    a16,
};

static std::ostream & operator << (std::ostream & stream, InstructionOption8080 option)
{
    switch (option)
    {
    case InstructionOption8080::reg16_4:
        stream << "reg16";
        break;
    case InstructionOption8080::reg16_psw_4:
        stream << "reg16 or PSW";
        break;
    case InstructionOption8080::regm8_s0:
        stream << "regm8";
        break;
    case InstructionOption8080::regm8_d0:
        stream << "regm8";
        break;
    case InstructionOption8080::regm8_d3:
        stream << "regm8";
        break;
    case InstructionOption8080::n3:
        stream << "n";
        break;
    case InstructionOption8080::p8:
        stream << "port";
        break;
    case InstructionOption8080::d8:
        stream << "data8";
        break;
    case InstructionOption8080::d16:
        stream << "data16";
        break;
    case InstructionOption8080::a16:
        stream << "address16";
        break;
    }
    return stream;
}

using InstructionOptions8080 = std::vector<InstructionOption8080>;

enum class OpcodesRaw8080 : uint8_t
{
    NOP = 0x00,
    LXI = 0x01,
    STAX = 0x02,
    INX = 0x03,
    INR = 0x04,
    DCR = 0x05,
    RLC = 0x07,
    DAD = 0x09,
    LDAX = 0x0A,
    DCX = 0x0B,
    RRC = 0x0F,
    RAL = 0x17,
    RAR = 0x1F,
    SHLD = 0x22,
    DAA = 0x27,
    LHLD = 0x2A,
    CMA = 0x2F,
    STA = 0x32,
    MVI = 0x36,
    STC = 0x37,
    LDA = 0x3A,
    CMC = 0x3F,
    MOV = 0x40,
    HLT = 0x76,
    ADD = 0x80,
    ADC = 0x88,
    SUB = 0x90,
    SBB = 0x98,
    ANA = 0xA0,
    XRA = 0xA8,
    ORA = 0xB0,
    CMP = 0xB8,
    RNZ = 0xC0, 
    POP = 0xC1, 
    JNZ = 0xC2, 
    JMP = 0xC3, 
    CNZ = 0xC4, 
    PUSH = 0xC5, 
    ADI = 0xC6, 
    RST = 0xC7, 
    RZ = 0xC8, 
    RET = 0xC9, 
    JZ = 0xCA, 
    CZ = 0xCC, 
    CALL = 0xCD, 
    ACI = 0xCE, 
    RNC = 0xD0, 
    JNC = 0xD2, 
    OUTP = 0xD3, 
    CNC = 0xD4, 
    SUI = 0xD6, 
    RC = 0xD8, 
    JC = 0xDA, 
    INP = 0xDB, 
    CC = 0xDC, 
    SBI = 0xDE, 
    RPO = 0xE0, 
    JPO = 0XE2, 
    XTHL = 0xE3, 
    CPO = 0xE4, 
    ANI = 0xE6, 
    RPE = 0xE8, 
    PCHL = 0xE9, 
    JPE = 0xEA, 
    XCHG = 0xEB,
    CPE = 0xEC, 
    XRI = 0xEE, 
    RP = 0xF0, 
    JP = 0xF2, 
    DI = 0xF3, 
    CP = 0xF4, 
    ORI = 0xF6, 
    RM = 0xF8, 
    SPHL = 0xF9, 
    JM = 0xFA, 
    EI = 0xFB, 
    CM = 0xFC, 
    CPI = 0xFE, 
// 8085 only
    DSUB = 0x08, // undocumented: HL = HL - BC . Z, S, P, CY, AC and X5, V all flag receives influence
    ARHL = 0x10, // undocumented: Rotate HL right. 16-bit rotation, flags unchanged.HL = HL/2 and CY = L0. 
                 // ARHL order is arithmetic right shift of HL register pair. MSB of the H register which means the mark does not change. 
                 // The least significant bit of L register enters into the CY flag. Just the CY flag changes.
    RDEL = 0x18, // undocumented: Rotate DE left. Bit 15 to Carry. No other flags. DE = DE*2 and E0=CY and CY =D7
    RIM = 0x20,  // Read interrupt mask
    LDHI = 0x28, // undocumented: Add 00bb immediate to HL, setting flags. DE = HL + imm
    SIM = 0x30,  // Set interrupt mask
    LDSI = 0x38, // undocumented: Add 00bb immediate to stack pointer, setting flags. DE < - SP + imm
    RSTV = 0xCB, // undocumented: RST 8 (to 0040) if the V flag is set. push PC and PC = 0040H
    SHLX = 0xD9, // undocumented: LD (DE),HL. [DE] = HL
    JNK = 0xDD,  // undocumented: Jump to location addr if K flag is reset. if (!KFLag) PC = addr
    LHLX = 0xED, // undocumented: LD HL,(DE). HL=[DE]
    JK = 0xFD,   // undocumented: Jump to location addr if K flag is set. if (KFLag) PC = addr
};

enum ProcessorType
{
    Both,
    Only8085,
};

enum Reg8Selector
{
    B,
    C,
    D,
    E,
    H,
    L,
    M,
    A,
};

enum Reg16Selector
{
    BC,
    DE,
    HL,
    SP,
};

struct InstructionParserData
{
    OpcodesRaw8080 opcodeByte;
    ProcessorType processor;
    std::string instructionMnemonic;
    InstructionOptions8080 instructionOptions;
};

static const InstructionParserData instructionParserData[] =
{
    { OpcodesRaw8080::NOP, ProcessorType::Both, "NOP", {} },
    { OpcodesRaw8080::LXI, ProcessorType::Both, "LXI", { InstructionOption8080::reg16_4, InstructionOption8080::d16 } },
    { OpcodesRaw8080::STAX, ProcessorType::Both, "STAX", { InstructionOption8080::reg16_4 } },
    { OpcodesRaw8080::INX, ProcessorType::Both, "INX", { InstructionOption8080::reg16_4 } },
    { OpcodesRaw8080::INR, ProcessorType::Both, "INR", { InstructionOption8080::regm8_d3 } },
    { OpcodesRaw8080::DCR, ProcessorType::Both, "DCR", { InstructionOption8080::regm8_d3 } },
    { OpcodesRaw8080::RLC, ProcessorType::Both, "RLC", { } },
    { OpcodesRaw8080::DSUB, ProcessorType::Only8085, "DSUB", {} },
    { OpcodesRaw8080::DAD, ProcessorType::Both, "DAD", { InstructionOption8080::reg16_4 } },
    { OpcodesRaw8080::LDAX, ProcessorType::Both, "LDAX", { InstructionOption8080::reg16_4 } },
    { OpcodesRaw8080::DCX, ProcessorType::Both, "DCX", { InstructionOption8080::reg16_4 } },
    { OpcodesRaw8080::RRC, ProcessorType::Both, "RRC", {} },
    { OpcodesRaw8080::ARHL, ProcessorType::Only8085, "ARHL", {} },
    { OpcodesRaw8080::RAL, ProcessorType::Both, "RAL", {} },
    { OpcodesRaw8080::RDEL, ProcessorType::Only8085, "RDEL", {} },
    { OpcodesRaw8080::RAR, ProcessorType::Both, "RAR", {} },
    { OpcodesRaw8080::RIM, ProcessorType::Only8085, "RIM", {} },
    { OpcodesRaw8080::SHLD, ProcessorType::Both, "SHLD", { InstructionOption8080::a16 } },
    { OpcodesRaw8080::DAA, ProcessorType::Both, "DAA", {} },
    { OpcodesRaw8080::LDHI, ProcessorType::Only8085, "LDHI", { InstructionOption8080::d8 } },
    { OpcodesRaw8080::LHLD, ProcessorType::Both, "LHLD", { InstructionOption8080::a16 } },
    { OpcodesRaw8080::CMA, ProcessorType::Both, "CMA", {} },
    { OpcodesRaw8080::SIM, ProcessorType::Only8085, "SIM", {} },
    { OpcodesRaw8080::STA, ProcessorType::Both, "STA", { InstructionOption8080::a16 } },
    { OpcodesRaw8080::MVI, ProcessorType::Both, "MVI", { InstructionOption8080::regm8_d3, InstructionOption8080::d8 } },
    { OpcodesRaw8080::STC, ProcessorType::Both, "STC", {} },
    { OpcodesRaw8080::LDSI, ProcessorType::Only8085, "LDSI", { InstructionOption8080::d8 } },
    { OpcodesRaw8080::LDA, ProcessorType::Both, "LDA", { InstructionOption8080::a16 } },
    { OpcodesRaw8080::CMC, ProcessorType::Both, "CMC", { } },
    { OpcodesRaw8080::MOV, ProcessorType::Both, "MOV", { InstructionOption8080::regm8_d3, InstructionOption8080::regm8_s0 } },
    { OpcodesRaw8080::HLT, ProcessorType::Both, "HLT", {} },
    { OpcodesRaw8080::ADD, ProcessorType::Both, "ADD", { InstructionOption8080::regm8_d0 } },
    { OpcodesRaw8080::ADC, ProcessorType::Both, "ADC", { InstructionOption8080::regm8_d0 } },
    { OpcodesRaw8080::SUB, ProcessorType::Both, "SUB", { InstructionOption8080::regm8_d0 } },
    { OpcodesRaw8080::SBB, ProcessorType::Both, "SBB", { InstructionOption8080::regm8_d0 } },
    { OpcodesRaw8080::ANA, ProcessorType::Both, "ANA", { InstructionOption8080::regm8_d0 } },
    { OpcodesRaw8080::XRA, ProcessorType::Both, "XRA", { InstructionOption8080::regm8_d0 } },
    { OpcodesRaw8080::ORA, ProcessorType::Both, "ORA", { InstructionOption8080::regm8_d0 } },
    { OpcodesRaw8080::CMP, ProcessorType::Both, "CMP", { InstructionOption8080::regm8_d0 } },
    { OpcodesRaw8080::RNZ, ProcessorType::Both, "RNZ", { } },
    { OpcodesRaw8080::POP, ProcessorType::Both, "POP", { InstructionOption8080::reg16_psw_4 } },
    { OpcodesRaw8080::JNZ, ProcessorType::Both, "JNZ", { InstructionOption8080::a16 } },
    { OpcodesRaw8080::JMP, ProcessorType::Both, "JMP", { InstructionOption8080::a16 } },
    { OpcodesRaw8080::CNZ, ProcessorType::Both, "CNZ", { InstructionOption8080::a16 } },
    { OpcodesRaw8080::PUSH, ProcessorType::Both, "PUSH", { InstructionOption8080::reg16_psw_4 } },
    { OpcodesRaw8080::ADI, ProcessorType::Both, "ADI", { InstructionOption8080::d8 } },
    { OpcodesRaw8080::RST, ProcessorType::Both, "RST", { InstructionOption8080::n3 } },
    { OpcodesRaw8080::RZ, ProcessorType::Both, "RZ", { } },
    { OpcodesRaw8080::RET, ProcessorType::Both, "RET", { } },
    { OpcodesRaw8080::JZ, ProcessorType::Both, "JZ", { InstructionOption8080::a16 } },
    { OpcodesRaw8080::RSTV, ProcessorType::Only8085, "RSTV", {} },
    { OpcodesRaw8080::CZ, ProcessorType::Both, "CZ", { InstructionOption8080::a16 } },
    { OpcodesRaw8080::CALL, ProcessorType::Both, "CALL", { InstructionOption8080::a16 } },
    { OpcodesRaw8080::ACI, ProcessorType::Both, "ACI", { InstructionOption8080::d8 } },
    { OpcodesRaw8080::RNC, ProcessorType::Both, "RNC", { } },
    { OpcodesRaw8080::JNC, ProcessorType::Both, "JNC", { InstructionOption8080::a16 } },
    { OpcodesRaw8080::OUTP, ProcessorType::Both, "OUT", { InstructionOption8080::p8 } },
    { OpcodesRaw8080::CNC, ProcessorType::Both, "CNC", { InstructionOption8080::a16 } },
    { OpcodesRaw8080::SUI, ProcessorType::Both, "SUI", { InstructionOption8080::d8 } },
    { OpcodesRaw8080::RC, ProcessorType::Both, "RC", { } },
    { OpcodesRaw8080::SHLX, ProcessorType::Only8085, "SHLX", {} },
    { OpcodesRaw8080::JC, ProcessorType::Both, "JC", { InstructionOption8080::a16 } },
    { OpcodesRaw8080::INP, ProcessorType::Both, "IN", { InstructionOption8080::p8 } },
    { OpcodesRaw8080::CC, ProcessorType::Both, "CC", { InstructionOption8080::a16 } },
    { OpcodesRaw8080::JNK, ProcessorType::Only8085, "JNK", { InstructionOption8080::a16 } },
    { OpcodesRaw8080::SBI, ProcessorType::Both, "SBI", { InstructionOption8080::d8 } },
    { OpcodesRaw8080::RPO, ProcessorType::Both, "RPO", { } },
    { OpcodesRaw8080::JPO, ProcessorType::Both, "JPO", { InstructionOption8080::a16 } },
    { OpcodesRaw8080::XTHL, ProcessorType::Both, "XTHL", { } },
    { OpcodesRaw8080::CPO, ProcessorType::Both, "CPO", { InstructionOption8080::a16 } },
    { OpcodesRaw8080::ANI, ProcessorType::Both, "ANI", { InstructionOption8080::d8 } },
    { OpcodesRaw8080::RPE, ProcessorType::Both, "RPE", { } },
    { OpcodesRaw8080::PCHL, ProcessorType::Both, "PCHL", { } },
    { OpcodesRaw8080::JPE, ProcessorType::Both, "JPE", { InstructionOption8080::a16 } },
    { OpcodesRaw8080::XCHG, ProcessorType::Both, "XCHG", {} },
    { OpcodesRaw8080::CPE, ProcessorType::Both, "CPE", { InstructionOption8080::a16 } },
    { OpcodesRaw8080::LHLX, ProcessorType::Only8085, "LHLX", {} },
    { OpcodesRaw8080::XRI, ProcessorType::Both, "XRI", { InstructionOption8080::d8 } },
    { OpcodesRaw8080::RP, ProcessorType::Both, "RP", { } },
    { OpcodesRaw8080::JP, ProcessorType::Both, "JP", { InstructionOption8080::a16 } },
    { OpcodesRaw8080::DI, ProcessorType::Both, "DI", { } },
    { OpcodesRaw8080::CP, ProcessorType::Both, "CP", { InstructionOption8080::a16 } },
    { OpcodesRaw8080::ORI, ProcessorType::Both, "ORI", { InstructionOption8080::d8 } },
    { OpcodesRaw8080::RM, ProcessorType::Both, "RM", { } },
    { OpcodesRaw8080::SPHL, ProcessorType::Both, "SPHL", { } },
    { OpcodesRaw8080::JM, ProcessorType::Both, "JM", { InstructionOption8080::a16 } },
    { OpcodesRaw8080::EI,ProcessorType::Both,  "EI", { } },
    { OpcodesRaw8080::CM, ProcessorType::Both, "CM", { InstructionOption8080::a16 } },
    { OpcodesRaw8080::JK, ProcessorType::Only8085, "JK", { InstructionOption8080::a16 } },
    { OpcodesRaw8080::CPI, ProcessorType::Both, "CPI", { InstructionOption8080::d8 } },
};

static const InstructionData8080 instructiond8080[256] =
{
        { 0x00, 4,  0,  1, 0, 1, "NOP" },
        { 0x01, 10, 0,  3, 0, 3, "LXI B," },
        { 0x02, 7,  0,  2, 0, 1, "STAX B" },
        { 0x03, 5,  0,  1, 0, 1, "INX B" },
        { 0x04, 5,  0,  1, 0, 1, "INR B" },
        { 0x05, 5,  0,  1, 0, 1, "DCR B" },
        { 0x06, 0,  0,  0, 0, 0, "" },
        { 0x07, 4,  0,  1, 0, 1, "RLC" },
        { 0x08, 0,  0,  0, 0, 0, "" },
        { 0x09, 10, 0,  3, 0, 1, "DAD B" },
        { 0x0A, 7,  0,  2, 0, 1, "LDAX B" },
        { 0x0B, 5,  0,  1, 0, 1, "DCX B" },
        { 0x0C, 5,  0,  1, 0, 1, "INR C" },
        { 0x0D, 5,  0,  1, 0, 1, "DCR C" },
        { 0x0E, 0,  0,  0, 0, 0, "" },
        { 0x0F, 4,  0,  1, 0, 1, "RRC" },
        { 0x10, 0,  0,  0, 0, 0, "" },
        { 0x11, 10, 0,  3, 0, 3, "LXI D," },
        { 0x12, 7,  0,  2, 0, 1, "STAX D" },
        { 0x13, 5,  0,  1, 0, 1, "INX D" },
        { 0x14, 5,  0,  1, 0, 1, "INR D" },
        { 0x15, 5,  0,  1, 0, 1, "DCR D" },
        { 0x16, 0,  0,  0, 0, 0, "" },
        { 0x17, 4,  0,  1, 0, 1, "RAL" },
        { 0x18, 0,  0,  0, 0, 0, "" },
        { 0x19, 10, 0,  3, 0, 1, "DAD D" },
        { 0x1A, 7,  0,  2, 0, 1, "LDAX D" },
        { 0x1B, 5,  0,  1, 0, 1, "DCX D" },
        { 0x1C, 5,  0,  1, 0, 1, "INR E" },
        { 0x1D, 5,  0,  1, 0, 1, "DCR E" },
        { 0x1E, 0,  0,  0, 0, 0, "" },
        { 0x1F, 4,  0,  1, 0, 1, "RAR" },
        { 0x20, 0,  0,  0, 0, 0, "" },
        { 0x21, 10, 0,  3, 0, 3, "LXI H," },
        { 0x22, 16, 0,  5, 0, 3, "SHLD " },
        { 0x23, 5,  0,  1, 0, 1, "INX H" },
        { 0x24, 5,  0,  1, 0, 1, "INR H" },
        { 0x25, 5,  0,  1, 0, 1, "DCR H" },
        { 0x26, 0,  0,  0, 0, 0, "" },
        { 0x27, 4,  0,  1, 0, 1, "DAA" },
        { 0x28, 0,  0,  0, 0, 0, "" },
        { 0x29, 10, 0,  3, 0, 1, "DAD H" },
        { 0x2A, 16, 0,  5, 0, 3, "LHLD " },
        { 0x2B, 5,  0,  1, 0, 1, "DCX H" },
        { 0x2C, 5,  0,  1, 0, 1, "INR L" },
        { 0x2D, 5,  0,  1, 0, 1, "DCR L" },
        { 0x2E, 0,  0,  0, 0, 0, "" },
        { 0x2F, 4,  0,  1, 0, 1, "CMA" },
        { 0x30, 0,  0,  0, 0, 0, "" },
        { 0x31, 10, 0,  3, 0, 3, "LXI SP," },
        { 0x32, 13, 0,  4, 0, 3, "STA " },
        { 0x33, 5,  0,  1, 0, 1, "INX SP" },
        { 0x34, 10, 0,  3, 0, 1, "INR M" },
        { 0x35, 10, 0,  3, 0, 1, "DCR M" },
        { 0x36, 10, 0,  3, 0, 2, "MVI M," },
        { 0x37, 4,  0,  1, 0, 1, "STC" },
        { 0x38, 0,  0,  0, 0, 0, "" },
        { 0x39, 10, 0,  3, 0, 1, "DAD SP" },
        { 0x3A, 13, 0,  4, 0, 3, "LDA " },
        { 0x3B, 5,  0,  1, 0, 1, "DCX SP" },
        { 0x3C, 5,  0,  1, 0, 1, "INR A" },
        { 0x3D, 5,  0,  1, 0, 1, "DCR A" },
        { 0x3E, 0,  0,  0, 0, 0, "" },
        { 0x3F, 4,  0,  1, 0, 1, "CMC" },
        { 0x40, 5,  0,  1, 0, 1, "MOV B,B" },
        { 0x41, 5,  0,  1, 0, 1, "MOV B,C" },
        { 0x42, 5,  0,  1, 0, 1, "MOV B,D" },
        { 0x43, 5,  0,  1, 0, 1, "MOV B,E" },
        { 0x44, 5,  0,  1, 0, 1, "MOV B,H" },
        { 0x45, 5,  0,  1, 0, 1, "MOV B,L" },
        { 0x46, 7,  0,  2, 0, 1, "MOV B,M" },
        { 0x47, 5,  0,  1, 0, 1, "MOV B,A" },
        { 0x48, 5,  0,  1, 0, 1, "MOV C,B" },
        { 0x49, 5,  0,  1, 0, 1, "MOV C,C" },
        { 0x4A, 5,  0,  1, 0, 1, "MOV C,D" },
        { 0x4B, 5,  0,  1, 0, 1, "MOV C,E" },
        { 0x4C, 5,  0,  1, 0, 1, "MOV C,H" },
        { 0x4D, 5,  0,  1, 0, 1, "MOV C,L" },
        { 0x4E, 7,  0,  2, 0, 1, "MOV C,M" },
        { 0x4F, 5,  0,  1, 0, 1, "MOV C,A" },
        { 0x50, 5,  0,  1, 0, 1, "MOV D,B" },
        { 0x51, 5,  0,  1, 0, 1, "MOV D,C" },
        { 0x52, 5,  0,  1, 0, 1, "MOV D,D" },
        { 0x53, 5,  0,  1, 0, 1, "MOV D,E" },
        { 0x54, 5,  0,  1, 0, 1, "MOV D,H" },
        { 0x55, 5,  0,  1, 0, 1, "MOV D,L" },
        { 0x56, 7,  0,  2, 0, 1, "MOV D,M" },
        { 0x57, 5,  0,  1, 0, 1, "MOV D,A" },
        { 0x58, 5,  0,  1, 0, 1, "MOV E,B" },
        { 0x59, 5,  0,  1, 0, 1, "MOV E,C" },
        { 0x5A, 5,  0,  1, 0, 1, "MOV E,D" },
        { 0x5B, 5,  0,  1, 0, 1, "MOV E,E" },
        { 0x5C, 5,  0,  1, 0, 1, "MOV E,H" },
        { 0x5D, 5,  0,  1, 0, 1, "MOV E,L" },
        { 0x5E, 7,  0,  2, 0, 1, "MOV E,M" },
        { 0x5F, 5,  0,  1, 0, 1, "MOV E,A" },
        { 0x60, 5,  0,  1, 0, 1, "MOV H,B" },
        { 0x61, 5,  0,  1, 0, 1, "MOV H,C" },
        { 0x62, 5,  0,  1, 0, 1, "MOV H,D" },
        { 0x63, 5,  0,  1, 0, 1, "MOV H,E" },
        { 0x64, 5,  0,  1, 0, 1, "MOV H,H" },
        { 0x65, 5,  0,  1, 0, 1, "MOV H,L" },
        { 0x66, 7,  0,  2, 0, 1, "MOV H,M" },
        { 0x67, 5,  0,  1, 0, 1, "MOV H,A" },
        { 0x68, 5,  0,  1, 0, 1, "MOV L,B" },
        { 0x69, 5,  0,  1, 0, 1, "MOV L,C" },
        { 0x6A, 5,  0,  1, 0, 1, "MOV L,D" },
        { 0x6B, 5,  0,  1, 0, 1, "MOV L,E" },
        { 0x6C, 5,  0,  1, 0, 1, "MOV L,H" },
        { 0x6D, 5,  0,  1, 0, 1, "MOV L,L" },
        { 0x6E, 7,  0,  2, 0, 1, "MOV L,M" },
        { 0x6F, 5,  0,  1, 0, 1, "MOV L,A" },
        { 0x70, 7,  0,  2, 0, 1, "MOV M,B" },
        { 0x71, 7,  0,  2, 0, 1, "MOV M,C" },
        { 0x72, 7,  0,  2, 0, 1, "MOV M,D" },
        { 0x73, 7,  0,  2, 0, 1, "MOV M,E" },
        { 0x74, 7,  0,  2, 0, 1, "MOV M,H" },
        { 0x75, 7,  0,  2, 0, 1, "MOV M,L" },
        { 0x76, 7,  0,  1, 0, 1, "HLT" },
        { 0x77, 7,  0,  2, 0, 1, "MOV M,A" },
        { 0x78, 5,  0,  1, 0, 1, "MOV A,B" },
        { 0x79, 5,  0,  1, 0, 1, "MOV A,C" },
        { 0x7A, 5,  0,  1, 0, 1, "MOV A,D" },
        { 0x7B, 5,  0,  1, 0, 1, "MOV A,E" },
        { 0x7C, 5,  0,  1, 0, 1, "MOV A,H" },
        { 0x7D, 5,  0,  1, 0, 1, "MOV A,L" },
        { 0x7E, 7,  0,  2, 0, 1, "MOV A,M" },
        { 0x7F, 5,  0,  1, 0, 1, "MOV A,A" },
        { 0x80, 4,  0,  1, 0, 1, "ADD B" },
        { 0x81, 4,  0,  1, 0, 1, "ADD C" },
        { 0x82, 4,  0,  1, 0, 1, "ADD D" },
        { 0x83, 4,  0,  1, 0, 1, "ADD E" },
        { 0x84, 4,  0,  1, 0, 1, "ADD H" },
        { 0x85, 4,  0,  1, 0, 1, "ADD L" },
        { 0x86, 7,  0,  2, 0, 1, "ADD M" },
        { 0x87, 4,  0,  1, 0, 1, "ADD A" },
        { 0x88, 4,  0,  1, 0, 1, "ADC B" },
        { 0x89, 4,  0,  1, 0, 1, "ADC C" },
        { 0x8A, 4,  0,  1, 0, 1, "ADC D" },
        { 0x8B, 4,  0,  1, 0, 1, "ADC E" },
        { 0x8C, 4,  0,  1, 0, 1, "ADC H" },
        { 0x8D, 4,  0,  1, 0, 1, "ADC L" },
        { 0x8E, 7,  0,  2, 0, 1, "ADC M" },
        { 0x8F, 4,  0,  1, 0, 1, "ADC A" },
        { 0x90, 4,  0,  1, 0, 1, "SUB B" },
        { 0x91, 4,  0,  1, 0, 1, "SUB C" },
        { 0x92, 4,  0,  1, 0, 1, "SUB D" },
        { 0x93, 4,  0,  1, 0, 1, "SUB E" },
        { 0x94, 4,  0,  1, 0, 1, "SUB H" },
        { 0x95, 4,  0,  1, 0, 1, "SUB L" },
        { 0x96, 7,  0,  2, 0, 1, "SUB M" },
        { 0x97, 4,  0,  1, 0, 1, "SUB A" },
        { 0x98, 4,  0,  1, 0, 1, "SBB B" },
        { 0x99, 4,  0,  1, 0, 1, "SBB C" },
        { 0x9A, 4,  0,  1, 0, 1, "SBB D" },
        { 0x9B, 4,  0,  1, 0, 1, "SBB E" },
        { 0x9C, 4,  0,  1, 0, 1, "SBB H" },
        { 0x9D, 4,  0,  1, 0, 1, "SBB L" },
        { 0x9E, 7,  0,  2, 0, 1, "SBB M" },
        { 0x9F, 4,  0,  1, 0, 1, "SBB A" },
        { 0xA0, 4,  0,  1, 0, 1, "ANA B" },
        { 0xA1, 4,  0,  1, 0, 1, "ANA C" },
        { 0xA2, 4,  0,  1, 0, 1, "ANA D" },
        { 0xA3, 4,  0,  1, 0, 1, "ANA E" },
        { 0xA4, 4,  0,  1, 0, 1, "ANA H" },
        { 0xA5, 4,  0,  1, 0, 1, "ANA L" },
        { 0xA6, 7,  0,  2, 0, 1, "ANA M" },
        { 0xA7, 4,  0,  1, 0, 1, "ANA A" },
        { 0xA8, 4,  0,  1, 0, 1, "XRA B" },
        { 0xA9, 4,  0,  1, 0, 1, "XRA C" },
        { 0xAA, 4,  0,  1, 0, 1, "XRA D" },
        { 0xAB, 4,  0,  1, 0, 1, "XRA E" },
        { 0xAC, 4,  0,  1, 0, 1, "XRA H" },
        { 0xAD, 4,  0,  1, 0, 1, "XRA L" },
        { 0xAE, 7,  0,  2, 0, 1, "XRA M" },
        { 0xAF, 4,  0,  1, 0, 1, "XRA A" },
        { 0xB0, 4,  0,  1, 0, 1, "ORA B" },
        { 0xB1, 4,  0,  1, 0, 1, "ORA C" },
        { 0xB2, 4,  0,  1, 0, 1, "ORA D" },
        { 0xB3, 4,  0,  1, 0, 1, "ORA E" },
        { 0xB4, 4,  0,  1, 0, 1, "ORA H" },
        { 0xB5, 4,  0,  1, 0, 1, "ORA L" },
        { 0xB6, 7,  0,  2, 0, 1, "ORA M" },
        { 0xB7, 4,  0,  1, 0, 1, "ORA A" },
        { 0xB8, 4,  0,  1, 0, 1, "CMP B" },
        { 0xB9, 4,  0,  1, 0, 1, "CMP C" },
        { 0xBA, 4,  0,  1, 0, 1, "CMP D" },
        { 0xBB, 4,  0,  1, 0, 1, "CMP E" },
        { 0xBC, 4,  0,  1, 0, 1, "CMP H" },
        { 0xBD, 4,  0,  1, 0, 1, "CMP L" },
        { 0xBE, 7,  0,  2, 0, 1, "CMP M" },
        { 0xBF, 4,  0,  1, 0, 1, "CMP A" },
        { 0xC0, 11, 5,  3, 1, 1, "RNZ" },
        { 0xC1, 10, 0,  3, 0, 1, "POP B" },
        { 0xC2, 10, 0,  3, 0, 3, "JNZ " },
        { 0xC3, 10, 0,  3, 0, 3, "JMP " },
        { 0xC4, 17, 11, 5, 3, 3, "CNZ " },
        { 0xC5, 11, 0,  3, 0, 1, "PUSH B" },
        { 0xC6, 7,  0,  2, 0, 2, "ADI " },
        { 0xC7, 11, 0,  3, 0, 1, "RST 0" },
        { 0xC8, 11, 5,  3, 1, 1, "RZ" },
        { 0xC9, 10, 0,  3, 0, 1, "RET" },
        { 0xCA, 10, 0,  3, 0, 3, "JZ " },
        { 0xCB, 0,  0,  0, 0, 0, "" },
        { 0xCC, 17, 11, 5, 3, 3, "CZ " },
        { 0xCD, 17, 0,  5, 0, 3, "CALL " },
        { 0xCE, 7,  0,  2, 0, 2, "ACI " },
        { 0xCF, 11, 0,  3, 0, 1, "RST 1" },
        { 0xD0, 11, 5,  3, 1, 1, "RNC" },
        { 0xD1, 10, 0,  3, 0, 1, "POP D" },
        { 0xD2, 10, 0,  3, 0, 3, "JNC " },
        { 0xD3, 10, 0,  3, 0, 2, "OUT " },
        { 0xD4, 17, 11, 5, 3, 3, "CNC " },
        { 0xD5, 11, 0,  3, 0, 1, "PUSH D" },
        { 0xD6, 7,  0,  2, 0, 2, "SUI " },
        { 0xD7, 11, 0,  3, 0, 1, "RST 2" },
        { 0xD8, 11, 5,  3, 1, 1, "RC" },
        { 0xD9, 0,  0,  0, 0, 0, "" },
        { 0xDA, 10, 0,  3, 0, 3, "JC " },
        { 0xDB, 10, 0,  3, 0, 2, "IN " },
        { 0xDC, 17, 11, 5, 3, 3, "CC " },
        { 0xDD, 0,  0,  0, 0, 0, "" },
        { 0xDE, 7,  0,  2, 0, 2, "SBI " },
        { 0xDF, 11, 0,  3, 0, 1, "RST 3" },
        { 0xE0, 11, 5,  3, 1, 1, "RPO" },
        { 0xE1, 10, 0,  3, 0, 1, "POP H" },
        { 0xE2, 10, 0,  3, 0, 3, "JPO " },
        { 0xE3, 18, 0,  5, 0, 1, "XTHL" },
        { 0xE4, 17, 11, 5, 3, 3, "CPO " },
        { 0xE5, 11, 0,  3, 0, 1, "PUSH H" },
        { 0xE6, 7,  0,  2, 0, 2, "ANI " },
        { 0xE7, 11, 0,  3, 0, 1, "RST 4" },
        { 0xE8, 11, 5,  3, 1, 1, "RPE" },
        { 0xE9, 5,  0,  1, 0, 1, "PCHL" },
        { 0xEA, 10, 0,  3, 0, 3, "JPE " },
        { 0xEB, 4,  0,  1, 0, 1, "XCHG" },
        { 0xEC, 17, 11, 5, 3, 3, "CPE " },
        { 0xED, 0,  0,  0, 0, 0, "" },
        { 0xEE, 7,  0,  2, 0, 2, "XRI " },
        { 0xEF, 11, 0,  3, 0, 1, "RST 5" },
        { 0xF0, 11, 5,  3, 1, 1, "RP" },
        { 0xF1, 10, 0,  3, 0, 1, "POP PSW" },
        { 0xF2, 10, 0,  3, 0, 3, "JP " },
        { 0xF3, 4,  0,  1, 0, 1, "DI" },
        { 0xF4, 17, 11, 5, 3, 3, "CP " },
        { 0xF5, 11, 0,  3, 0, 1, "PUSH PSW" },
        { 0xF6, 7,  0,  2, 0, 2, "ORI " },
        { 0xF7, 11, 0,  3, 0, 1, "RST 6" },
        { 0xF8, 11, 5,  3, 1, 1, "RM" },
        { 0xF9, 5,  0,  1, 0, 1, "SPHL" },
        { 0xFA, 10, 0,  3, 0, 3, "JM " },
        { 0xFB, 4,  0,  1, 0, 1, "EI" },
        { 0xFC, 17, 11, 5, 3, 3, "CM " },
        { 0xFD, 0,  0,  0, 0, 0, "" },
        { 0xFE, 7,  0,  2, 0, 2, "CPI " },
        { 0xFF, 11, 0,  3, 0, 1, "RST 7" },
};

static const uint8_t flagsZSTable[256] =
{
    Flags8080::Zero, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign
};

static const uint8_t flagsPZSTable[256] =
{
    Flags8080::Zero | Flags8080::Parity, 0, 0, Flags8080::Parity, 0, Flags8080::Parity, Flags8080::Parity, 0,
    0, Flags8080::Parity, Flags8080::Parity, 0, Flags8080::Parity, 0, 0, Flags8080::Parity,
    0, Flags8080::Parity, Flags8080::Parity, 0, Flags8080::Parity, 0, 0, Flags8080::Parity, Flags8080::Parity, 0, 0, Flags8080::Parity, 0, Flags8080::Parity, Flags8080::Parity, 0,
    0, Flags8080::Parity, Flags8080::Parity, 0, Flags8080::Parity, 0, 0, Flags8080::Parity, Flags8080::Parity, 0, 0, Flags8080::Parity, 0, Flags8080::Parity, Flags8080::Parity, 0,
    Flags8080::Parity, 0, 0, Flags8080::Parity, 0, Flags8080::Parity, Flags8080::Parity, 0, 0, Flags8080::Parity, Flags8080::Parity, 0, Flags8080::Parity, 0, 0, Flags8080::Parity,
    0, Flags8080::Parity, Flags8080::Parity, 0, Flags8080::Parity, 0, 0, Flags8080::Parity, Flags8080::Parity, 0, 0, Flags8080::Parity, 0, Flags8080::Parity, Flags8080::Parity, 0,
    Flags8080::Parity, 0, 0, Flags8080::Parity, 0, Flags8080::Parity, Flags8080::Parity, 0, 0, Flags8080::Parity, Flags8080::Parity, 0, Flags8080::Parity, 0, 0, Flags8080::Parity,
    Flags8080::Parity, 0, 0, Flags8080::Parity, 0, Flags8080::Parity, Flags8080::Parity, 0, 0, Flags8080::Parity, Flags8080::Parity, 0, Flags8080::Parity, 0, 0, Flags8080::Parity,
    0, Flags8080::Parity, Flags8080::Parity, 0, Flags8080::Parity, 0, 0, Flags8080::Parity, Flags8080::Parity, 0, 0, Flags8080::Parity, 0, Flags8080::Parity, Flags8080::Parity, 0,
    Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity,
    Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign,
    Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity,
    Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity,
    Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity,
    Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign,
    Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity,
    Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity,
    Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity,
    Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign,
    Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign,
    Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity, Flags8080::Sign, Flags8080::Sign, Flags8080::Sign | Flags8080::Parity
};

static const uint16_t correctDAATable[1024] =
{
    0x4400, 0x0001, 0x0002, 0x0403, 0x0004, 0x0405, 0x0406, 0x0007,
    0x0008, 0x0409, 0x1010, 0x1411, 0x1412, 0x1013, 0x1414, 0x1015,
    0x0010, 0x0411, 0x0412, 0x0013, 0x0414, 0x0015, 0x0016, 0x0417,
    0x0418, 0x0019, 0x1020, 0x1421, 0x1422, 0x1023, 0x1424, 0x1025,
    0x0020, 0x0421, 0x0422, 0x0023, 0x0424, 0x0025, 0x0026, 0x0427,
    0x0428, 0x0029, 0x1430, 0x1031, 0x1032, 0x1433, 0x1034, 0x1435,
    0x0430, 0x0031, 0x0032, 0x0433, 0x0034, 0x0435, 0x0436, 0x0037,
    0x0038, 0x0439, 0x1040, 0x1441, 0x1442, 0x1043, 0x1444, 0x1045,
    0x0040, 0x0441, 0x0442, 0x0043, 0x0444, 0x0045, 0x0046, 0x0447,
    0x0448, 0x0049, 0x1450, 0x1051, 0x1052, 0x1453, 0x1054, 0x1455,
    0x0450, 0x0051, 0x0052, 0x0453, 0x0054, 0x0455, 0x0456, 0x0057,
    0x0058, 0x0459, 0x1460, 0x1061, 0x1062, 0x1463, 0x1064, 0x1465,
    0x0460, 0x0061, 0x0062, 0x0463, 0x0064, 0x0465, 0x0466, 0x0067,
    0x0068, 0x0469, 0x1070, 0x1471, 0x1472, 0x1073, 0x1474, 0x1075,
    0x0070, 0x0471, 0x0472, 0x0073, 0x0474, 0x0075, 0x0076, 0x0477,
    0x0478, 0x0079, 0x9080, 0x9481, 0x9482, 0x9083, 0x9484, 0x9085,
    0x8080, 0x8481, 0x8482, 0x8083, 0x8484, 0x8085, 0x8086, 0x8487,
    0x8488, 0x8089, 0x9490, 0x9091, 0x9092, 0x9493, 0x9094, 0x9495,
    0x8490, 0x8091, 0x8092, 0x8493, 0x8094, 0x8495, 0x8496, 0x8097,
    0x8098, 0x8499, 0x5500, 0x1101, 0x1102, 0x1503, 0x1104, 0x1505,
    0x4500, 0x0101, 0x0102, 0x0503, 0x0104, 0x0505, 0x0506, 0x0107,
    0x0108, 0x0509, 0x1110, 0x1511, 0x1512, 0x1113, 0x1514, 0x1115,
    0x0110, 0x0511, 0x0512, 0x0113, 0x0514, 0x0115, 0x0116, 0x0517,
    0x0518, 0x0119, 0x1120, 0x1521, 0x1522, 0x1123, 0x1524, 0x1125,
    0x0120, 0x0521, 0x0522, 0x0123, 0x0524, 0x0125, 0x0126, 0x0527,
    0x0528, 0x0129, 0x1530, 0x1131, 0x1132, 0x1533, 0x1134, 0x1535,
    0x0530, 0x0131, 0x0132, 0x0533, 0x0134, 0x0535, 0x0536, 0x0137,
    0x0138, 0x0539, 0x1140, 0x1541, 0x1542, 0x1143, 0x1544, 0x1145,
    0x0140, 0x0541, 0x0542, 0x0143, 0x0544, 0x0145, 0x0146, 0x0547,
    0x0548, 0x0149, 0x1550, 0x1151, 0x1152, 0x1553, 0x1154, 0x1555,
    0x0550, 0x0151, 0x0152, 0x0553, 0x0154, 0x0555, 0x0556, 0x0157,
    0x0158, 0x0559, 0x5400, 0x1001, 0x1002, 0x1403, 0x1004, 0x1405,
    0x0560, 0x0161, 0x0162, 0x0563, 0x0164, 0x0565, 0x0566, 0x0167,
    0x0168, 0x0569, 0x1170, 0x1571, 0x1572, 0x1173, 0x1574, 0x1175,
    0x0170, 0x0571, 0x0572, 0x0173, 0x0574, 0x0175, 0x0176, 0x0577,
    0x0578, 0x0179, 0x9180, 0x9581, 0x9582, 0x9183, 0x9584, 0x9185,
    0x8180, 0x8581, 0x8582, 0x8183, 0x8584, 0x8185, 0x8186, 0x8587,
    0x8588, 0x8189, 0x9590, 0x9191, 0x9192, 0x9593, 0x9194, 0x9595,
    0x8590, 0x8191, 0x8192, 0x8593, 0x8194, 0x8595, 0x8596, 0x8197,
    0x8198, 0x8599, 0x95a0, 0x91a1, 0x91a2, 0x95a3, 0x91a4, 0x95a5,
    0x85a0, 0x81a1, 0x81a2, 0x85a3, 0x81a4, 0x85a5, 0x85a6, 0x81a7,
    0x81a8, 0x85a9, 0x91b0, 0x95b1, 0x95b2, 0x91b3, 0x95b4, 0x91b5,
    0x81b0, 0x85b1, 0x85b2, 0x81b3, 0x85b4, 0x81b5, 0x81b6, 0x85b7,
    0x85b8, 0x81b9, 0x95c0, 0x91c1, 0x91c2, 0x95c3, 0x91c4, 0x95c5,
    0x85c0, 0x81c1, 0x81c2, 0x85c3, 0x81c4, 0x85c5, 0x85c6, 0x81c7,
    0x81c8, 0x85c9, 0x91d0, 0x95d1, 0x95d2, 0x91d3, 0x95d4, 0x91d5,
    0x81d0, 0x85d1, 0x85d2, 0x81d3, 0x85d4, 0x81d5, 0x81d6, 0x85d7,
    0x85d8, 0x81d9, 0x91e0, 0x95e1, 0x95e2, 0x91e3, 0x95e4, 0x91e5,
    0x81e0, 0x85e1, 0x85e2, 0x81e3, 0x85e4, 0x81e5, 0x81e6, 0x85e7,
    0x85e8, 0x81e9, 0x95f0, 0x91f1, 0x91f2, 0x95f3, 0x91f4, 0x95f5,
    0x85f0, 0x81f1, 0x81f2, 0x85f3, 0x81f4, 0x85f5, 0x85f6, 0x81f7,
    0x81f8, 0x85f9, 0x5500, 0x1101, 0x1102, 0x1503, 0x1104, 0x1505,
    0x4500, 0x0101, 0x0102, 0x0503, 0x0104, 0x0505, 0x0506, 0x0107,
    0x0108, 0x0509, 0x1110, 0x1511, 0x1512, 0x1113, 0x1514, 0x1115,
    0x0110, 0x0511, 0x0512, 0x0113, 0x0514, 0x0115, 0x0116, 0x0517,
    0x0518, 0x0119, 0x1120, 0x1521, 0x1522, 0x1123, 0x1524, 0x1125,
    0x0120, 0x0521, 0x0522, 0x0123, 0x0524, 0x0125, 0x0126, 0x0527,
    0x0528, 0x0129, 0x1530, 0x1131, 0x1132, 0x1533, 0x1134, 0x1535,
    0x0530, 0x0131, 0x0132, 0x0533, 0x0134, 0x0535, 0x0536, 0x0137,
    0x0138, 0x0539, 0x1140, 0x1541, 0x1542, 0x1143, 0x1544, 0x1145,
    0x0140, 0x0541, 0x0542, 0x0143, 0x0544, 0x0145, 0x0146, 0x0547,
    0x0548, 0x0149, 0x1550, 0x1151, 0x1152, 0x1553, 0x1154, 0x1555,
    0x0550, 0x0151, 0x0152, 0x0553, 0x0154, 0x0555, 0x0556, 0x0157,
    0x0158, 0x0559, 0x1560, 0x1161, 0x1162, 0x1563, 0x1164, 0x1565,
    0x1406, 0x1007, 0x1008, 0x1409, 0x140a, 0x100b, 0x140c, 0x100d,
    0x100e, 0x140f, 0x1010, 0x1411, 0x1412, 0x1013, 0x1414, 0x1015,
    0x1016, 0x1417, 0x1418, 0x1019, 0x101a, 0x141b, 0x101c, 0x141d,
    0x141e, 0x101f, 0x1020, 0x1421, 0x1422, 0x1023, 0x1424, 0x1025,
    0x1026, 0x1427, 0x1428, 0x1029, 0x102a, 0x142b, 0x102c, 0x142d,
    0x142e, 0x102f, 0x1430, 0x1031, 0x1032, 0x1433, 0x1034, 0x1435,
    0x1436, 0x1037, 0x1038, 0x1439, 0x143a, 0x103b, 0x143c, 0x103d,
    0x103e, 0x143f, 0x1040, 0x1441, 0x1442, 0x1043, 0x1444, 0x1045,
    0x1046, 0x1447, 0x1448, 0x1049, 0x104a, 0x144b, 0x104c, 0x144d,
    0x144e, 0x104f, 0x1450, 0x1051, 0x1052, 0x1453, 0x1054, 0x1455,
    0x1456, 0x1057, 0x1058, 0x1459, 0x145a, 0x105b, 0x145c, 0x105d,
    0x105e, 0x145f, 0x1460, 0x1061, 0x1062, 0x1463, 0x1064, 0x1465,
    0x1466, 0x1067, 0x1068, 0x1469, 0x146a, 0x106b, 0x146c, 0x106d,
    0x106e, 0x146f, 0x1070, 0x1471, 0x1472, 0x1073, 0x1474, 0x1075,
    0x1076, 0x1477, 0x1478, 0x1079, 0x107a, 0x147b, 0x107c, 0x147d,
    0x147e, 0x107f, 0x9080, 0x9481, 0x9482, 0x9083, 0x9484, 0x9085,
    0x9086, 0x9487, 0x9488, 0x9089, 0x908a, 0x948b, 0x908c, 0x948d,
    0x948e, 0x908f, 0x9490, 0x9091, 0x9092, 0x9493, 0x9094, 0x9495,
    0x9496, 0x9097, 0x9098, 0x9499, 0x949a, 0x909b, 0x949c, 0x909d,
    0x909e, 0x949f, 0x5500, 0x1101, 0x1102, 0x1503, 0x1104, 0x1505,
    0x1506, 0x1107, 0x1108, 0x1509, 0x150a, 0x110b, 0x150c, 0x110d,
    0x110e, 0x150f, 0x1110, 0x1511, 0x1512, 0x1113, 0x1514, 0x1115,
    0x1116, 0x1517, 0x1518, 0x1119, 0x111a, 0x151b, 0x111c, 0x151d,
    0x151e, 0x111f, 0x1120, 0x1521, 0x1522, 0x1123, 0x1524, 0x1125,
    0x1126, 0x1527, 0x1528, 0x1129, 0x112a, 0x152b, 0x112c, 0x152d,
    0x152e, 0x112f, 0x1530, 0x1131, 0x1132, 0x1533, 0x1134, 0x1535,
    0x1536, 0x1137, 0x1138, 0x1539, 0x153a, 0x113b, 0x153c, 0x113d,
    0x113e, 0x153f, 0x1140, 0x1541, 0x1542, 0x1143, 0x1544, 0x1145,
    0x1146, 0x1547, 0x1548, 0x1149, 0x114a, 0x154b, 0x114c, 0x154d,
    0x154e, 0x114f, 0x1550, 0x1151, 0x1152, 0x1553, 0x1154, 0x1555,
    0x1556, 0x1157, 0x1158, 0x1559, 0x155a, 0x115b, 0x155c, 0x115d,
    0x115e, 0x155f, 0x5400, 0x1001, 0x1002, 0x1403, 0x1004, 0x1405,
    0x1566, 0x1167, 0x1168, 0x1569, 0x156a, 0x116b, 0x156c, 0x116d,
    0x116e, 0x156f, 0x1170, 0x1571, 0x1572, 0x1173, 0x1574, 0x1175,
    0x1176, 0x1577, 0x1578, 0x1179, 0x117a, 0x157b, 0x117c, 0x157d,
    0x157e, 0x117f, 0x9180, 0x9581, 0x9582, 0x9183, 0x9584, 0x9185,
    0x9186, 0x9587, 0x9588, 0x9189, 0x918a, 0x958b, 0x918c, 0x958d,
    0x958e, 0x918f, 0x9590, 0x9191, 0x9192, 0x9593, 0x9194, 0x9595,
    0x9596, 0x9197, 0x9198, 0x9599, 0x959a, 0x919b, 0x959c, 0x919d,
    0x919e, 0x959f, 0x95a0, 0x91a1, 0x91a2, 0x95a3, 0x91a4, 0x95a5,
    0x95a6, 0x91a7, 0x91a8, 0x95a9, 0x95aa, 0x91ab, 0x95ac, 0x91ad,
    0x91ae, 0x95af, 0x91b0, 0x95b1, 0x95b2, 0x91b3, 0x95b4, 0x91b5,
    0x91b6, 0x95b7, 0x95b8, 0x91b9, 0x91ba, 0x95bb, 0x91bc, 0x95bd,
    0x95be, 0x91bf, 0x95c0, 0x91c1, 0x91c2, 0x95c3, 0x91c4, 0x95c5,
    0x95c6, 0x91c7, 0x91c8, 0x95c9, 0x95ca, 0x91cb, 0x95cc, 0x91cd,
    0x91ce, 0x95cf, 0x91d0, 0x95d1, 0x95d2, 0x91d3, 0x95d4, 0x91d5,
    0x91d6, 0x95d7, 0x95d8, 0x91d9, 0x91da, 0x95db, 0x91dc, 0x95dd,
    0x95de, 0x91df, 0x91e0, 0x95e1, 0x95e2, 0x91e3, 0x95e4, 0x91e5,
    0x91e6, 0x95e7, 0x95e8, 0x91e9, 0x91ea, 0x95eb, 0x91ec, 0x95ed,
    0x95ee, 0x91ef, 0x95f0, 0x91f1, 0x91f2, 0x95f3, 0x91f4, 0x95f5,
    0x95f6, 0x91f7, 0x91f8, 0x95f9, 0x95fa, 0x91fb, 0x95fc, 0x91fd,
    0x91fe, 0x95ff, 0x5500, 0x1101, 0x1102, 0x1503, 0x1104, 0x1505,
    0x1506, 0x1107, 0x1108, 0x1509, 0x150a, 0x110b, 0x150c, 0x110d,
    0x110e, 0x150f, 0x1110, 0x1511, 0x1512, 0x1113, 0x1514, 0x1115,
    0x1116, 0x1517, 0x1518, 0x1119, 0x111a, 0x151b, 0x111c, 0x151d,
    0x151e, 0x111f, 0x1120, 0x1521, 0x1522, 0x1123, 0x1524, 0x1125,
    0x1126, 0x1527, 0x1528, 0x1129, 0x112a, 0x152b, 0x112c, 0x152d,
    0x152e, 0x112f, 0x1530, 0x1131, 0x1132, 0x1533, 0x1134, 0x1535,
    0x1536, 0x1137, 0x1138, 0x1539, 0x153a, 0x113b, 0x153c, 0x113d,
    0x113e, 0x153f, 0x1140, 0x1541, 0x1542, 0x1143, 0x1544, 0x1145,
    0x1146, 0x1547, 0x1548, 0x1149, 0x114a, 0x154b, 0x114c, 0x154d,
    0x154e, 0x114f, 0x1550, 0x1151, 0x1152, 0x1553, 0x1154, 0x1555,
    0x1556, 0x1157, 0x1158, 0x1559, 0x155a, 0x115b, 0x155c, 0x115d,
    0x115e, 0x155f, 0x1560, 0x1161, 0x1162, 0x1563, 0x1164, 0x1565,
};

std::ostream & Simulate::operator << (std::ostream & stream, Opcodes8080 opcode)
{
    stream << instructiond8080[uint8_t(opcode)].instructionMnemonic;
    return stream;
}

Processor8080::Processor8080(double clockFreq)
    : Processor(clockFreq, 16, 8)
{

}

Processor8080::~Processor8080()
{

}

void Processor8080::Setup(MemoryManagerPtr memoryManager, IOManagerPtr ioManager)
{
    memory = memoryManager;
    io = ioManager;
}

void Processor8080::Reset()
{
    registers.pc = InitialPC;
    inte.reset();
    hlda.reset();
}

void Processor8080::FetchInstruction()
{
    //#ifdef DEBUG
    // Turn tracing on when reached trap address
    if (registers.pc == registers.trap)
        registers.trace = 1;
    /* Call single-step debugger, exit if requested */
    if (registers.trace)
        if (debugCallback && !debugCallback(0, registers))
            return;
    //#endif

    uint8_t data = FetchInstructionByte();
    instruction = Opcodes8080(data);
    registers.instructionCycles = instructiond8080[data].cycleCount;
    registers.cycleCount -= registers.instructionCycles;
}

uint8_t Add(uint8_t a, uint8_t r, Flags8080 & flags)
{
    Reg16LH result;
    result.W = a + r;
    flags = Flags8080(flagsZSTable[result.B.l] | 
                      (result.B.h & Flags8080::Carry) |
                      ((a ^ r ^ result.B.l) & Flags8080::AuxCarry));
    return result.B.l;
}

uint8_t AddC(uint8_t a, uint8_t r, Flags8080 & flags)
{
    Reg16LH result;
    result.W = a + r + (flags & Flags8080::Carry);
    flags = Flags8080(flagsZSTable[result.B.l] |
                      (result.B.h & Flags8080::Carry) |
                      ((a ^ r ^ result.B.l) & Flags8080::AuxCarry));
    return result.B.l;
}

uint16_t AddW(uint16_t hl, uint16_t rp, Flags8080 & flags)
{
    Reg16 result = hl + rp;
    flags = Flags8080(flags & ~(Flags8080::Carry) |
                      (((long)hl + (long)rp) & 0x10000 ? Flags8080::Carry : 0));
    return result;
}

uint8_t Sub(uint8_t a, uint8_t r, Flags8080 & flags)
{
    Reg16LH result;
    result.W = a - r;
    flags = Flags8080(flagsZSTable[result.B.l] |
        (-result.B.h & Flags8080::Carry) |
        ((a ^ r ^ result.B.l) & Flags8080::AuxCarry));
    return result.B.l;
}

uint8_t SubC(uint8_t a, uint8_t r, Flags8080 & flags)
{
    Reg16LH result;
    result.W = a - r - (flags & Flags8080::Carry);
    flags = Flags8080(flagsZSTable[result.B.l] |
        (-result.B.h & Flags8080::Carry) |
        ((a ^ r ^ result.B.l) & Flags8080::AuxCarry));
    return result.B.l;
}

uint8_t Inc(uint8_t r, Flags8080 & flags)
{
    Reg8 result = r + 1;
    flags = Flags8080((flags & Flags8080::Carry) | flagsZSTable[result] |
        (result & 0x0F ? 0 : Flags8080::AuxCarry));
    return result;
}

uint8_t Dec(uint8_t r, Flags8080 & flags)
{
    Reg8 result = r - 1;
    flags = Flags8080((flags & Flags8080::Carry) | flagsZSTable[result] |
        (r & 0x0F ? 0 : Flags8080::AuxCarry));
    return result;
}

uint8_t DAA(uint8_t a, Flags8080 & flags)
{
    Reg16LH result{};
    result.B.l = a;
    if (flags & Flags8080::Carry)
        result.B.h |= 0x01;
    if (flags & Flags8080::AuxCarry)
        result.B.h |= 0x02;
    result.W = correctDAATable[result.W];
    flags = Flags8080(result.B.h);
    return result.B.l;
}

uint8_t And(uint8_t a, uint8_t r, Flags8080 & flags)
{
    Reg8 result = a & r;
    flags = Flags8080(Flags8080::AuxCarry | flagsPZSTable[result]);
    return result;
}

uint8_t Or(uint8_t a, uint8_t r, Flags8080 & flags)
{
    Reg8 result = a | r;
    flags = Flags8080(Flags8080::AuxCarry | flagsPZSTable[result]);
    return result;
}

uint8_t Xor(uint8_t a, uint8_t r, Flags8080 & flags)
{
    Reg8 result = a ^ r;
    flags = Flags8080(Flags8080::AuxCarry | flagsPZSTable[result]);
    return result;
}

void Processor8080::ExecuteInstruction()
{
    switch (instruction)
    {
    case Opcodes8080::NOP:                                                                                          break;
    case Opcodes8080::LXI_B:    registers.bc.B.l = FetchInstructionByte();              
                                registers.bc.B.h = FetchInstructionByte();                                          break;
    case Opcodes8080::STAX_B:   memory->Store8(registers.bc.W, registers.a);                                        break;
    case Opcodes8080::INX_B:    registers.bc.W++;                                                                   break;
    case Opcodes8080::INR_B:    registers.bc.B.h = Inc(registers.bc.B.h, registers.flags);                          break;
    case Opcodes8080::DCR_B:    registers.bc.B.h = Dec(registers.bc.B.h, registers.flags);                          break;
    case Opcodes8080::DAD_B:    registers.hl.W = AddW(registers.hl.W, registers.bc.W, registers.flags);             break;
    case Opcodes8080::LDAX_B:   registers.a = memory->Fetch8(registers.bc.W);                                       break;
    case Opcodes8080::DCX_B:    registers.bc.W--;                                                                   break;
    case Opcodes8080::INR_C:    registers.bc.B.l = Inc(registers.bc.B.l, registers.flags);                          break;
    case Opcodes8080::DCR_C:    registers.bc.B.l = Dec(registers.bc.B.l, registers.flags);                          break;
    case Opcodes8080::LXI_D:    registers.de.B.l = FetchInstructionByte();
                                registers.de.B.h = FetchInstructionByte();                                          break;
    case Opcodes8080::STAX_D:   memory->Store8(registers.de.W, registers.a);                                        break;
    case Opcodes8080::INX_D:    registers.de.W++;                                                                   break;
    case Opcodes8080::INR_D:    registers.de.B.h = Inc(registers.de.B.h, registers.flags);                          break;
    case Opcodes8080::DCR_D:    registers.de.B.h = Dec(registers.de.B.h, registers.flags);                          break;
    case Opcodes8080::DAD_D:    registers.hl.W = AddW(registers.hl.W, registers.de.W, registers.flags);             break;
    case Opcodes8080::LDAX_D:   registers.a = memory->Fetch8(registers.de.W);                                       break;
    case Opcodes8080::DCX_D:    registers.de.W--;                                                                   break;
    case Opcodes8080::INR_E:    registers.de.B.l = Inc(registers.de.B.l, registers.flags);                          break;
    case Opcodes8080::DCR_E:    registers.de.B.l = Dec(registers.de.B.l, registers.flags);                          break;
    case Opcodes8080::LXI_H:    registers.hl.B.l = FetchInstructionByte();
                                registers.hl.B.h = FetchInstructionByte();                                          break;
    case Opcodes8080::SHLD:     registers.wz.B.l = FetchInstructionByte();
                                registers.wz.B.h = FetchInstructionByte();
                                memory->Store16(registers.wz.W, registers.hl.W);                                    break;
    case Opcodes8080::INX_H:    registers.hl.W++;                                                                   break;
    case Opcodes8080::INR_H:    registers.hl.B.h = Inc(registers.hl.B.h, registers.flags);                          break;
    case Opcodes8080::DCR_H:    registers.hl.B.h = Dec(registers.hl.B.h, registers.flags);                          break;
    case Opcodes8080::DAA:      registers.a = DAA(registers.a, registers.flags);                                    break;
    case Opcodes8080::DAD_H:    registers.hl.W = AddW(registers.hl.W, registers.hl.W, registers.flags);             break;
    case Opcodes8080::LHLD:     registers.wz.B.l = FetchInstructionByte();
                                registers.wz.B.h = FetchInstructionByte();
                                registers.hl.W = memory->Fetch16(registers.wz.W);                                   break;
    case Opcodes8080::DCX_H:    registers.hl.W--;                                                                   break;
    case Opcodes8080::INR_L:    registers.hl.B.l = Inc(registers.hl.B.l, registers.flags);                          break;
    case Opcodes8080::DCR_L:    registers.hl.B.l = Dec(registers.hl.B.l, registers.flags);                          break;
    case Opcodes8080::LXI_SP:   registers.sp.B.l = FetchInstructionByte();
                                registers.sp.B.h = FetchInstructionByte();                                          break;
    case Opcodes8080::STA:      registers.wz.B.l = FetchInstructionByte();
                                registers.wz.B.h = FetchInstructionByte();
                                memory->Store8(registers.wz.W, registers.a);                                        break;
    case Opcodes8080::INX_SP:   registers.sp.W++;                                                                   break;
    case Opcodes8080::INR_M:    memory->Store8(registers.hl.W, Inc(memory->Fetch8(registers.hl.W), registers.flags));break;
    case Opcodes8080::DCR_M:    memory->Store8(registers.hl.W, Dec(memory->Fetch8(registers.hl.W), registers.flags));break;
    case Opcodes8080::DAD_SP:   registers.hl.W = AddW(registers.hl.W, registers.sp.W, registers.flags);             break;
    case Opcodes8080::LDA:      registers.wz.B.l = FetchInstructionByte();
                                registers.wz.B.h = FetchInstructionByte();
                                registers.a = memory->Fetch8(registers.wz.W);                                       break;
    case Opcodes8080::DCX_SP:   registers.sp.W--;                                                                   break;
    case Opcodes8080::INR_A:    registers.a = Inc(registers.a, registers.flags);                                    break;
    case Opcodes8080::DCR_A:    registers.a = Dec(registers.a, registers.flags);                                    break;
    case Opcodes8080::MVI_M:    memory->Store8(registers.hl.W, FetchInstructionByte());                             break;
    case Opcodes8080::MOV_B_B:  registers.bc.B.h = registers.bc.B.h;                                                break;
    case Opcodes8080::MOV_B_C:  registers.bc.B.h = registers.bc.B.l;                                                break;
    case Opcodes8080::MOV_B_D:  registers.bc.B.h = registers.de.B.h;                                                break;
    case Opcodes8080::MOV_B_E:  registers.bc.B.h = registers.de.B.l;                                                break;
    case Opcodes8080::MOV_B_H:  registers.bc.B.h = registers.hl.B.h;                                                break;
    case Opcodes8080::MOV_B_L:  registers.bc.B.h = registers.hl.B.l;                                                break;
    case Opcodes8080::MOV_B_M:  registers.bc.B.h = memory->Fetch8(registers.hl.W);                                  break;
    case Opcodes8080::MOV_B_A:  registers.bc.B.h = registers.a;                                                     break;
    case Opcodes8080::MOV_C_B:  registers.bc.B.l = registers.bc.B.h;                                                break;
    case Opcodes8080::MOV_C_C:  registers.bc.B.l = registers.bc.B.l;                                                break;
    case Opcodes8080::MOV_C_D:  registers.bc.B.l = registers.de.B.h;                                                break;
    case Opcodes8080::MOV_C_E:  registers.bc.B.l = registers.de.B.l;                                                break;
    case Opcodes8080::MOV_C_H:  registers.bc.B.l = registers.hl.B.h;                                                break;
    case Opcodes8080::MOV_C_L:  registers.bc.B.l = registers.hl.B.l;                                                break;
    case Opcodes8080::MOV_C_M:  registers.bc.B.l = memory->Fetch8(registers.hl.W);                                  break;
    case Opcodes8080::MOV_C_A:  registers.bc.B.l = registers.a;                                                     break;
    case Opcodes8080::MOV_D_B:  registers.de.B.h = registers.bc.B.h;                                                break;
    case Opcodes8080::MOV_D_C:  registers.de.B.h = registers.bc.B.l;                                                break;
    case Opcodes8080::MOV_D_D:  registers.de.B.h = registers.de.B.h;                                                break;
    case Opcodes8080::MOV_D_E:  registers.de.B.h = registers.de.B.l;                                                break;
    case Opcodes8080::MOV_D_H:  registers.de.B.h = registers.hl.B.h;                                                break;
    case Opcodes8080::MOV_D_L:  registers.de.B.h = registers.hl.B.l;                                                break;
    case Opcodes8080::MOV_D_M:  registers.de.B.h = memory->Fetch8(registers.hl.W);                                  break;
    case Opcodes8080::MOV_D_A:  registers.de.B.h = registers.a;                                                     break;
    case Opcodes8080::MOV_E_B:  registers.de.B.l = registers.bc.B.h;                                                break;
    case Opcodes8080::MOV_E_C:  registers.de.B.l = registers.bc.B.l;                                                break;
    case Opcodes8080::MOV_E_D:  registers.de.B.l = registers.de.B.h;                                                break;
    case Opcodes8080::MOV_E_E:  registers.de.B.l = registers.de.B.l;                                                break;
    case Opcodes8080::MOV_E_H:  registers.de.B.l = registers.hl.B.h;                                                break;
    case Opcodes8080::MOV_E_L:  registers.de.B.l = registers.hl.B.l;                                                break;
    case Opcodes8080::MOV_E_M:  registers.de.B.l = memory->Fetch8(registers.hl.W);                                  break;
    case Opcodes8080::MOV_E_A:  registers.de.B.l = registers.a;                                                     break;
    case Opcodes8080::MOV_H_B:  registers.hl.B.h = registers.bc.B.h;                                                break;
    case Opcodes8080::MOV_H_C:  registers.hl.B.h = registers.bc.B.l;                                                break;
    case Opcodes8080::MOV_H_D:  registers.hl.B.h = registers.de.B.h;                                                break;
    case Opcodes8080::MOV_H_E:  registers.hl.B.h = registers.de.B.l;                                                break;
    case Opcodes8080::MOV_H_H:  registers.hl.B.h = registers.hl.B.h;                                                break;
    case Opcodes8080::MOV_H_L:  registers.hl.B.h = registers.hl.B.l;                                                break;
    case Opcodes8080::MOV_H_M:  registers.hl.B.h = memory->Fetch8(registers.hl.W);                                  break;
    case Opcodes8080::MOV_H_A:  registers.hl.B.h = registers.a;                                                     break;
    case Opcodes8080::MOV_L_B:  registers.hl.B.l = registers.bc.B.h;                                                break;
    case Opcodes8080::MOV_L_C:  registers.hl.B.l = registers.bc.B.l;                                                break;
    case Opcodes8080::MOV_L_D:  registers.hl.B.l = registers.de.B.h;                                                break;
    case Opcodes8080::MOV_L_E:  registers.hl.B.l = registers.de.B.l;                                                break;
    case Opcodes8080::MOV_L_H:  registers.hl.B.l = registers.hl.B.h;                                                break;
    case Opcodes8080::MOV_L_L:  registers.hl.B.l = registers.hl.B.l;                                                break;
    case Opcodes8080::MOV_L_M:  registers.hl.B.l = memory->Fetch8(registers.hl.W);                                  break;
    case Opcodes8080::MOV_L_A:  registers.hl.B.l = registers.a;                                                     break;
    case Opcodes8080::MOV_M_B:  memory->Store8(registers.hl.W, registers.bc.B.h);                                   break;
    case Opcodes8080::MOV_M_C:  memory->Store8(registers.hl.W, registers.bc.B.l);                                   break;
    case Opcodes8080::MOV_M_D:  memory->Store8(registers.hl.W, registers.de.B.h);                                   break;
    case Opcodes8080::MOV_M_E:  memory->Store8(registers.hl.W, registers.de.B.l);                                   break;
    case Opcodes8080::MOV_M_H:  memory->Store8(registers.hl.W, registers.hl.B.h);                                   break;
    case Opcodes8080::MOV_M_L:  memory->Store8(registers.hl.W, registers.hl.B.l);                                   break;
    case Opcodes8080::HLT:  halt = true;                                                                            break;
    case Opcodes8080::MOV_M_A:  memory->Store8(registers.hl.W, registers.a);                                        break;
    case Opcodes8080::MOV_A_B:  registers.a = registers.bc.B.h;                                                     break;
    case Opcodes8080::MOV_A_C:  registers.a = registers.bc.B.l;                                                     break;
    case Opcodes8080::MOV_A_D:  registers.a = registers.de.B.h;                                                     break;
    case Opcodes8080::MOV_A_E:  registers.a = registers.de.B.l;                                                     break;
    case Opcodes8080::MOV_A_H:  registers.a = registers.hl.B.h;                                                     break;
    case Opcodes8080::MOV_A_L:  registers.a = registers.hl.B.l;                                                     break;
    case Opcodes8080::MOV_A_M:  registers.a = memory->Fetch8(registers.hl.W);                                       break;
    case Opcodes8080::MOV_A_A:  registers.a = registers.a;                                                          break;
    case Opcodes8080::ADD_B:    registers.a = Add(registers.a, registers.bc.B.h, registers.flags);                  break;
    case Opcodes8080::ADD_C:    registers.a = Add(registers.a, registers.bc.B.l, registers.flags);                  break;
    case Opcodes8080::ADD_D:    registers.a = Add(registers.a, registers.de.B.h, registers.flags);                  break;
    case Opcodes8080::ADD_E:    registers.a = Add(registers.a, registers.de.B.l, registers.flags);                  break;
    case Opcodes8080::ADD_H:    registers.a = Add(registers.a, registers.hl.B.h, registers.flags);                  break;
    case Opcodes8080::ADD_L:    registers.a = Add(registers.a, registers.hl.B.l, registers.flags);                  break;
    case Opcodes8080::ADD_M:    registers.a = Add(registers.a, memory->Fetch8(registers.hl.W), registers.flags);    break;
    case Opcodes8080::ADD_A:    registers.a = Add(registers.a, registers.a, registers.flags);                       break;
    case Opcodes8080::ADC_B:    registers.a = AddC(registers.a, registers.bc.B.h, registers.flags);                 break;
    case Opcodes8080::ADC_C:    registers.a = AddC(registers.a, registers.bc.B.l, registers.flags);                 break;
    case Opcodes8080::ADC_D:    registers.a = AddC(registers.a, registers.de.B.h, registers.flags);                 break;
    case Opcodes8080::ADC_E:    registers.a = AddC(registers.a, registers.de.B.l, registers.flags);                 break;
    case Opcodes8080::ADC_H:    registers.a = AddC(registers.a, registers.hl.B.h, registers.flags);                 break;
    case Opcodes8080::ADC_L:    registers.a = AddC(registers.a, registers.hl.B.l, registers.flags);                 break;
    case Opcodes8080::ADC_M:    registers.a = AddC(registers.a, memory->Fetch8(registers.hl.W), registers.flags);   break;
    case Opcodes8080::ADC_A:    registers.a = AddC(registers.a, registers.a, registers.flags);                      break;
    case Opcodes8080::SUB_B:    registers.a = Sub(registers.a, registers.bc.B.h, registers.flags);                  break;
    case Opcodes8080::SUB_C:    registers.a = Sub(registers.a, registers.bc.B.l, registers.flags);                  break;
    case Opcodes8080::SUB_D:    registers.a = Sub(registers.a, registers.de.B.h, registers.flags);                  break;
    case Opcodes8080::SUB_E:    registers.a = Sub(registers.a, registers.de.B.l, registers.flags);                  break;
    case Opcodes8080::SUB_H:    registers.a = Sub(registers.a, registers.hl.B.h, registers.flags);                  break;
    case Opcodes8080::SUB_L:    registers.a = Sub(registers.a, registers.hl.B.l, registers.flags);                  break;
    case Opcodes8080::SUB_M:    registers.a = Sub(registers.a, memory->Fetch8(registers.hl.W), registers.flags);    break;
    case Opcodes8080::SUB_A:    registers.a = Sub(registers.a, registers.a, registers.flags);                       break;
    case Opcodes8080::SBB_B:    registers.a = SubC(registers.a, registers.bc.B.h, registers.flags);                 break;
    case Opcodes8080::SBB_C:    registers.a = SubC(registers.a, registers.bc.B.l, registers.flags);                 break;
    case Opcodes8080::SBB_D:    registers.a = SubC(registers.a, registers.de.B.h, registers.flags);                 break;
    case Opcodes8080::SBB_E:    registers.a = SubC(registers.a, registers.de.B.l, registers.flags);                 break;
    case Opcodes8080::SBB_H:    registers.a = SubC(registers.a, registers.hl.B.h, registers.flags);                 break;
    case Opcodes8080::SBB_L:    registers.a = SubC(registers.a, registers.hl.B.l, registers.flags);                 break;
    case Opcodes8080::SBB_M:    registers.a = SubC(registers.a, memory->Fetch8(registers.hl.W), registers.flags);   break;
    case Opcodes8080::SBB_A:    registers.a = SubC(registers.a, registers.a, registers.flags);                      break;
    case Opcodes8080::ANA_B:    registers.a = And(registers.a, registers.bc.B.h, registers.flags);                  break;
    case Opcodes8080::ANA_C:    registers.a = And(registers.a, registers.bc.B.l, registers.flags);                  break;
    case Opcodes8080::ANA_D:    registers.a = And(registers.a, registers.de.B.h, registers.flags);                  break;
    case Opcodes8080::ANA_E:    registers.a = And(registers.a, registers.de.B.l, registers.flags);                  break;
    case Opcodes8080::ANA_H:    registers.a = And(registers.a, registers.hl.B.h, registers.flags);                  break;
    case Opcodes8080::ANA_L:    registers.a = And(registers.a, registers.hl.B.l, registers.flags);                  break;
    case Opcodes8080::ANA_M:    registers.a = And(registers.a, memory->Fetch8(registers.hl.W), registers.flags);    break;
    case Opcodes8080::ANA_A:    registers.a = And(registers.a, registers.a, registers.flags);                       break;
    case Opcodes8080::XRA_B:    registers.a = Xor(registers.a, registers.bc.B.h, registers.flags);                  break;
    case Opcodes8080::XRA_C:    registers.a = Xor(registers.a, registers.bc.B.l, registers.flags);                  break;
    case Opcodes8080::XRA_D:    registers.a = Xor(registers.a, registers.de.B.h, registers.flags);                  break;
    case Opcodes8080::XRA_E:    registers.a = Xor(registers.a, registers.de.B.l, registers.flags);                  break;
    case Opcodes8080::XRA_H:    registers.a = Xor(registers.a, registers.hl.B.h, registers.flags);                  break;
    case Opcodes8080::XRA_L:    registers.a = Xor(registers.a, registers.hl.B.l, registers.flags);                  break;
    case Opcodes8080::XRA_M:    registers.a = Xor(registers.a, memory->Fetch8(registers.hl.W), registers.flags);    break;
    case Opcodes8080::XRA_A:    registers.a = Xor(registers.a, registers.a, registers.flags);                       break;
    case Opcodes8080::ORA_B:    registers.a = Or(registers.a, registers.bc.B.h, registers.flags);                   break;
    case Opcodes8080::ORA_C:    registers.a = Or(registers.a, registers.bc.B.l, registers.flags);                   break;
    case Opcodes8080::ORA_D:    registers.a = Or(registers.a, registers.de.B.h, registers.flags);                   break;
    case Opcodes8080::ORA_E:    registers.a = Or(registers.a, registers.de.B.l, registers.flags);                   break;
    case Opcodes8080::ORA_H:    registers.a = Or(registers.a, registers.hl.B.h, registers.flags);                   break;
    case Opcodes8080::ORA_L:    registers.a = Or(registers.a, registers.hl.B.l, registers.flags);                   break;
    case Opcodes8080::ORA_M:    registers.a = Or(registers.a, memory->Fetch8(registers.hl.W), registers.flags);     break;
    case Opcodes8080::ORA_A:    registers.a = Or(registers.a, registers.a, registers.flags);                        break;
    case Opcodes8080::ADI:      registers.a = Add(registers.a, FetchInstructionByte(), registers.flags);            break;
    case Opcodes8080::ACI:      registers.a = AddC(registers.a, FetchInstructionByte(), registers.flags);           break;
    case Opcodes8080::SUI:      registers.a = Sub(registers.a, FetchInstructionByte(), registers.flags);            break;
    case Opcodes8080::SBI:      registers.a = SubC(registers.a, FetchInstructionByte(), registers.flags);           break;
    case Opcodes8080::ANI:      registers.a = And(registers.a, FetchInstructionByte(), registers.flags);            break;
    case Opcodes8080::XCHG:     std::swap(registers.de.W, registers.hl.W);                                          break;
    case Opcodes8080::XRI:      registers.a = Xor(registers.a, FetchInstructionByte(), registers.flags);            break;
    case Opcodes8080::EI:       inte = true;                                                                        break;
    case Opcodes8080::ORI:      registers.a = Or(registers.a, FetchInstructionByte(), registers.flags);             break;
    case Opcodes8080::DI:       inte = false;                                                                       break;

    default:
        std::cout << "Unrecognized instruction: " << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << instruction << " at PC=" << std::setw(2) << registers.pc - 2 << std::endl;
    }

}

bool Processor8080::RunInstruction()
{
    register InterruptFlags8080 interruptFlag;

    FetchInstruction();
    ExecuteInstruction();
    // If cycle counter expired...
    if (registers.cycleCount <= 0)
    {
        if (InterruptPending())
        {
            ResetInterrupt();
            registers.cycleCount += registers.cycleCountBackup - 1;     // Restore the interrupt count

                                                                        // Call periodic handler or set pending IRQ
            if (registers.cycleCount > 0)
                interruptFlag = registers.interruptRequest;
            else
            {
                interruptFlag = Loop();                               // Call periodic handler
                registers.cycleCount += registers.cycleCountPeriod;     // Reset the cycle counter
                if (interruptFlag == InterruptFlags8080::None)
                    interruptFlag = registers.interruptRequest;       // Pending IRQ
            }
        }
        else
        {
            interruptFlag = Loop();                                   // Call periodic handler
            registers.cycleCount += registers.cycleCountPeriod;         // Reset the cycle counter
            if (interruptFlag == InterruptFlags8080::None)
                interruptFlag = registers.interruptRequest;           // Pending IRQ
        }

        if (interruptFlag == InterruptFlags8080::Quit)
            return false; /* Exit if INT_QUIT */
        if (interruptFlag != InterruptFlags8080::None)
            HandleInterrupt(interruptFlag);   /* Int-pt if needed */
    }
    return true;
}

void Processor8080::Run()
{
    while (RunInstruction())
    {
    }
}

InstructionData8080 Processor8080::GetInstructionData()
{
    return GetInstructionData(instruction);
}

InstructionData8080 Processor8080::GetInstructionData(Opcodes8080 opcode)
{
    return instructiond8080[int(opcode)];
}

size_t Processor8080::DisassembleInstruction(std::vector<uint8_t> const & machineCode, std::string & mnemonic)
{
    std::ostringstream stream;
    Opcodes8080 opcode = Opcodes8080(machineCode[0]);
    stream << instructiond8080[machineCode[0]].instructionMnemonic;
    size_t instructionSize = instructiond8080[machineCode[0]].instructionSize;
    switch (opcode)
    {
    case Opcodes8080::SHLD:
    case Opcodes8080::LHLD:
    case Opcodes8080::STA:
    case Opcodes8080::LDA:      
    case Opcodes8080::LXI_B:
    case Opcodes8080::LXI_D:
    case Opcodes8080::LXI_H:
    case Opcodes8080::LXI_SP:
    case Opcodes8080::JMP:
    case Opcodes8080::JNZ:
    case Opcodes8080::JZ:
    case Opcodes8080::JNC:
    case Opcodes8080::JC:
    case Opcodes8080::JPO:
    case Opcodes8080::JPE:
    case Opcodes8080::JP:
    case Opcodes8080::JM:
    case Opcodes8080::CALL:
    case Opcodes8080::CNZ:
    case Opcodes8080::CZ:
    case Opcodes8080::CNC:
    case Opcodes8080::CC:
    case Opcodes8080::CPO:
    case Opcodes8080::CPE:
    case Opcodes8080::CP:
    case Opcodes8080::CM:
        stream << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << int(machineCode[2])
            << std::setw(2) << std::setfill('0') << int(machineCode[1]);
        break;

    case Opcodes8080::MVI_M:
    case Opcodes8080::ADI:
    case Opcodes8080::ACI:
    case Opcodes8080::SUI:
    case Opcodes8080::SBI:
    case Opcodes8080::ANI:
    case Opcodes8080::XRI:
    case Opcodes8080::ORI:
    case Opcodes8080::CPI:
    case Opcodes8080::INP:
    case Opcodes8080::OUTP:
        stream << std::hex << std::setw(2) << std::setfill('0') << std::uppercase << int(machineCode[1]);
        break;

    case Opcodes8080::NOP:
    case Opcodes8080::STAX_B:
    case Opcodes8080::INX_B:
    case Opcodes8080::INR_B:
    case Opcodes8080::DCR_B:
    case Opcodes8080::RLC:
    case Opcodes8080::DAD_B:
    case Opcodes8080::LDAX_B:
    case Opcodes8080::DCX_B:
    case Opcodes8080::INR_C:
    case Opcodes8080::DCR_C:
    case Opcodes8080::RRC:
    case Opcodes8080::STAX_D:
    case Opcodes8080::INX_D:
    case Opcodes8080::INR_D:
    case Opcodes8080::DCR_D:
    case Opcodes8080::RAL:
    case Opcodes8080::DAD_D:
    case Opcodes8080::LDAX_D:
    case Opcodes8080::DCX_D:
    case Opcodes8080::INR_E:
    case Opcodes8080::DCR_E:
    case Opcodes8080::RAR:
    case Opcodes8080::INX_H:
    case Opcodes8080::INR_H:
    case Opcodes8080::DCR_H:
    case Opcodes8080::DAA:
    case Opcodes8080::DAD_H:
    case Opcodes8080::DCX_H:
    case Opcodes8080::INR_L:
    case Opcodes8080::DCR_L:
    case Opcodes8080::CMA:
    case Opcodes8080::INX_SP:
    case Opcodes8080::INR_M:
    case Opcodes8080::DCR_M:
    case Opcodes8080::STC:
    case Opcodes8080::DAD_SP:
    case Opcodes8080::DCX_SP:
    case Opcodes8080::INR_A:
    case Opcodes8080::DCR_A:
    case Opcodes8080::CMC:
    case Opcodes8080::MOV_B_B:
    case Opcodes8080::MOV_B_C:
    case Opcodes8080::MOV_B_D:
    case Opcodes8080::MOV_B_E:
    case Opcodes8080::MOV_B_H:
    case Opcodes8080::MOV_B_L:
    case Opcodes8080::MOV_B_M:
    case Opcodes8080::MOV_B_A:
    case Opcodes8080::MOV_C_B:
    case Opcodes8080::MOV_C_C:
    case Opcodes8080::MOV_C_D:
    case Opcodes8080::MOV_C_E:
    case Opcodes8080::MOV_C_H:
    case Opcodes8080::MOV_C_L:
    case Opcodes8080::MOV_C_M:
    case Opcodes8080::MOV_C_A:
    case Opcodes8080::MOV_D_B:
    case Opcodes8080::MOV_D_C:
    case Opcodes8080::MOV_D_D:
    case Opcodes8080::MOV_D_E:
    case Opcodes8080::MOV_D_H:
    case Opcodes8080::MOV_D_L:
    case Opcodes8080::MOV_D_M:
    case Opcodes8080::MOV_D_A:
    case Opcodes8080::MOV_E_B:
    case Opcodes8080::MOV_E_C:
    case Opcodes8080::MOV_E_D:
    case Opcodes8080::MOV_E_E:
    case Opcodes8080::MOV_E_H:
    case Opcodes8080::MOV_E_L:
    case Opcodes8080::MOV_E_M:
    case Opcodes8080::MOV_E_A:
    case Opcodes8080::MOV_H_B:
    case Opcodes8080::MOV_H_C:
    case Opcodes8080::MOV_H_D:
    case Opcodes8080::MOV_H_E:
    case Opcodes8080::MOV_H_H:
    case Opcodes8080::MOV_H_L:
    case Opcodes8080::MOV_H_M:
    case Opcodes8080::MOV_H_A:
    case Opcodes8080::MOV_L_B:
    case Opcodes8080::MOV_L_C:
    case Opcodes8080::MOV_L_D:
    case Opcodes8080::MOV_L_E:
    case Opcodes8080::MOV_L_H:
    case Opcodes8080::MOV_L_L:
    case Opcodes8080::MOV_L_M:
    case Opcodes8080::MOV_L_A:
    case Opcodes8080::MOV_M_B:
    case Opcodes8080::MOV_M_C:
    case Opcodes8080::MOV_M_D:
    case Opcodes8080::MOV_M_E:
    case Opcodes8080::MOV_M_H:
    case Opcodes8080::MOV_M_L:
    case Opcodes8080::HLT:
    case Opcodes8080::MOV_M_A:
    case Opcodes8080::MOV_A_B:
    case Opcodes8080::MOV_A_C:
    case Opcodes8080::MOV_A_D:
    case Opcodes8080::MOV_A_E:
    case Opcodes8080::MOV_A_H:
    case Opcodes8080::MOV_A_L:
    case Opcodes8080::MOV_A_M:
    case Opcodes8080::MOV_A_A:
    case Opcodes8080::ADD_B:
    case Opcodes8080::ADD_C:
    case Opcodes8080::ADD_D:
    case Opcodes8080::ADD_E:
    case Opcodes8080::ADD_H:
    case Opcodes8080::ADD_L:
    case Opcodes8080::ADD_M:
    case Opcodes8080::ADD_A:
    case Opcodes8080::ADC_B:
    case Opcodes8080::ADC_C:
    case Opcodes8080::ADC_D:
    case Opcodes8080::ADC_E:
    case Opcodes8080::ADC_H:
    case Opcodes8080::ADC_L:
    case Opcodes8080::ADC_M:
    case Opcodes8080::ADC_A:
    case Opcodes8080::SUB_B:
    case Opcodes8080::SUB_C:
    case Opcodes8080::SUB_D:
    case Opcodes8080::SUB_E:
    case Opcodes8080::SUB_H:
    case Opcodes8080::SUB_L:
    case Opcodes8080::SUB_M:
    case Opcodes8080::SUB_A:
    case Opcodes8080::SBB_B:
    case Opcodes8080::SBB_C:
    case Opcodes8080::SBB_D:
    case Opcodes8080::SBB_E:
    case Opcodes8080::SBB_H:
    case Opcodes8080::SBB_L:
    case Opcodes8080::SBB_M:
    case Opcodes8080::SBB_A:
    case Opcodes8080::ANA_B:
    case Opcodes8080::ANA_C:
    case Opcodes8080::ANA_D:
    case Opcodes8080::ANA_E:
    case Opcodes8080::ANA_H:
    case Opcodes8080::ANA_L:
    case Opcodes8080::ANA_M:
    case Opcodes8080::ANA_A:
    case Opcodes8080::XRA_B:
    case Opcodes8080::XRA_C:
    case Opcodes8080::XRA_D:
    case Opcodes8080::XRA_E:
    case Opcodes8080::XRA_H:
    case Opcodes8080::XRA_L:
    case Opcodes8080::XRA_M:
    case Opcodes8080::XRA_A:
    case Opcodes8080::ORA_B:
    case Opcodes8080::ORA_C:
    case Opcodes8080::ORA_D:
    case Opcodes8080::ORA_E:
    case Opcodes8080::ORA_H:
    case Opcodes8080::ORA_L:
    case Opcodes8080::ORA_M:
    case Opcodes8080::ORA_A:
    case Opcodes8080::CMP_B:
    case Opcodes8080::CMP_C:
    case Opcodes8080::CMP_D:
    case Opcodes8080::CMP_E:
    case Opcodes8080::CMP_H:
    case Opcodes8080::CMP_L:
    case Opcodes8080::CMP_M:
    case Opcodes8080::CMP_A:
    case Opcodes8080::RET:
    case Opcodes8080::RNZ:
    case Opcodes8080::RZ:
    case Opcodes8080::RNC:
    case Opcodes8080::RC:
    case Opcodes8080::RPO:
    case Opcodes8080::RPE:
    case Opcodes8080::RP:
    case Opcodes8080::RM:
    case Opcodes8080::XCHG:
    case Opcodes8080::EI:
    case Opcodes8080::DI:
    case Opcodes8080::RST_0:
    case Opcodes8080::RST_1:
    case Opcodes8080::RST_2:
    case Opcodes8080::RST_3:
    case Opcodes8080::RST_4:
    case Opcodes8080::RST_5:
    case Opcodes8080::RST_6:
    case Opcodes8080::RST_7:
    case Opcodes8080::PCHL:
    case Opcodes8080::XTHL:
    case Opcodes8080::SPHL:
    case Opcodes8080::PUSH_B:
    case Opcodes8080::PUSH_D:
    case Opcodes8080::PUSH_H:
    case Opcodes8080::PUSH_PSW:
    case Opcodes8080::POP_B:
    case Opcodes8080::POP_D:
    case Opcodes8080::POP_H:
    case Opcodes8080::POP_PSW:
        break;

    default:
        throw DisassemblerUnknownInstructionException(machineCode[0]);
        break;
    }
    mnemonic = stream.str();
    return instructionSize;
}

InstructionParserData const * FindOpcode(std::string const & opcodeString)
{
    size_t index = 0;
    for (auto & instructionData : instructionParserData)
    {
        if (instructionData.instructionMnemonic == opcodeString)
            return &(instructionParserData[index]);
        index++;
    }
    return nullptr;
}

std::string trim(std::string const & text)
{
    size_t left = 0;
    size_t right = text.length();
    while ((left < text.length()) && isspace(text[left]))
        ++left;
    while ((right > 0) && !isspace(text[right - 1]))
        --right;
    return text.substr(left, right - left);
}

uint8_t GetRegisterSelector(std::string const & text)
{
    if (text == "B")
        return 0;
    if (text == "C")
        return 1;
    if (text == "D")
        return 2;
    if (text == "E")
        return 3;
    if (text == "H")
        return 4;
    if (text == "L")
        return 5;
    if (text == "M")
        return 6;
    if (text == "A")
        return 7;
    return 0xFF;
}

uint8_t GetRegisterPairSelector(std::string const & text)
{
    if (text == "B")
        return 0;
    if (text == "D")
        return 1;
    if (text == "H")
        return 2;
    if (text == "SP")
        return 3;
    return 0xFF;
}

uint8_t GetRegisterPairSelectorPushPop(std::string const & text)
{
    if (text == "B")
        return 0;
    if (text == "D")
        return 1;
    if (text == "H")
        return 2;
    if (text == "PSW")
        return 3;
    return 0xFF;
}

uint8_t GetRestartSelector(std::string const & text)
{
    if (text == "0")
        return 0;
    if (text == "1")
        return 1;
    if (text == "2")
        return 2;
    if (text == "3")
        return 3;
    if (text == "4")
        return 4;
    if (text == "5")
        return 5;
    if (text == "6")
        return 6;
    if (text == "7")
        return 7;
    return 0xFF;
}

std::string BuildExpectedMnemonic(std::string const & opcodeString, InstructionOptions8080 const & options)
{
    std::ostringstream stream;
    stream << opcodeString;
    if (!options.empty())
    {
        std::string infix = " ";
        for (auto & option : options)
        {
            stream << infix << option;
            infix = ",";
        }
    }
    return stream.str();
}

size_t Processor8080::AssembleInstruction(std::string const & mnemonic, std::vector<uint8_t> & machineCode)
{
    std::string opcodeString;
    StringVector options;

    ParseInstruction(mnemonic, opcodeString, options);
    InstructionParserData const * instructionData = FindOpcode(opcodeString);
    if (!instructionData)
    {
        throw AssemblerUnknownOpcodeException(opcodeString);
    }
    if (instructionData->instructionOptions.size() != options.size())
    {
        throw AssemblerInvalidOptionException(BuildExpectedMnemonic(opcodeString, instructionData->instructionOptions), mnemonic);
    }
    uint8_t opcodeByte = uint8_t(instructionData->opcodeByte);
    uint8_t instructionSize = 1;
    uint8_t selectorSrce = 0xFF;
    uint8_t selectorDest = 0xFF;
    uint8_t byte2 = 0x00;
    uint8_t byte3 = 0x00;
    StringVector::const_iterator optionIndex = options.begin();
    for (auto & option : instructionData->instructionOptions)
    {
        switch (option)
        {
        case InstructionOption8080::regm8_s0:
            {
                selectorSrce = GetRegisterSelector(*optionIndex);
                if (selectorSrce == 0xFF)
                {
                    throw AssemblerInvalidOptionException(BuildExpectedMnemonic(opcodeString, instructionData->instructionOptions), mnemonic);
                }
                opcodeByte |= selectorSrce << 0;
            }
            break;
        case InstructionOption8080::regm8_d3:
            {
                selectorDest = GetRegisterSelector(*optionIndex);
                if (selectorDest == 0xFF)
                {
                    throw AssemblerInvalidOptionException(BuildExpectedMnemonic(opcodeString, instructionData->instructionOptions), mnemonic);
                }
                opcodeByte |= selectorDest << 3;
            }
            break;
        case InstructionOption8080::regm8_d0:
            {
                selectorDest = GetRegisterSelector(*optionIndex);
                if (selectorDest == 0xFF)
                {
                    throw AssemblerInvalidOptionException(BuildExpectedMnemonic(opcodeString, instructionData->instructionOptions), mnemonic);
                }
                opcodeByte |= selectorDest << 0;
            }
            break;
        case InstructionOption8080::reg16_4:
            {
                selectorDest = GetRegisterPairSelector(*optionIndex);
                if (selectorDest == 0xFF)
                {
                    throw AssemblerInvalidOptionException(BuildExpectedMnemonic(opcodeString, instructionData->instructionOptions), mnemonic);
                }
                opcodeByte |= selectorDest << 4;
            }
            break;
        case InstructionOption8080::reg16_psw_4:
            {
                selectorDest = GetRegisterPairSelectorPushPop(*optionIndex);
                if (selectorDest == 0xFF)
                {
                    throw AssemblerInvalidOptionException(BuildExpectedMnemonic(opcodeString, instructionData->instructionOptions), mnemonic);
                }
                opcodeByte |= selectorDest << 4;
            }
            break;
        case InstructionOption8080::n3:
            {
                selectorDest = GetRestartSelector(*optionIndex);
                if (selectorDest == 0xFF)
                {
                    throw AssemblerInvalidOptionException(BuildExpectedMnemonic(opcodeString, instructionData->instructionOptions), mnemonic);
                }
                opcodeByte |= selectorDest << 3;
            }
            break;
        case InstructionOption8080::d8:
        case InstructionOption8080::p8:
            {
                if (!Core::Util::TryParse(*optionIndex, byte2, 16))
                {
                    throw AssemblerInvalidOptionException(BuildExpectedMnemonic(opcodeString, instructionData->instructionOptions), mnemonic);
                }
                instructionSize = 2;
            }
            break;
        case InstructionOption8080::d16:
        case InstructionOption8080::a16:
            {
                Reg16LH data;
                if (!Core::Util::TryParse(*optionIndex, data.W, 16))
                {
                    throw AssemblerInvalidOptionException(BuildExpectedMnemonic(opcodeString, instructionData->instructionOptions), mnemonic);
                }
                byte2 = data.B.l;
                byte3 = data.B.h;
                instructionSize = 3;
            }
            break;
        default:
            throw std::runtime_error("Invalid option");
        }
        ++optionIndex;
    }
    switch (instructionData->opcodeByte)
    {
    case OpcodesRaw8080::MOV:
        if ((selectorSrce == Reg8Selector::M) && (selectorDest == Reg8Selector::M))
        {
            throw AssemblerInvalidOptionException(BuildExpectedMnemonic(opcodeString, instructionData->instructionOptions), mnemonic);
        }
        break;
    case OpcodesRaw8080::LDAX:
    case OpcodesRaw8080::STAX:
        if ((selectorDest == Reg16Selector::HL) || (selectorDest == Reg16Selector::SP))
        {
            throw AssemblerInvalidOptionException(BuildExpectedMnemonic(opcodeString, instructionData->instructionOptions), mnemonic);
        }
        break;
    default:
        break;
    }

    machineCode.push_back(opcodeByte);
    if (instructionSize > 1)
        machineCode.push_back(byte2);
    if (instructionSize > 2)
        machineCode.push_back(byte3);

    return instructionSize;
}

void Processor8080::Disassemble(std::vector<uint8_t> const & machineCode, std::ostream & disassembledCode)
{
    
}

void Processor8080::Assemble(std::string const & disassembledCode, std::vector<uint8_t> & machineCode)
{

}

void Processor8080::PrintRegisterValues(std::ostream & stream)
{

}

std::vector<std::string> Processor8080::GetRegisterNames()
{
    std::vector<std::string> names;
    names.push_back("B");
    names.push_back("C");
    names.push_back("D");
    names.push_back("E");
    names.push_back("H");
    names.push_back("L");
    names.push_back("A");
    names.push_back("F");
    names.push_back("SP");
    names.push_back("PC");
    return names;
}

bool Processor8080::InterruptPending()
{
    return false;
}

void Processor8080::ResetInterrupt()
{

}

void Processor8080::HandleInterrupt(InterruptFlags8080 flags)
{
}

InterruptFlags8080 Processor8080::Loop()
{
    return InterruptFlags8080::None;
}

uint8_t Processor8080::FetchInstructionByte()
{
    return memory->Fetch8(registers.pc++);
}

void Processor8080::ParseInstruction(std::string const & mnemonic, std::string & opcodeString, StringVector & options)
{
    opcodeString.clear();
    options.clear();
    std::string::const_iterator index = mnemonic.begin();
    while ((index != mnemonic.end()) && isspace(*index))
        index++;
    while ((index != mnemonic.end()) && !isspace(*index))
        opcodeString += *index++;
    while ((index != mnemonic.end()) && isspace(*index))
        index++;
    while (index != mnemonic.end())
    {
        std::string option;
        while ((index != mnemonic.end()) && (*index != ',') && !isspace(*index))
        {
            option += *index++;
        }
        while ((index != mnemonic.end()) && ((*index == ',') || isspace(*index)))
            ++index;
        options.push_back(option);
    }
}

} // namespace Simulate