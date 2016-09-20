#include "CPUParserIntel8080_8085.h"

#include "ICPUAssembler.h"
#include "core/Util.h"
#include "Nodes.h"

namespace Assembler
{

CPUParserIntel8080_8085::CPUParserIntel8080_8085(CPUType cpuType, Scanner & scanner, ErrorHandler & errorHandler, std::wostream & reportStream)
    : CPUParser(cpuType, scanner, errorHandler, reportStream)
    , registers8()
    , registers16()
    , rstCodes()
{
    Init();
}

CPUParserIntel8080_8085::~CPUParserIntel8080_8085()
{
}

void CPUParserIntel8080_8085::Init()
{
	opcodes.Set(L"MOV", OpcodeType::MOV);
	opcodes.Set(L"MVI", OpcodeType::MVI);
	opcodes.Set(L"LXI", OpcodeType::LXI);
	opcodes.Set(L"STAX", OpcodeType::STAX);
	opcodes.Set(L"LDAX", OpcodeType::LDAX);
	opcodes.Set(L"STA", OpcodeType::STA);
	opcodes.Set(L"LDA", OpcodeType::LDA);
	opcodes.Set(L"SHLD", OpcodeType::SHLD);
	opcodes.Set(L"LHLD", OpcodeType::LHLD);
	opcodes.Set(L"XCHG", OpcodeType::XCHG);
	opcodes.Set(L"PUSH", OpcodeType::PUSH);
	opcodes.Set(L"POP", OpcodeType::POP);
	opcodes.Set(L"XTHL", OpcodeType::XTHL);
	opcodes.Set(L"SPHL", OpcodeType::SPHL);
	opcodes.Set(L"INX", OpcodeType::INX);
	opcodes.Set(L"DCX", OpcodeType::DCX);
	opcodes.Set(L"JMP", OpcodeType::JMP);
	opcodes.Set(L"JC", OpcodeType::JC);
	opcodes.Set(L"JNC", OpcodeType::JNC);
	opcodes.Set(L"JZ", OpcodeType::JZ);
	opcodes.Set(L"JNZ", OpcodeType::JNZ);
	opcodes.Set(L"JP", OpcodeType::JP);
	opcodes.Set(L"JM", OpcodeType::JM);
	opcodes.Set(L"JPE", OpcodeType::JPE);
	opcodes.Set(L"JPO", OpcodeType::JPO);
	opcodes.Set(L"PCHL", OpcodeType::PCHL);
	opcodes.Set(L"CALL", OpcodeType::CALL);
	opcodes.Set(L"CC", OpcodeType::CC);
	opcodes.Set(L"CNC", OpcodeType::CNC);
	opcodes.Set(L"CZ", OpcodeType::CZ);
	opcodes.Set(L"CNZ", OpcodeType::CNZ);
	opcodes.Set(L"CP", OpcodeType::CP);
	opcodes.Set(L"CM", OpcodeType::CM);
	opcodes.Set(L"CPE", OpcodeType::CPE);
	opcodes.Set(L"CPO", OpcodeType::CPO);
	opcodes.Set(L"RET", OpcodeType::RET);
	opcodes.Set(L"RC", OpcodeType::RC);
	opcodes.Set(L"RNC", OpcodeType::RNC);
	opcodes.Set(L"RZ", OpcodeType::RZ);
	opcodes.Set(L"RNZ", OpcodeType::RNZ);
	opcodes.Set(L"RP", OpcodeType::RP);
	opcodes.Set(L"RM", OpcodeType::RM);
	opcodes.Set(L"RPE", OpcodeType::RPE);
	opcodes.Set(L"RPO", OpcodeType::RPO);
	opcodes.Set(L"RST", OpcodeType::RST);
	opcodes.Set(L"INR", OpcodeType::INR);
	opcodes.Set(L"DCR", OpcodeType::DCR);
	opcodes.Set(L"ADD", OpcodeType::ADD);
	opcodes.Set(L"ADC", OpcodeType::ADC);
	opcodes.Set(L"SUB", OpcodeType::SUB);
	opcodes.Set(L"SBB", OpcodeType::SBB);
	opcodes.Set(L"DAD", OpcodeType::DAD);
	opcodes.Set(L"ANA", OpcodeType::ANA);
	opcodes.Set(L"ORA", OpcodeType::ORA);
	opcodes.Set(L"XRA", OpcodeType::XRA);
	opcodes.Set(L"CMP", OpcodeType::CMP);
	opcodes.Set(L"ADI", OpcodeType::ADI);
	opcodes.Set(L"ACI", OpcodeType::ACI);
	opcodes.Set(L"SUI", OpcodeType::SUI);
	opcodes.Set(L"SBI", OpcodeType::SBI);
	opcodes.Set(L"ANI", OpcodeType::ANI);
	opcodes.Set(L"ORI", OpcodeType::ORI);
	opcodes.Set(L"XRI", OpcodeType::XRI);
	opcodes.Set(L"CPI", OpcodeType::CPI);
	opcodes.Set(L"RLC", OpcodeType::RLC);
	opcodes.Set(L"RRC", OpcodeType::RRC);
	opcodes.Set(L"RAL", OpcodeType::RAL);
	opcodes.Set(L"RAR", OpcodeType::RAR);
	opcodes.Set(L"CMA", OpcodeType::CMA);
	opcodes.Set(L"STC", OpcodeType::STC);
	opcodes.Set(L"CMC", OpcodeType::CMC);
	opcodes.Set(L"DAA", OpcodeType::DAA);
	opcodes.Set(L"IN", OpcodeType::INP);
	opcodes.Set(L"OUT", OpcodeType::OUTP);
	opcodes.Set(L"NOP", OpcodeType::NOP);
	opcodes.Set(L"EI", OpcodeType::EI);
	opcodes.Set(L"DI", OpcodeType::DI);
	opcodes.Set(L"HLT", OpcodeType::HLT);

    if (cpuType == CPUType::Intel8085)
    {
    	opcodes.Set(L"DSUB", OpcodeType::DSUB);
    	opcodes.Set(L"ARHL", OpcodeType::ARHL);
    	opcodes.Set(L"RDEL", OpcodeType::RDEL);
    	opcodes.Set(L"RIM", OpcodeType::RIM);
    	opcodes.Set(L"LDHI", OpcodeType::LDHI);
    	opcodes.Set(L"SIM", OpcodeType::SIM);
    	opcodes.Set(L"LDSI", OpcodeType::LDSI);
    	opcodes.Set(L"RSTV", OpcodeType::RSTV);
    	opcodes.Set(L"SHLX", OpcodeType::SHLX);
    	opcodes.Set(L"JNK", OpcodeType::JNK);
    	opcodes.Set(L"LHLX", OpcodeType::LHLX);
    	opcodes.Set(L"JK", OpcodeType::JK);
    }

    registers8.Set(L"B", Register8Type::B);
    registers8.Set(L"C", Register8Type::C);
    registers8.Set(L"D", Register8Type::D);
    registers8.Set(L"E", Register8Type::E);
    registers8.Set(L"H", Register8Type::H);
    registers8.Set(L"L", Register8Type::L);
    registers8.Set(L"A", Register8Type::A);
    registers8.Set(L"M", Register8Type::M);

    registers16.Set(L"B", Register16Type::BC);
    registers16.Set(L"D", Register16Type::DE);
    registers16.Set(L"H", Register16Type::HL);
    registers16.Set(L"SP", Register16Type::SP);
    registers16.Set(L"PSW", Register16Type::PSW);

    rstCodes.Set(L"0", RSTCode::RST0);
    rstCodes.Set(L"1", RSTCode::RST1);
    rstCodes.Set(L"2", RSTCode::RST2);
    rstCodes.Set(L"3", RSTCode::RST3);
    rstCodes.Set(L"4", RSTCode::RST4);
    rstCodes.Set(L"5", RSTCode::RST5);
    rstCodes.Set(L"6", RSTCode::RST6);
    rstCodes.Set(L"7", RSTCode::RST7);

    instructionData.Set(OpcodeType::MOV,  InstructionMapping8080 { OperandType::R8_R8, size_t{ 1 } });
    instructionData.Set(OpcodeType::MVI,  InstructionMapping8080 { OperandType::R8_D8, size_t{ 2 } });
    instructionData.Set(OpcodeType::LXI,  InstructionMapping8080 { OperandType::R16_BDH_SP_D16, size_t{ 3 } });
    instructionData.Set(OpcodeType::STAX, InstructionMapping8080 { OperandType::R16_BD, size_t{ 1 } });
    instructionData.Set(OpcodeType::LDAX, InstructionMapping8080 { OperandType::R16_BD, size_t{ 1 } });
    instructionData.Set(OpcodeType::STA,  InstructionMapping8080 { OperandType::A16, size_t{ 3 } });
    instructionData.Set(OpcodeType::LDA,  InstructionMapping8080 { OperandType::A16, size_t{ 3 } });
    instructionData.Set(OpcodeType::SHLD, InstructionMapping8080 { OperandType::A16, size_t{ 3 } });
    instructionData.Set(OpcodeType::LHLD, InstructionMapping8080 { OperandType::A16, size_t{ 3 } });
    instructionData.Set(OpcodeType::XCHG, InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    instructionData.Set(OpcodeType::PUSH, InstructionMapping8080 { OperandType::R16_BDH_PSW, size_t{ 1 } });
    instructionData.Set(OpcodeType::POP,  InstructionMapping8080 { OperandType::R16_BDH_PSW, size_t{ 1 } });
    instructionData.Set(OpcodeType::XTHL, InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    instructionData.Set(OpcodeType::SPHL, InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    instructionData.Set(OpcodeType::INX,  InstructionMapping8080 { OperandType::R16_BDH_SP, size_t{ 1 } });
    instructionData.Set(OpcodeType::DCX,  InstructionMapping8080 { OperandType::R16_BDH_SP, size_t{ 1 } });
    instructionData.Set(OpcodeType::CALL, InstructionMapping8080 { OperandType::A16, size_t{ 3 } });
    instructionData.Set(OpcodeType::CC,   InstructionMapping8080 { OperandType::A16, size_t{ 3 } });
    instructionData.Set(OpcodeType::CNC,  InstructionMapping8080 { OperandType::A16, size_t{ 3 } });
    instructionData.Set(OpcodeType::CZ,   InstructionMapping8080 { OperandType::A16, size_t{ 3 } });
    instructionData.Set(OpcodeType::CNZ,  InstructionMapping8080 { OperandType::A16, size_t{ 3 } });
    instructionData.Set(OpcodeType::CP,   InstructionMapping8080 { OperandType::A16, size_t{ 3 } });
    instructionData.Set(OpcodeType::CM,   InstructionMapping8080 { OperandType::A16, size_t{ 3 } });
    instructionData.Set(OpcodeType::CPE,  InstructionMapping8080 { OperandType::A16, size_t{ 3 } });
    instructionData.Set(OpcodeType::CPO,  InstructionMapping8080 { OperandType::A16, size_t{ 3 } });
    instructionData.Set(OpcodeType::PCHL, InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    instructionData.Set(OpcodeType::JMP,  InstructionMapping8080 { OperandType::A16, size_t{ 3 } });
    instructionData.Set(OpcodeType::JC,   InstructionMapping8080 { OperandType::A16, size_t{ 3 } });
    instructionData.Set(OpcodeType::JNC,  InstructionMapping8080 { OperandType::A16, size_t{ 3 } });
    instructionData.Set(OpcodeType::JZ,   InstructionMapping8080 { OperandType::A16, size_t{ 3 } });
    instructionData.Set(OpcodeType::JNZ,  InstructionMapping8080 { OperandType::A16, size_t{ 3 } });
    instructionData.Set(OpcodeType::JP,   InstructionMapping8080 { OperandType::A16, size_t{ 3 } });
    instructionData.Set(OpcodeType::JM,   InstructionMapping8080 { OperandType::A16, size_t{ 3 } });
    instructionData.Set(OpcodeType::JPE,  InstructionMapping8080 { OperandType::A16, size_t{ 3 } });
    instructionData.Set(OpcodeType::JPO,  InstructionMapping8080 { OperandType::A16, size_t{ 3 } });
    instructionData.Set(OpcodeType::RET,  InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    instructionData.Set(OpcodeType::RC,   InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    instructionData.Set(OpcodeType::RNC,  InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    instructionData.Set(OpcodeType::RZ,   InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    instructionData.Set(OpcodeType::RNZ,  InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    instructionData.Set(OpcodeType::RP,   InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    instructionData.Set(OpcodeType::RM,   InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    instructionData.Set(OpcodeType::RPE,  InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    instructionData.Set(OpcodeType::RPO,  InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    instructionData.Set(OpcodeType::RST,  InstructionMapping8080 { OperandType::N, size_t{ 1 } });
    instructionData.Set(OpcodeType::INR,  InstructionMapping8080 { OperandType::R8, size_t{ 1 } });
    instructionData.Set(OpcodeType::DCR,  InstructionMapping8080 { OperandType::R8, size_t{ 1 } });
    instructionData.Set(OpcodeType::ADD,  InstructionMapping8080 { OperandType::R8, size_t{ 1 } });
    instructionData.Set(OpcodeType::ADC,  InstructionMapping8080 { OperandType::R8, size_t{ 1 } });
    instructionData.Set(OpcodeType::SUB,  InstructionMapping8080 { OperandType::R8, size_t{ 1 } });
    instructionData.Set(OpcodeType::SBB,  InstructionMapping8080 { OperandType::R8, size_t{ 1 } });
    instructionData.Set(OpcodeType::DAD,  InstructionMapping8080 { OperandType::R16_BDH_SP, size_t{ 1 } });
    instructionData.Set(OpcodeType::ANA,  InstructionMapping8080 { OperandType::R8, size_t{ 1 } });
    instructionData.Set(OpcodeType::ORA,  InstructionMapping8080 { OperandType::R8, size_t{ 1 } });
    instructionData.Set(OpcodeType::XRA,  InstructionMapping8080 { OperandType::R8, size_t{ 1 } });
    instructionData.Set(OpcodeType::CMP,  InstructionMapping8080 { OperandType::R8, size_t{ 1 } });
    instructionData.Set(OpcodeType::ADI,  InstructionMapping8080 { OperandType::D8, size_t{ 2 } });
    instructionData.Set(OpcodeType::ACI,  InstructionMapping8080 { OperandType::D8, size_t{ 2 } });
    instructionData.Set(OpcodeType::SUI,  InstructionMapping8080 { OperandType::D8, size_t{ 2 } });
    instructionData.Set(OpcodeType::SBI,  InstructionMapping8080 { OperandType::D8, size_t{ 2 } });
    instructionData.Set(OpcodeType::ANI,  InstructionMapping8080 { OperandType::D8, size_t{ 2 } });
    instructionData.Set(OpcodeType::ORI,  InstructionMapping8080 { OperandType::D8, size_t{ 2 } });
    instructionData.Set(OpcodeType::XRI,  InstructionMapping8080 { OperandType::D8, size_t{ 2 } });
    instructionData.Set(OpcodeType::CPI,  InstructionMapping8080 { OperandType::D8, size_t{ 2 } });
    instructionData.Set(OpcodeType::RLC,  InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    instructionData.Set(OpcodeType::RRC,  InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    instructionData.Set(OpcodeType::RAL,  InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    instructionData.Set(OpcodeType::RAR,  InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    instructionData.Set(OpcodeType::CMA,  InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    instructionData.Set(OpcodeType::STC,  InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    instructionData.Set(OpcodeType::CMC,  InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    instructionData.Set(OpcodeType::DAA,  InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    instructionData.Set(OpcodeType::INP,  InstructionMapping8080 { OperandType::P8, size_t{ 2 } });
    instructionData.Set(OpcodeType::OUTP, InstructionMapping8080 { OperandType::P8, size_t{ 2 } });
    instructionData.Set(OpcodeType::EI,   InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    instructionData.Set(OpcodeType::DI,   InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    instructionData.Set(OpcodeType::NOP,  InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    instructionData.Set(OpcodeType::HLT,  InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });

    if (cpuType == CPUType::Intel8085)
    {
    	instructionData.Set(OpcodeType::DSUB, InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    	instructionData.Set(OpcodeType::ARHL, InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    	instructionData.Set(OpcodeType::RDEL, InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    	instructionData.Set(OpcodeType::RIM,  InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    	instructionData.Set(OpcodeType::LDHI, InstructionMapping8080 { OperandType::D8, size_t{ 2 } });
    	instructionData.Set(OpcodeType::SIM,  InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    	instructionData.Set(OpcodeType::LDSI, InstructionMapping8080 { OperandType::D8, size_t{ 2 } });
    	instructionData.Set(OpcodeType::RSTV, InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    	instructionData.Set(OpcodeType::SHLX, InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    	instructionData.Set(OpcodeType::JNK,  InstructionMapping8080 { OperandType::A16, size_t{ 3 } });
    	instructionData.Set(OpcodeType::LHLX, InstructionMapping8080 { OperandType::Simple, size_t{ 1 } });
    	instructionData.Set(OpcodeType::JK,   InstructionMapping8080 { OperandType::A16, size_t{ 3 } });
    }
}

Register8Node<Register8Type>::Ptr CPUParserIntel8080_8085::CreateRegisterNode(Register8Type registerType, std::wstring const & value, Location const & location)
{
    return std::make_shared<Register8Node<Register8Type>>(registerType, value, location);
}

Register16Node<Register16Type>::Ptr CPUParserIntel8080_8085::CreateRegisterNode(Register16Type registerType, std::wstring const & value, Location const & location)
{
    return std::make_shared<Register16Node<Register16Type>>(registerType, value, location);
}

RSTNode::Ptr CPUParserIntel8080_8085::CreateRSTNode(RSTCode rstCode, std::wstring const & value, Location const & location)
{
    return std::make_shared<RSTNode>(rstCode, value, location);
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

void CPUParserIntel8080_8085::PrintWithErrors()
{
    ASTNode::Ptr node = ast.FirstNode();
    size_t line = 1;
    AddressType address{};
    AssemblerMessages::const_iterator messageIt = errorHandler.begin();
    
    while (node != nullptr)
    {
        PrintWithErrors(node, line, messageIt);
        node = node->Next();
    }
    reportStream << std::endl;
}

void CPUParserIntel8080_8085::PrintWithErrors(ASTNode::Ptr node, size_t & line, AssemblerMessages::const_iterator & it)
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
        while ((it != errorHandler.end()) && (it->Loc().GetLine() < line))
        {
            reportStream << L"--> Error: " << it->Loc() << L" - " << it->Message() << std::endl;
            ++it;
        }
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
            reportStream << std::hex << std::setw(4) << std::setfill(L'0') << address << L" " << std::dec;
            OpcodeType opcodeType = opcode->Type();
            MachineCode const & machineCode = opcode->Code();
            for (size_t i = 0; i < machineCode.size(); ++i)
            {
                reportStream << std::hex << std::setw(2) << int(machineCode[i]) << L" ";
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

void CPUParserIntel8080_8085::HandleOpcodeAndOperands(LabelNode::Ptr label)
{
    OpcodeType opcode = opcodes.Get(lastToken.value, OpcodeType::Invalid);
    if (opcode == OpcodeType::Invalid)
    {
        std::wostringstream stream;
        stream << L"Expected Opcode: " << lastToken.value;
        SemanticError(stream.str());
        return;
    }
    auto opcodeNode = Nodes::CreateOpcode(programCounter, opcode, lastToken.value, label, lastToken.location);
    ast.AddNode(opcodeNode);
    InstructionMapping8080 const & instructionInfo = instructionData.Get(opcode);
    OperandType operandType = instructionInfo.operandType;
    if (operandType == OperandType::Invalid)
    {
        std::wostringstream stream;
        stream << L"No state defined for opcode: " << lastToken.value;
        SemanticError(lastToken.location, stream.str());
    }
    else
    {
        HandleOperands(opcodeNode, operandType);
        programCounter += instructionInfo.instructionSize;
    }
}

void CPUParserIntel8080_8085::HandleOperands(OpcodeNode<OpcodeType, AddressType>::Ptr opcode, OperandType state)
{
    switch (state)
    {
    case OperandType::Simple:
        {
            HandleComment();
            Expect(TokenType::EOL);
        }
        break;
    case OperandType::R8:
        {
            Expect(TokenType::Identifier);
            Register8Type reg = registers8.Get(lastToken.value, Register8Type::Invalid);
            opcode->AddChild(CreateRegisterNode(reg, lastToken.value, lastToken.location));
            if (reg == Register8Type::Invalid)
            {
                std::wostringstream stream;
                stream << L"Invalid register: " << lastToken.value;
                SemanticError(lastToken.location, stream.str());
            }
            HandleComment();
            Expect(TokenType::EOL);
        }
        break;
    case OperandType::R8_R8:
        {
            Expect(TokenType::Identifier);
            Register8Type registerDst = registers8.Get(lastToken.value, Register8Type::Invalid);
            opcode->AddChild(CreateRegisterNode(registerDst, lastToken.value, lastToken.location));
            if (registerDst == Register8Type::Invalid)
            {
                std::wostringstream stream;
                stream << L"Invalid destination register: " << lastToken.value;
                SemanticError(lastToken.location, stream.str());
            }
            Expect(TokenType::Comma);
            Expect(TokenType::Identifier);
            Register8Type registerSrc = registers8.Get(lastToken.value, Register8Type::Invalid);
            opcode->AddChild(CreateRegisterNode(registerSrc, lastToken.value, lastToken.location));
            if (registerSrc == Register8Type::Invalid)
            {
                std::wostringstream stream;
                stream << L"Invalid source register: " << lastToken.value;
                SemanticError(lastToken.location, stream.str());
            }
            if ((registerDst == Register8Type::M) && (registerSrc == Register8Type::M))
            {
                SemanticError(opcode->Loc(), L"Incorrect opcode: MOV M,M");
            }
            HandleComment();
            Expect(TokenType::EOL);
        }
        break;
    case OperandType::R8_D8:
        {
            Expect(TokenType::Identifier);
            Register8Type registerDst = registers8.Get(lastToken.value, Register8Type::Invalid);
            opcode->AddChild(CreateRegisterNode(registerDst, lastToken.value, lastToken.location));
            if (registerDst == Register8Type::Invalid)
            {
                std::wostringstream stream;
                stream << L"Invalid register: " << lastToken.value;
                SemanticError(lastToken.location, stream.str());
            }
            Expect(TokenType::Comma);
            ParseExpression8(opcode);
            HandleComment();
            Expect(TokenType::EOL);
        }
        break;
    case OperandType::R16_BD:
        {
            Expect(TokenType::Identifier);
            Register16Type registerType = registers16.Get(lastToken.value, Register16Type::Invalid);
            opcode->AddChild(CreateRegisterNode(registerType, lastToken.value, lastToken.location));
            if (registerType == Register16Type::Invalid)
            {
                std::wostringstream stream;
                stream << L"Invalid register pair: " << lastToken.value;
                SemanticError(lastToken.location, stream.str());
            }
            if ((registerType != Register16Type::Invalid) && (registerType != Register16Type::BC) && (registerType != Register16Type::DE))
            {
                std::wostringstream stream;
                stream << L"Opcode " << opcode->Value() << L" only supports B or D register pair, " << lastToken.value << L" specified";
                SemanticError(opcode->Loc(), stream.str());
            }
            HandleComment();
            Expect(TokenType::EOL);
        }
        break;
    case OperandType::R16_BDH_SP:
        {
            Expect(TokenType::Identifier);
            Register16Type registerType = registers16.Get(lastToken.value, Register16Type::Invalid);
            opcode->AddChild(CreateRegisterNode(registerType, lastToken.value, lastToken.location));
            if (registerType == Register16Type::Invalid)
            {
                std::wostringstream stream;
                stream << L"Invalid register pair: " << lastToken.value;
                SemanticError(lastToken.location, stream.str());
            }
            if ((registerType != Register16Type::Invalid) && (registerType != Register16Type::BC) && (registerType != Register16Type::DE) && 
                (registerType != Register16Type::HL) && (registerType != Register16Type::SP))
            {
                std::wostringstream stream;
                stream << L"Opcode " << opcode->Value() << L" only supports B, D, H or SP register pair, " << lastToken.value << L" specified";
                SemanticError(opcode->Loc(), stream.str());
            }
            HandleComment();
            Expect(TokenType::EOL);
        }
        break;
    case OperandType::R16_BDH_PSW:
        {
            Expect(TokenType::Identifier);
            Register16Type registerType = registers16.Get(lastToken.value, Register16Type::Invalid);
            opcode->AddChild(CreateRegisterNode(registerType, lastToken.value, lastToken.location));
            if (registerType == Register16Type::Invalid)
            {
                std::wostringstream stream;
                stream << L"Invalid register pair: " << lastToken.value;
                SemanticError(lastToken.location, stream.str());
            }
            if ((registerType != Register16Type::Invalid) && (registerType != Register16Type::BC) && (registerType != Register16Type::DE) && 
                (registerType != Register16Type::HL) && (registerType != Register16Type::PSW))
            {
                std::wostringstream stream;
                stream << L"Opcode " << opcode->Value() << L" only supports B, D, H or PSW register pair, " << lastToken.value << L" specified";
                SemanticError(opcode->Loc(), stream.str());
            }
            HandleComment();
            Expect(TokenType::EOL);
        }
        break;
    case OperandType::R16_BDH_SP_D16:
        {
            Expect(TokenType::Identifier);
            Register16Type registerType = registers16.Get(lastToken.value, Register16Type::Invalid);
            opcode->AddChild(CreateRegisterNode(registerType, lastToken.value, lastToken.location));
            if (registerType == Register16Type::Invalid)
            {
                std::wostringstream stream;
                stream << L"Invalid register pair: " << lastToken.value;
                SemanticError(lastToken.location, stream.str());
            }
            if ((registerType != Register16Type::Invalid) && (registerType != Register16Type::BC) && (registerType != Register16Type::DE) && 
                (registerType != Register16Type::HL) && (registerType != Register16Type::SP))
            {
                std::wostringstream stream;
                stream << L"Opcode " << opcode->Value() << L" only supports B, D, H or SP register pair, " << lastToken.value << L" specified";
                SemanticError(opcode->Loc(), stream.str());
            }
            Expect(TokenType::Comma);
            ParseExpression16(opcode);
            HandleComment();
            Expect(TokenType::EOL);
        }
        break;
    case OperandType::D8:
        {
            ParseExpression8(opcode);
            HandleComment();
            Expect(TokenType::EOL);
        }
        break;
    case OperandType::P8:
        {
            ParseExpression8(opcode);
            HandleComment();
            Expect(TokenType::EOL);
        }
        break;
    case OperandType::A16:
        {
            ParseExpression16(opcode);
            HandleComment();
            Expect(TokenType::EOL);
        }
        break;
    case OperandType::D16:
        {
            ParseExpression16(opcode);
            HandleComment();
            Expect(TokenType::EOL);
        }
        break;
    case OperandType::N:
        {
            Expect(TokenType::Number);
            RSTCode reg = rstCodes.Get(lastToken.value, RSTCode::Invalid);
            if (reg != RSTCode::Invalid)
                opcode->AddChild(CreateRSTNode(reg, lastToken.value, lastToken.location));
            else
            {
                std::wostringstream stream;
                stream << L"Invalid RST code: " << lastToken.value;
                SemanticError(lastToken.location, stream.str());
            }
            HandleComment();
            Expect(TokenType::EOL);
        }
        break;
    default:
        SemanticError(L"Unknown operand mode");
        break;
    }

}

void CPUParserIntel8080_8085::ParseExpression8(OpcodeNode<OpcodeType, AddressType>::Ptr opcode)
{
    ExpressionNode::Ptr expression = Nodes::CreateExpression(L"", lastToken.location);
    opcode->AddChild(expression);
    // TODO: Parse an actual expression
    if (currentToken.kind == TokenType::Number)
    {
        Get();
        expression->AddChild(Nodes::CreateData8(lastToken.value, lastToken.location));
    }
    else if (currentToken.kind == TokenType::Identifier)
    {
        Get();
        expression->AddChild(Nodes::CreateRefData(lastToken.value, 0, lastToken.location));
    }
}

void CPUParserIntel8080_8085::ParseExpression16(OpcodeNode<OpcodeType, AddressType>::Ptr opcode)
{
    ExpressionNode::Ptr expression = Nodes::CreateExpression(L"", lastToken.location);
    opcode->AddChild(expression);
    // TODO: Parse an actual expression
    if (currentToken.kind == TokenType::Number)
    {
        Get();
        expression->AddChild(Nodes::CreateData16(lastToken.value, lastToken.location));
    }
    else if (currentToken.kind == TokenType::LocCounter)
    {
        Get();
        expression->AddChild(Nodes::CreateLocCounter(programCounter, lastToken.value, lastToken.location));
    }
    else if (currentToken.kind == TokenType::Identifier)
    {
        Get();
        if (HaveLabel(lastToken.value))
        {
            Label<AddressType> const & label = GetLabel(lastToken.value);
            if (label.locationDefined)
                expression->AddChild(Nodes::CreateRefAddress(lastToken.value, label.location, lastToken.location));
            else
                expression->AddChild(Nodes::CreateRefAddressUndefined<AddressType>(lastToken.value, lastToken.location));
        }
        else
        {
            AddLabel(Label<AddressType>(lastToken.value));
            expression->AddChild(Nodes::CreateRefAddressUndefined<AddressType>(lastToken.value, lastToken.location));
        }
    }
}

} // namespace Assembler

