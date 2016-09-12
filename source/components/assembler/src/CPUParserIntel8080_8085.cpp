#include "CPUParserIntel8080_8085.h"

#include "core/Util.h"

namespace Assembler
{

CPUParserIntel8080_8085::CPUParserIntel8080_8085(CPUType cpuType, Scanner & scanner, ErrorHandler & errorHandler)
    : CPUParser(cpuType, scanner, errorHandler)
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

    startStates.Set(OpcodeType::MOV, OperandState::R8_R8);
    startStates.Set(OpcodeType::MVI, OperandState::R8_D8);
    startStates.Set(OpcodeType::LXI, OperandState::R16_BDH_SP_D16);
    startStates.Set(OpcodeType::STAX, OperandState::R16_BD);
    startStates.Set(OpcodeType::LDAX, OperandState::R16_BD);
    startStates.Set(OpcodeType::STA, OperandState::A16);
    startStates.Set(OpcodeType::LDA, OperandState::A16);
    startStates.Set(OpcodeType::SHLD, OperandState::A16);
    startStates.Set(OpcodeType::LHLD, OperandState::A16);
    startStates.Set(OpcodeType::XCHG, OperandState::Simple);
    startStates.Set(OpcodeType::PUSH, OperandState::R16_BDH_PSW);
    startStates.Set(OpcodeType::POP, OperandState::R16_BDH_PSW);
    startStates.Set(OpcodeType::XTHL, OperandState::Simple);
    startStates.Set(OpcodeType::SPHL, OperandState::Simple);
    startStates.Set(OpcodeType::INX, OperandState::R16_BDH_SP);
    startStates.Set(OpcodeType::DCX, OperandState::R16_BDH_SP);
    startStates.Set(OpcodeType::CALL, OperandState::A16);
    startStates.Set(OpcodeType::CC, OperandState::A16);
    startStates.Set(OpcodeType::CNC, OperandState::A16);
    startStates.Set(OpcodeType::CZ, OperandState::A16);
    startStates.Set(OpcodeType::CNZ, OperandState::A16);
    startStates.Set(OpcodeType::CP, OperandState::A16);
    startStates.Set(OpcodeType::CM, OperandState::A16);
    startStates.Set(OpcodeType::CPE, OperandState::A16);
    startStates.Set(OpcodeType::CPO, OperandState::A16);
    startStates.Set(OpcodeType::PCHL, OperandState::Simple);
    startStates.Set(OpcodeType::JMP, OperandState::A16);
    startStates.Set(OpcodeType::JC, OperandState::A16);
    startStates.Set(OpcodeType::JNC, OperandState::A16);
    startStates.Set(OpcodeType::JZ, OperandState::A16);
    startStates.Set(OpcodeType::JNZ, OperandState::A16);
    startStates.Set(OpcodeType::JP, OperandState::A16);
    startStates.Set(OpcodeType::JM, OperandState::A16);
    startStates.Set(OpcodeType::JPE, OperandState::A16);
    startStates.Set(OpcodeType::JPO, OperandState::A16);
    startStates.Set(OpcodeType::RET, OperandState::Simple);
    startStates.Set(OpcodeType::RC, OperandState::Simple);
    startStates.Set(OpcodeType::RNC, OperandState::Simple);
    startStates.Set(OpcodeType::RZ, OperandState::Simple);
    startStates.Set(OpcodeType::RNZ, OperandState::Simple);
    startStates.Set(OpcodeType::RP, OperandState::Simple);
    startStates.Set(OpcodeType::RM, OperandState::Simple);
    startStates.Set(OpcodeType::RPE, OperandState::Simple);
    startStates.Set(OpcodeType::RPO, OperandState::Simple);
    startStates.Set(OpcodeType::RST, OperandState::N);
    startStates.Set(OpcodeType::INR, OperandState::R8);
    startStates.Set(OpcodeType::DCR, OperandState::R8);
    startStates.Set(OpcodeType::ADD, OperandState::R8);
    startStates.Set(OpcodeType::ADC, OperandState::R8);
    startStates.Set(OpcodeType::SUB, OperandState::R8);
    startStates.Set(OpcodeType::SBB, OperandState::R8);
    startStates.Set(OpcodeType::DAD, OperandState::R16_BDH_SP);
    startStates.Set(OpcodeType::ANA, OperandState::R8);
    startStates.Set(OpcodeType::ORA, OperandState::R8);
    startStates.Set(OpcodeType::XRA, OperandState::R8);
    startStates.Set(OpcodeType::CMP, OperandState::R8);
    startStates.Set(OpcodeType::ADI, OperandState::D8);
    startStates.Set(OpcodeType::ACI, OperandState::D8);
    startStates.Set(OpcodeType::SUI, OperandState::D8);
    startStates.Set(OpcodeType::SBI, OperandState::D8);
    startStates.Set(OpcodeType::ANI, OperandState::D8);
    startStates.Set(OpcodeType::ORI, OperandState::D8);
    startStates.Set(OpcodeType::XRI, OperandState::D8);
    startStates.Set(OpcodeType::CPI, OperandState::D8);
    startStates.Set(OpcodeType::RLC, OperandState::Simple);
    startStates.Set(OpcodeType::RRC, OperandState::Simple);
    startStates.Set(OpcodeType::RAL, OperandState::Simple);
    startStates.Set(OpcodeType::RAR, OperandState::Simple);
    startStates.Set(OpcodeType::CMA, OperandState::Simple);
    startStates.Set(OpcodeType::STC, OperandState::Simple);
    startStates.Set(OpcodeType::CMC, OperandState::Simple);
    startStates.Set(OpcodeType::DAA, OperandState::Simple);
    startStates.Set(OpcodeType::INP, OperandState::P8);
    startStates.Set(OpcodeType::OUTP, OperandState::P8);
    startStates.Set(OpcodeType::EI, OperandState::Simple);
    startStates.Set(OpcodeType::DI, OperandState::Simple);
    startStates.Set(OpcodeType::NOP, OperandState::Simple);
    startStates.Set(OpcodeType::HLT, OperandState::Simple);

    if (cpuType == CPUType::Intel8085)
    {
    	startStates.Set(OpcodeType::DSUB, OperandState::Simple);
    	startStates.Set(OpcodeType::ARHL, OperandState::Simple);
    	startStates.Set(OpcodeType::RDEL, OperandState::Simple);
    	startStates.Set(OpcodeType::RIM, OperandState::Simple);
    	startStates.Set(OpcodeType::LDHI, OperandState::D8);
    	startStates.Set(OpcodeType::SIM, OperandState::Simple);
    	startStates.Set(OpcodeType::LDSI, OperandState::D8);
    	startStates.Set(OpcodeType::RSTV, OperandState::Simple);
    	startStates.Set(OpcodeType::SHLX, OperandState::Simple);
    	startStates.Set(OpcodeType::JNK, OperandState::A16);
    	startStates.Set(OpcodeType::LHLX, OperandState::Simple);
    	startStates.Set(OpcodeType::JK, OperandState::A16);
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
    auto opcodeNode = Nodes::CreateOpcode(opcode, lastToken.value, label, lastToken.location);
    ast.AddNode(opcodeNode);
    OperandState state = startStates.Get(opcode);
    if (state == OperandState::Invalid)
    {
        std::wostringstream stream;
        stream << L"No state defined for opcode: " << lastToken.value;
        SemanticError(lastToken.location, stream.str());
    }
    else
        HandleOperands(opcodeNode, state);
}

void CPUParserIntel8080_8085::HandleOperands(OpcodeNode<OpcodeType>::Ptr opcode, OperandState state)
{
    switch (state)
    {
    case OperandState::Simple:
        {
            Expect(TokenType::EOL);
        }
        break;
    case OperandState::R8:
        {
            Expect(TokenType::Identifier);
            Register8Type reg = registers8.Get(lastToken.value, Register8Type::Invalid);
            if (reg != Register8Type::Invalid)
                opcode->AddChild(CreateRegisterNode(reg, lastToken.value, lastToken.location));
            else
            {
                std::wostringstream stream;
                stream << L"Invalid register: " << lastToken.value;
                SemanticError(lastToken.location, stream.str());
            }
            Expect(TokenType::EOL);
        }
        break;
    case OperandState::R8_R8:
        {
            Expect(TokenType::Identifier);
            Register8Type registerDst = registers8.Get(lastToken.value, Register8Type::Invalid);
            if (registerDst != Register8Type::Invalid)
                opcode->AddChild(CreateRegisterNode(registerDst, lastToken.value, lastToken.location));
            else
            {
                std::wostringstream stream;
                stream << L"Invalid destination register: " << lastToken.value;
                SemanticError(lastToken.location, stream.str());
            }
            Expect(TokenType::Comma);
            Expect(TokenType::Identifier);
            Register8Type registerSrc = registers8.Get(lastToken.value, Register8Type::Invalid);
            if (registerSrc != Register8Type::Invalid)
                opcode->AddChild(CreateRegisterNode(registerSrc, lastToken.value, lastToken.location));
            else
            {
                std::wostringstream stream;
                stream << L"Invalid source register: " << lastToken.value;
                SemanticError(lastToken.location, stream.str());
            }
            if ((registerDst == Register8Type::M) && (registerSrc == Register8Type::M))
            {
                SemanticError(opcode->Loc(), L"Incorrect opcode: MOV M,M");
            }
            Expect(TokenType::EOL);
        }
        break;
    case OperandState::R8_D8:
        {
            Expect(TokenType::Identifier);
            Register8Type registerDst = registers8.Get(lastToken.value, Register8Type::Invalid);
            if (registerDst != Register8Type::Invalid)
                opcode->AddChild(CreateRegisterNode(registerDst, lastToken.value, lastToken.location));
            else
            {
                std::wostringstream stream;
                stream << L"Invalid register: " << lastToken.value;
                SemanticError(lastToken.location, stream.str());
            }
            Expect(TokenType::Comma);
            ParseExpression8(opcode);
            Expect(TokenType::EOL);
        }
        break;
    case OperandState::R16_BD:
        {
            Expect(TokenType::Identifier);
            Register16Type registerType = registers16.Get(lastToken.value, Register16Type::Invalid);
            if (registerType != Register16Type::Invalid)
                opcode->AddChild(CreateRegisterNode(registerType, lastToken.value, lastToken.location));
            else
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
            Expect(TokenType::EOL);
        }
        break;
    case OperandState::R16_BDH_SP:
        {
            Expect(TokenType::Identifier);
            Register16Type registerType = registers16.Get(lastToken.value, Register16Type::Invalid);
            if (registerType != Register16Type::Invalid)
                opcode->AddChild(CreateRegisterNode(registerType, lastToken.value, lastToken.location));
            else
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
            Expect(TokenType::EOL);
        }
        break;
    case OperandState::R16_BDH_PSW:
        {
            Expect(TokenType::Identifier);
            Register16Type registerType = registers16.Get(lastToken.value, Register16Type::Invalid);
            if (registerType != Register16Type::Invalid)
                opcode->AddChild(CreateRegisterNode(registerType, lastToken.value, lastToken.location));
            else
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
            Expect(TokenType::EOL);
        }
        break;
    case OperandState::R16_BDH_SP_D16:
        {
            Expect(TokenType::Identifier);
            Register16Type registerType = registers16.Get(lastToken.value, Register16Type::Invalid);
            if (registerType != Register16Type::Invalid)
                opcode->AddChild(CreateRegisterNode(registerType, lastToken.value, lastToken.location));
            else
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
            Expect(TokenType::EOL);
        }
        break;
    case OperandState::D8:
        {
            ParseExpression8(opcode);
            Expect(TokenType::EOL);
        }
        break;
    case OperandState::P8:
        {
            ParseExpression8(opcode);
            Expect(TokenType::EOL);
        }
        break;
    case OperandState::A16:
        {
            ParseExpression16(opcode);
            Expect(TokenType::EOL);
        }
        break;
    case OperandState::D16:
        {
            ParseExpression16(opcode);
            Expect(TokenType::EOL);
        }
        break;
    case OperandState::N:
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
            Expect(TokenType::EOL);
        }
        break;
    default:
        SemanticError(L"Unknown operand mode");
        break;
    }

}

void CPUParserIntel8080_8085::ParseExpression8(OpcodeNode<OpcodeType>::Ptr opcode)
{
    ExpressionNode::Ptr expression = Nodes::CreateExpression(L"", lastToken.location);
    opcode->AddChild(expression);
    // TODO: Parse an actual expression
    Expect(TokenType::Number);
    expression->AddChild(Nodes::CreateData8(lastToken.value, lastToken.location));
}

void CPUParserIntel8080_8085::ParseExpression16(OpcodeNode<OpcodeType>::Ptr opcode)
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
        expression->AddChild(Nodes::CreateLocCounter(lastToken.value, lastToken.location));
    }
    else if (currentToken.kind == TokenType::Identifier)
    {
        Get();
        expression->AddChild(Nodes::CreateRef(lastToken.value, lastToken.location));
    }

}

} // namespace Assembler

