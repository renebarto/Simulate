#pragma once

#include "assembler/CPUParser.h"
#include "assembler/SymbolList.h"

namespace Assembler
{

enum class OpcodeType : uint16_t
{
    Invalid = 0xFFFF,
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

enum class Register8Type : uint8_t
{
    Invalid = 0xFF,
    B = 0x00,
    C = 0x01,
    D = 0x02,
    E = 0x03,
    H = 0x04,
    L = 0x05,
    M = 0x06,
    A = 0x07,
};

enum class Register16Type : uint8_t
{
    Invalid = 0xFF,
    BC = 0x00,
    DE = 0x01,
    HL = 0x02,
    SP = 0x03,
    PSW = 0x04,
};

enum class RSTCode : uint8_t
{
    Invalid = 0xFF,
    RST0 = 0x00,
    RST1 = 0x01,
    RST2 = 0x02,
    RST3 = 0x03,
    RST4 = 0x04,
    RST5 = 0x05,
    RST6 = 0x06,
    RST7 = 0x07,
};

enum class OperandType
{
    Invalid,
    Simple,
    R8_R8,       // ddd, sss
    R8,         // ddd / sss
    R8_D8,       // ddd / sss + 8 bit immediate
    R16_BD,     // rp: BC or DE
    R16_BDH_PSW,// rp: BC, DE, HL or PSW (PUSH/POP)
    R16_BDH_SP, // rp: BC, DE, HL or SP
    R16_BDH_SP_D16,// rp: BC, DE, HL or SP + 16 bit immediate
    D8,         // 8 bit immediate
    P8,         // 8 bit immediate port number
    D16,        // 16 bit immediate
    A16,        // 16 bit immediate address
    N,          // Restart code (0-7)
};

class RSTNode : public ASTNode
{
public:
    RSTNode(RSTCode rstCode, std::wstring const & value, Location const & location)
        : ASTNode(ASTNodeType::RSTCode, value, location)
        , rstCode(rstCode)
    {}
    virtual ~RSTNode() {}

    RSTCode GetRSTCode() const { return rstCode; }
private:
    RSTCode rstCode;    
};

using SegmentType = uint8_t;
using AddressType = uint16_t;

class CPUParserIntel8080_8085 : public CPUParser<OpcodeType, OperandType, SegmentType, AddressType>
{
public:
	CPUParserIntel8080_8085(CPUType cpuType, Scanner & scanner, ErrorHandler & errorHandler, PrettyPrinter<wchar_t> & printer);
	virtual ~CPUParserIntel8080_8085();

    void Print();
    void Print(ASTNode::Ptr node, size_t & line);
    void PrintWithErrors() override;
    void PrintSymbolTable() override;
    void PrintSymbolCrossReference() override;
    void DumpAST(std::wostream & stream, size_t startColumn) override;

private:
    using AddressType = uint16_t;
    using InstructionMapping8080 = InstructionMapping<OperandType, AddressType>;
    KeywordMap<Register8Type> registers8;
    KeywordMap<Register16Type> registers16;
    KeywordMap<RSTCode> rstCodes;
  	OpcodeMap<OpcodeType, InstructionMapping8080> instructionData;

    void Init();

    Register8Node<Register8Type>::Ptr CreateRegisterNode(Register8Type registerType, std::wstring const & value, Location const & location);
    Register16Node<Register16Type>::Ptr CreateRegisterNode(Register16Type registerType, std::wstring const & value, Location const & location);
    RSTNode::Ptr CreateRSTNode(RSTCode rstCode, std::wstring const & value, Location const & location);
    void HandleOpcodeAndOperands(LabelNode::Ptr label) override;
    void HandleOperands(OpcodeNode<OpcodeType, AddressType>::Ptr opcode, OperandType state) override;

    void PrintWithErrors(ASTNode::Ptr node, size_t & line, AssemblerMessages::const_iterator & it);
    void Print(ASTNode::Ptr node);
    void ParseExpression8(OpcodeNode<OpcodeType, AddressType>::Ptr opcode);
    void ParseExpression16(OpcodeNode<OpcodeType, AddressType>::Ptr opcode);
    void DumpNode(PrettyPrinter<wchar_t> & printer, ASTNode::Ptr node, size_t startColumn);
    void ScanOperandNodeForReferences(ASTNode::Ptr node, SymbolList<SymbolReference> & list);
    void ScanOpcodeNodeForReferences(OpcodeNode<OpcodeType, AddressType>::Ptr node, SymbolList<SymbolReference> & list);
    void ScanASTForReferences(SymbolList<SymbolReference> & list);
}; // CPUParserIntel8080_8085

} // namespace Assembler
