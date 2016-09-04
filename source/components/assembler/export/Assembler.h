#pragma once

#include <iostream>
#include <vector>
#include "AssemblerMessage.h"
#include "CharClass.h"
#include "Scanner.h"
#include "Token.h"
#include "simple-processor/simplemachine.h"

namespace Assembler
{

enum class OpcodeType : uint8_t
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


class Assembler
{
public:
    using AssemblerMessages = std::vector<AssemblerMessage>;
    Assembler(Simulate::SimpleMachine & machine, std::istream & source, std::wostream & reportStream);
    virtual ~Assembler();
    bool Assemble(std::vector<uint8_t> & machineCode);
    AssemblerMessages const & GetMessages() { return messages; }

protected:
    Simulate::SimpleMachine & machine;
    std::istream & source;
    AssemblerMessages messages;
    Scanner scanner;
    KeywordMap<OpcodeType> opcodeKeywords;

    CharClass ExtractCharacter();
    Token ExtractToken();
    bool HandleToken();
};

} // namespace Assembler
