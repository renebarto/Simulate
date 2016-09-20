#include "CPUAssemblerIntel8080_8085.h"

#include <limits>
#include "core/Util.h"
#include "Exceptions.h"
#include "Nodes.h"

namespace Assembler
{

CPUAssemblerIntel8080_8085::CPUAssemblerIntel8080_8085(CPUType cpuType, ErrorHandler & errorHandler, std::wostream & reportStream)
    : cpuType(cpuType)
    , localErrors()
    , errorHandler(errorHandler)
    , reportStream(reportStream)
    , instructionData()
{
    Init();
}

CPUAssemblerIntel8080_8085::~CPUAssemblerIntel8080_8085()
{
}

void CPUAssemblerIntel8080_8085::Init()
{
    instructionData.Set(OpcodeType::MOV,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::RegM8_D3_RegM8_S0, 0, 0, 0, 0, size_t{ 1 }, L"MOV" });
    instructionData.Set(OpcodeType::MVI,  InstructionData8080 { 0x06 ,InstructionOperandIntel8080_8085::RegM8_D3_I8, 0, 0, 0, 0, size_t{ 2 }, L"MVI" });
    instructionData.Set(OpcodeType::LXI,  InstructionData8080 { 0x01, InstructionOperandIntel8080_8085::Reg16_D4_I16, 0, 0, 0, 0, size_t{ 3 }, L"LXI" });
    instructionData.Set(OpcodeType::STAX, InstructionData8080 { 0x02, InstructionOperandIntel8080_8085::Reg16_D4_BD, 0, 0, 0, 0, size_t{ 1 }, L"STAX" });
    instructionData.Set(OpcodeType::LDAX, InstructionData8080 { 0x0A, InstructionOperandIntel8080_8085::Reg16_D4_BD, 0, 0, 0, 0, size_t{ 1 }, L"LDAX" });
    instructionData.Set(OpcodeType::STA,  InstructionData8080 { 0x32, InstructionOperandIntel8080_8085::I16, 0, 0, 0, 0, size_t{ 3 }, L"STA" });
    instructionData.Set(OpcodeType::LDA,  InstructionData8080 { 0x3A, InstructionOperandIntel8080_8085::I16, 0, 0, 0, 0, size_t{ 3 }, L"LDA" });
    instructionData.Set(OpcodeType::SHLD, InstructionData8080 { 0x22, InstructionOperandIntel8080_8085::I16, 0, 0, 0, 0, size_t{ 3 }, L"SHLD" });
    instructionData.Set(OpcodeType::LHLD, InstructionData8080 { 0x2A, InstructionOperandIntel8080_8085::I16, 0, 0, 0, 0, size_t{ 3 }, L"LHLD" });
    instructionData.Set(OpcodeType::XCHG, InstructionData8080 { 0xEB, InstructionOperandIntel8080_8085::None, 0, 0, 0, 0, size_t{ 1 }, L"XCHG" });
    instructionData.Set(OpcodeType::PUSH, InstructionData8080 { 0xC5, InstructionOperandIntel8080_8085::Reg16_D4_BDHPSW, 0, 0, 0, 0, size_t{ 1 }, L"PUSH" });
    instructionData.Set(OpcodeType::POP,  InstructionData8080 { 0xC1, InstructionOperandIntel8080_8085::Reg16_D4_BDHPSW, 0, 0, 0, 0, size_t{ 1 }, L"POP" });
    instructionData.Set(OpcodeType::XTHL, InstructionData8080 { 0xE3, InstructionOperandIntel8080_8085::None, 0, 0, 0, 0, size_t{ 1 }, L"XTHL" });
    instructionData.Set(OpcodeType::SPHL, InstructionData8080 { 0xF9, InstructionOperandIntel8080_8085::None, 0, 0, 0, 0, size_t{ 1 }, L"SPHL" });
    instructionData.Set(OpcodeType::INX,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::R16_BDH_SP, 0, 0, 0, 0, size_t{ 1 }, L"INX" });
    instructionData.Set(OpcodeType::DCX,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::R16_BDH_SP, 0, 0, 0, 0, size_t{ 1 }, L"DCX" });
    instructionData.Set(OpcodeType::CALL, InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::A16, 0, 0, 0, 0, size_t{ 3 }, L"CALL" });
    instructionData.Set(OpcodeType::CC,   InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::A16, 0, 0, 0, 0, size_t{ 3 }, L"CC" });
    instructionData.Set(OpcodeType::CNC,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::A16, 0, 0, 0, 0, size_t{ 3 }, L"CNC" });
    instructionData.Set(OpcodeType::CZ,   InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::A16, 0, 0, 0, 0, size_t{ 3 }, L"CZ" });
    instructionData.Set(OpcodeType::CNZ,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::A16, 0, 0, 0, 0, size_t{ 3 }, L"CNZ" });
    instructionData.Set(OpcodeType::CP,   InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::A16, 0, 0, 0, 0, size_t{ 3 }, L"CP" });
    instructionData.Set(OpcodeType::CM,   InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::A16, 0, 0, 0, 0, size_t{ 3 }, L"CM" });
    instructionData.Set(OpcodeType::CPE,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::A16, 0, 0, 0, 0, size_t{ 3 }, L"CPE" });
    instructionData.Set(OpcodeType::CPO,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::A16, 0, 0, 0, 0, size_t{ 3 }, L"CPO" });
    instructionData.Set(OpcodeType::PCHL, InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"PCHL" });
    instructionData.Set(OpcodeType::JMP,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::A16, 0, 0, 0, 0, size_t{ 3 }, L"JMP" });
    instructionData.Set(OpcodeType::JC,   InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::A16, 0, 0, 0, 0, size_t{ 3 }, L"JC" });
    instructionData.Set(OpcodeType::JNC,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::A16, 0, 0, 0, 0, size_t{ 3 }, L"JNC" });
    instructionData.Set(OpcodeType::JZ,   InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::A16, 0, 0, 0, 0, size_t{ 3 }, L"JZ" });
    instructionData.Set(OpcodeType::JNZ,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::A16, 0, 0, 0, 0, size_t{ 3 }, L"JNZ" });
    instructionData.Set(OpcodeType::JP,   InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::A16, 0, 0, 0, 0, size_t{ 3 }, L"JP" });
    instructionData.Set(OpcodeType::JM,   InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::A16, 0, 0, 0, 0, size_t{ 3 }, L"JM" });
    instructionData.Set(OpcodeType::JPE,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::A16, 0, 0, 0, 0, size_t{ 3 }, L"JPE" });
    instructionData.Set(OpcodeType::JPO,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::A16, 0, 0, 0, 0, size_t{ 3 }, L"JPO" });
    instructionData.Set(OpcodeType::RET,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"RET" });
    instructionData.Set(OpcodeType::RC,   InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"RC" });
    instructionData.Set(OpcodeType::RNC,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"RNC" });
    instructionData.Set(OpcodeType::RZ,   InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"RZ" });
    instructionData.Set(OpcodeType::RNZ,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"RNZ" });
    instructionData.Set(OpcodeType::RP,   InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"RP" });
    instructionData.Set(OpcodeType::RM,   InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"RM" });
    instructionData.Set(OpcodeType::RPE,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"RPE" });
    instructionData.Set(OpcodeType::RPO,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"RPO" });
    instructionData.Set(OpcodeType::RST,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::N, 0, 0, 0, 0, size_t{ 1 }, L"RST" });
    instructionData.Set(OpcodeType::INR,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::R8, 0, 0, 0, 0, size_t{ 1 }, L"INR" });
    instructionData.Set(OpcodeType::DCR,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::R8, 0, 0, 0, 0, size_t{ 1 }, L"DCR" });
    instructionData.Set(OpcodeType::ADD,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::R8, 0, 0, 0, 0, size_t{ 1 }, L"ADD" });
    instructionData.Set(OpcodeType::ADC,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::R8, 0, 0, 0, 0, size_t{ 1 }, L"ADC" });
    instructionData.Set(OpcodeType::SUB,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::R8, 0, 0, 0, 0, size_t{ 1 }, L"SUB" });
    instructionData.Set(OpcodeType::SBB,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::R8, 0, 0, 0, 0, size_t{ 1 }, L"SBB" });
    instructionData.Set(OpcodeType::DAD,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::R16_BDH_SP, 0, 0, 0, 0, size_t{ 1 }, L"DAD" });
    instructionData.Set(OpcodeType::ANA,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::R8, 0, 0, 0, 0, size_t{ 1 }, L"ANA" });
    instructionData.Set(OpcodeType::ORA,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::R8, 0, 0, 0, 0, size_t{ 1 }, L"ORA" });
    instructionData.Set(OpcodeType::XRA,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::R8, 0, 0, 0, 0, size_t{ 1 }, L"XRA" });
    instructionData.Set(OpcodeType::CMP,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::R8, 0, 0, 0, 0, size_t{ 1 }, L"CMP" });
    instructionData.Set(OpcodeType::ADI,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::D8, 0, 0, 0, 0, size_t{ 2 }, L"ADI" });
    instructionData.Set(OpcodeType::ACI,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::D8, 0, 0, 0, 0, size_t{ 2 }, L"ACI" });
    instructionData.Set(OpcodeType::SUI,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::D8, 0, 0, 0, 0, size_t{ 2 }, L"SUI" });
    instructionData.Set(OpcodeType::SBI,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::D8, 0, 0, 0, 0, size_t{ 2 }, L"SBI" });
    instructionData.Set(OpcodeType::ANI,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::D8, 0, 0, 0, 0, size_t{ 2 }, L"ANI" });
    instructionData.Set(OpcodeType::ORI,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::D8, 0, 0, 0, 0, size_t{ 2 }, L"ORI" });
    instructionData.Set(OpcodeType::XRI,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::D8, 0, 0, 0, 0, size_t{ 2 }, L"XRI" });
    instructionData.Set(OpcodeType::CPI,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::D8, 0, 0, 0, 0, size_t{ 2 }, L"CPI" });
    instructionData.Set(OpcodeType::RLC,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"RLC" });
    instructionData.Set(OpcodeType::RRC,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"RRC" });
    instructionData.Set(OpcodeType::RAL,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"RAL" });
    instructionData.Set(OpcodeType::RAR,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"RAR" });
    instructionData.Set(OpcodeType::CMA,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"CMA" });
    instructionData.Set(OpcodeType::STC,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"STC" });
    instructionData.Set(OpcodeType::CMC,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"CMC" });
    instructionData.Set(OpcodeType::DAA,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"DAA" });
    instructionData.Set(OpcodeType::INP,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::P8, 0, 0, 0, 0, size_t{ 2 }, L"IN" });
    instructionData.Set(OpcodeType::OUTP, InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::P8, 0, 0, 0, 0, size_t{ 2 }, L"OUT" });
    instructionData.Set(OpcodeType::EI,   InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"DI" });
    instructionData.Set(OpcodeType::DI,   InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"EI" });
    instructionData.Set(OpcodeType::NOP,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"NOP" });
    instructionData.Set(OpcodeType::HLT,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"HLT" });

    if (cpuType == CPUType::Intel8085)
    {
    	instructionData.Set(OpcodeType::DSUB, InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"DSUB" });
    	instructionData.Set(OpcodeType::ARHL, InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"ARHL" });
    	instructionData.Set(OpcodeType::RDEL, InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"RDEL" });
    	instructionData.Set(OpcodeType::RIM,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"RIM" });
    	instructionData.Set(OpcodeType::LDHI, InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::D8, 0, 0, 0, 0, size_t{ 2 }, L"LDHI" });
    	instructionData.Set(OpcodeType::SIM,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"SIM" });
    	instructionData.Set(OpcodeType::LDSI, InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::D8, 0, 0, 0, 0, size_t{ 2 }, L"LDSI" });
    	instructionData.Set(OpcodeType::RSTV, InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"RSTV" });
    	instructionData.Set(OpcodeType::SHLX, InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"SHLX" });
    	instructionData.Set(OpcodeType::JNK,  InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::A16, 0, 0, 0, 0, size_t{ 3 }, L"JNK" });
    	instructionData.Set(OpcodeType::LHLX, InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::Simple, 0, 0, 0, 0, size_t{ 1 }, L"LHLX" });
    	instructionData.Set(OpcodeType::JK,   InstructionData8080 { 0x40, InstructionOperandIntel8080_8085::A16, 0, 0, 0, 0, size_t{ 3 }, L"JK" });
    }
}

void CPUAssemblerIntel8080_8085::Error(Location const & location, std::wstring const & message)
{
    errorHandler.Error(location, message);
    localErrors.push_back(AssemblerMessage(location, message));
}

Register8Type CPUAssemblerIntel8080_8085::ExpectOperandRegister8(ASTNode::Ptr node)
{
    if (node->NodeType() != ASTNodeType::Register8)
    {
        std::wostringstream stream;
        stream << L"Invalid operand node, expected Register8: " << int(node->NodeType());
        Error(node->Loc(), stream.str());
    }
    Register8Node<Register8Type> * reg = dynamic_cast<Register8Node<Register8Type> *>(node.get());
    if (reg == nullptr)
        Error(node->Loc(), L"Invalid operand node, expected Register8");
    return reg->RegisterType();
}

Register16Type CPUAssemblerIntel8080_8085::ExpectOperandRegister16(ASTNode::Ptr node)
{
    if (node->NodeType() != ASTNodeType::Register16)
    {
        std::wostringstream stream;
        stream << L"Invalid operand node, expected Register16: " << int(node->NodeType());
        Error(node->Loc(), stream.str());
    }
    Register16Node<Register16Type> * reg = dynamic_cast<Register16Node<Register16Type> *>(node.get());
    if (reg == nullptr)
        Error(node->Loc(), L"Invalid operand node, expected Register16");
    return reg->RegisterType();
}

uint8_t CPUAssemblerIntel8080_8085::ExpectOperandData8(ASTNode::Ptr node)
{
    if (node->NodeType() != ASTNodeType::Expression)
    {
        std::wostringstream stream;
        stream << L"Invalid operand node, expected expression: " << int(node->NodeType());
        Error(node->Loc(), stream.str());
    }
    ExpressionNode * data = dynamic_cast<ExpressionNode *>(node.get());
    if (data == nullptr)
        Error(node->Loc(), L"Invalid operand node, expected expression");
    ASTNode::Ptr subNode = node->FirstChild();
    switch (subNode->NodeType())
    {
    case ASTNodeType::Data8:
        {
            int64_t value = ConvertToValue(subNode->Value());
            if ((value < 0) || (value > std::numeric_limits<uint8_t>::max()))
            {
                std::wostringstream stream;
                stream << "Value out of range for 8 bit value: " << value;
                Error(subNode->Loc(), stream.str());
            }
            return uint8_t(value);
        }
    default:
        break;
    }
    return 0;
}

uint16_t CPUAssemblerIntel8080_8085::ExpectOperandData16(ASTNode::Ptr node)
{
    if (node->NodeType() != ASTNodeType::Expression)
    {
        std::wostringstream stream;
        stream << L"Invalid operand node, expected expression: " << int(node->NodeType());
        Error(node->Loc(), stream.str());
    }
    ExpressionNode * data = dynamic_cast<ExpressionNode *>(node.get());
    if (data == nullptr)
        Error(node->Loc(), L"Invalid operand node, expected expression");
    ASTNode::Ptr subNode = node->FirstChild();
    switch (subNode->NodeType())
    {
    case ASTNodeType::Data16:
        {
            int64_t value = ConvertToValue(subNode->Value());
            if ((value < 0) || (value > std::numeric_limits<uint16_t>::max()))
            {
                std::wostringstream stream;
                stream << L"Value out of range for 16 bit value: " << value;
                Error(subNode->Loc(), stream.str());
            }
            return uint16_t(value);
        }
    default:
        break;
    }
    return 0;
}

bool CPUAssemblerIntel8080_8085::Generate(ASTree const & ast, MachineCode & machineCode)
{
    machineCode.clear();
    ASTNode::Ptr node = ast.FirstNode();
    size_t line = 1;
    while (node != nullptr)
    {
        localErrors.clear();
        switch (node->NodeType())
        {
        case ASTNodeType::Opcode:
            {
                OpcodeNode<OpcodeType, AddressType> * opcode = dynamic_cast<OpcodeNode<OpcodeType, AddressType> *>(node.get());
                if (opcode == nullptr)
                    throw AssemblerException("Invalid opcode node");
                InstructionData8080 const & instructionInfo = instructionData.Get(opcode->Type());
                std::vector<uint8_t> instructionCode(instructionInfo.instructionSize);
                instructionCode[0] = instructionInfo.opcodeByte;
                InstructionOperandIntel8080_8085 operandType = instructionInfo.operandType;
                switch (operandType)
                {
                case InstructionOperandIntel8080_8085::None:
                    {
                        instructionCode[0] = instructionInfo.opcodeByte;
                    }
                    break;

                case InstructionOperandIntel8080_8085::RegM8_D3_RegM8_S0:
                    {
                        ASTNode::Ptr operandNode = node->FirstChild();
                        Register8Type ddd = ExpectOperandRegister8(operandNode);

                        operandNode = operandNode->Next();
                        Register8Type sss = ExpectOperandRegister8(operandNode);
                        instructionCode[0] = instructionInfo.opcodeByte | ((int(ddd) & 0x07) << 3) | ((int(sss) & 0x07) << 0);
                    }
                    break;

                case InstructionOperandIntel8080_8085::RegM8_D3_I8:
                    {
                        ASTNode::Ptr operandNode = node->FirstChild();
                        Register8Type ddd = ExpectOperandRegister8(operandNode);

                        operandNode = operandNode->Next();
                        uint8_t data = ExpectOperandData8(operandNode);
                        instructionCode[0] = instructionInfo.opcodeByte | ((int(ddd) & 0x07) << 3);
                        instructionCode[1] = data;
                    }
                    break;

                case InstructionOperandIntel8080_8085::Reg16_D4_I16:
                    {
                        ASTNode::Ptr operandNode = node->FirstChild();
                        Register16Type rp = ExpectOperandRegister16(operandNode);

                        operandNode = operandNode->Next();
                        uint16_t data = ExpectOperandData16(operandNode);
                        instructionCode[0] = instructionInfo.opcodeByte | ((int(rp) & 0x03) << 4);
                        instructionCode[1] = (data >> 0) & 0xFF;
                        instructionCode[2] = (data >> 8) & 0xFF;
                    }
                    break;

                case InstructionOperandIntel8080_8085::Reg16_D4_BDHPSW:
                    {
                        ASTNode::Ptr operandNode = node->FirstChild();
                        Register16Type rp = ExpectOperandRegister16(operandNode);
                        rp = (rp == Register16Type::PSW) ? Register16Type::SP : rp;

                        instructionCode[0] = instructionInfo.opcodeByte | ((int(rp) & 0x03) << 4);
                    }
                    break;

                case InstructionOperandIntel8080_8085::Reg16_D4_BD:
                    {
                        ASTNode::Ptr operandNode = node->FirstChild();
                        Register16Type rp = ExpectOperandRegister16(operandNode);

                        instructionCode[0] = instructionInfo.opcodeByte | ((int(rp) & 0x01) << 4);
                    }
                    break;

                case InstructionOperandIntel8080_8085::I16:
                    {
                        ASTNode::Ptr operandNode = node->FirstChild();
                        uint16_t data = ExpectOperandData16(operandNode);
                        instructionCode[0] = instructionInfo.opcodeByte;
                        instructionCode[1] = (data >> 0) & 0xFF;
                        instructionCode[2] = (data >> 8) & 0xFF;
                    }
                    break;

                default:
                    throw AssemblerException("Invalid operand code");
                }
                
                opcode->SetCode(instructionCode);
                machineCode.insert(machineCode.end(), instructionCode.begin(), instructionCode.end());
            }
            break;
        }
        Print(node, line);
        for (auto error : localErrors)
            reportStream << std::endl << L"--> Error: " << error.Loc() << " - " << error.Message();
        errorHandler.NoError();
        node = node->Next();
    }
    reportStream << std::endl;
    return true;
}


static size_t BytesPerInstructionMax = 3;
static std::wstring Spacer(size_t numBytes)
{
    std::wstring result;
    result += std::wstring((BytesPerInstructionMax - numBytes) * 3, L' ');
    return result;
}
static std::wstring Spacer()
{
    return std::wstring(5, L' ') + Spacer(0);
}

void CPUAssemblerIntel8080_8085::Print(ASTree const & ast)
{
    ASTNode::Ptr node = ast.FirstNode();
    size_t line = 1;
    while (node != nullptr)
    {
        Print(node, line);
    }
    reportStream << std::endl;
}

void CPUAssemblerIntel8080_8085::Print(ASTNode::Ptr node, size_t & line)
{
    StatementNode * statementNode = dynamic_cast<StatementNode *>(node.get());
    std::wstring label;
    if ((statementNode != nullptr) && (statementNode->Label() != nullptr))
    {
        label = statementNode->Label()->Value() + L":";
    }
    while (line < node->Loc().GetLine())
    {
        reportStream << std::endl;
        ++line;
    }

    switch (node->NodeType())
    {
    case ASTNodeType::CPU:
        reportStream << Spacer() << L"CPU " << node->Value();
        break;
    case ASTNodeType::Comment:
        reportStream << L";" << node->Value();
        break;
    case ASTNodeType::ORG:
        reportStream << Spacer() << label << L"ORG " << node->Value();
        break;
    case ASTNodeType::END:
        reportStream << Spacer() << label << L"END";
        break;
    case ASTNodeType::Opcode:
        {
            OpcodeNode<OpcodeType, AddressType> * opcode = dynamic_cast<OpcodeNode<OpcodeType, AddressType> *>(node.get());
            AddressType address = opcode->Address();
            reportStream << std::hex << std::uppercase << std::setw(4) << std::setfill(L'0') << address << L" " << std::dec;
            OpcodeType opcodeType = opcode->Type();
            InstructionData8080 instructionInfo = instructionData.Get(opcodeType);
            MachineCode const & machineCode = opcode->Code();
            for (size_t i = 0; i < machineCode.size(); ++i)
            {
                reportStream << std::hex << std::uppercase << std::setw(2) << int(machineCode[i]) << L" " << std::dec;
            }
            reportStream << Spacer(machineCode.size()) << label << node->Value();
            ASTNode::Ptr subNode = node->FirstChild();
            bool firstNode = true;
            while (subNode != nullptr)
            {
                if (firstNode)
                {
                    reportStream << L" ";
                    firstNode = false;
                }
                else
                    reportStream << L",";

                switch (subNode->NodeType())
                {
                case ASTNodeType::Expression:
                    {
                        ASTNode::Ptr subsubNode = subNode->FirstChild();
                        bool firstNode = true;
                        while (subsubNode != nullptr)
                        {
                            if (firstNode)
                            {
                                firstNode = false;
                            }
                            else
                                reportStream << L",";

                            reportStream << subsubNode->Value();
                            subsubNode = subsubNode->Next();
                        }
                    }
                    break;
                default:
                    reportStream << subNode->Value();
                }
                subNode = subNode->Next();
            }
        }
        break;
    default:
        reportStream << node->Value();
    }
}

} // namespace Assembler

