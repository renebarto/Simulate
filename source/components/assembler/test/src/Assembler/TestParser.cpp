#include "unit-test-c++/UnitTestC++.h"

#include "Parser.h"
#include "AbstractSyntaxTree.h"
#include "CPUParserIntel8080_8085.h"

using namespace std;

namespace Assembler
{

namespace Test
{

class ParserTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void ParserTest::SetUp()
{
}

void ParserTest::TearDown()
{
}

void CheckLabel(ASTNode::Ptr node, std::wstring const & value)
{
    ASSERT_NOT_NULL(node);
    EXPECT_EQ(ASTNodeType::Label, node->NodeType());
    EXPECT_EQ(value, node->Value());
    EXPECT_NULL(node->FirstChild());
    EXPECT_NULL(node->Next());
}

void CheckOrg(ASTNode::Ptr node, std::wstring const & value, std::wstring const & labelValue = L"")
{
    ASSERT_NOT_NULL(node);
    EXPECT_EQ(ASTNodeType::ORG, node->NodeType());
    EXPECT_EQ(value, node->Value());
    StatementNode * statementNode = dynamic_cast<StatementNode *>(node.get());
    ASSERT_NOT_NULL(statementNode);
    auto label = statementNode->Label();
    if (labelValue.empty())
        ASSERT_NULL(label);
    else
        CheckLabel(label, labelValue);
}

void CheckOpcode(ASTNode::Ptr node, OpcodeType opcodeType, std::wstring const & opcode, std::wstring const & labelValue = L"")
{
    ASSERT_NOT_NULL(node);
    EXPECT_EQ(ASTNodeType::Opcode, node->NodeType());
    EXPECT_EQ(opcode, node->Value());
    OpcodeNode<OpcodeType> * opcodeNode = dynamic_cast<OpcodeNode<OpcodeType> *>(node.get());
    ASSERT_NOT_NULL(opcodeNode);
    EXPECT_EQ(opcodeType, opcodeNode->Type());
    auto label = opcodeNode->Label();
    if (labelValue.empty())
        ASSERT_NULL(label);
    else
        CheckLabel(label, labelValue);
}

void CheckRegister8(ASTNode::Ptr node, Register8Type type, std::wstring const & name)
{
    ASSERT_NOT_NULL(node);
    EXPECT_EQ(ASTNodeType::Register8, node->NodeType());
    EXPECT_EQ(name, node->Value());
    Register8Node<Register8Type> * registerNode = dynamic_cast<Register8Node<Register8Type> *>(node.get());
    ASSERT_NOT_NULL(registerNode);
    EXPECT_EQ(type, registerNode->RegisterType());
}

void CheckRegister16(ASTNode::Ptr node, Register16Type type, std::wstring const & name)
{
    ASSERT_NOT_NULL(node);
    EXPECT_EQ(ASTNodeType::Register16, node->NodeType());
    EXPECT_EQ(name, node->Value());
    Register16Node<Register16Type> * registerNode = dynamic_cast<Register16Node<Register16Type> *>(node.get());
    ASSERT_NOT_NULL(registerNode);
    EXPECT_EQ(type, registerNode->RegisterType());
}

void CheckRSTCode(ASTNode::Ptr node, RSTCode rstCode, std::wstring const & code)
{
    ASSERT_NOT_NULL(node);
    EXPECT_EQ(ASTNodeType::RSTCode, node->NodeType());
    EXPECT_EQ(code, node->Value());
    RSTNode * rstNode = dynamic_cast<RSTNode *>(node.get());
    ASSERT_NOT_NULL(rstNode);
    EXPECT_EQ(rstCode, rstNode->GetRSTCode());
}

void CheckData8(ASTNode::Ptr node, std::wstring const & valueStr, uint8_t value)
{
    ASSERT_NOT_NULL(node);
    EXPECT_EQ(ASTNodeType::Expression, node->NodeType());
    ASSERT_NOT_NULL(node->FirstChild());
    ASTNode::Ptr childNode = node->FirstChild();
    EXPECT_EQ(ASTNodeType::Data8, childNode->NodeType());
    EXPECT_EQ(valueStr, childNode->Value());
    Data8Node * dataNode = dynamic_cast<Data8Node *>(childNode.get());
    ASSERT_NOT_NULL(dataNode);
}

void CheckData16(ASTNode::Ptr node, std::wstring const & valueStr, uint16_t value)
{
    ASSERT_NOT_NULL(node);
    EXPECT_EQ(ASTNodeType::Expression, node->NodeType());
    ASSERT_NOT_NULL(node->FirstChild());
    ASTNode::Ptr childNode = node->FirstChild();
    EXPECT_EQ(ASTNodeType::Data16, childNode->NodeType());
    EXPECT_EQ(valueStr, childNode->Value());
    Data16Node * dataNode = dynamic_cast<Data16Node *>(childNode.get());
    ASSERT_NOT_NULL(dataNode);
}

void CheckOpcodeR8R8(ASTNode::Ptr node, std::wstring const & opcode, 
                     Register8Type dddType, std::wstring const & ddd, 
                     Register8Type sssType, std::wstring const & sss, std::wstring const & labelValue = L"")
{
    ASSERT_NOT_NULL(node);
    CheckOpcode(node, OpcodeType::MOV, opcode, labelValue);

    Register8Node<Register8Type>::Ptr dstReg = node->FirstChild();
    if (dddType != Register8Type::Invalid)
        CheckRegister8(dstReg, dddType, ddd);
    Register8Node<Register8Type>::Ptr srcReg = dstReg;
    if ((dstReg != nullptr) && (dddType != Register8Type::Invalid))
        srcReg = dstReg->Next();
    if (sssType != Register8Type::Invalid)
        CheckRegister8(srcReg, sssType, sss);
}

void CheckOpcodeR16(ASTNode::Ptr node, OpcodeType opcodeType, std::wstring const & opcode, 
                    Register16Type rpType, std::wstring const & rp, std::wstring const & labelValue = L"")
{
    ASSERT_NOT_NULL(node);
    CheckOpcode(node, opcodeType, opcode, labelValue);

    Register16Node<Register16Type>::Ptr reg = node->FirstChild();
    if (rpType != Register16Type::Invalid)
        CheckRegister16(reg, rpType, rp);
}

void CheckOpcodeR8(ASTNode::Ptr node, OpcodeType opcodeType, std::wstring const & opcode, 
                   Register8Type rType, std::wstring const & r, std::wstring const & labelValue = L"")
{
    ASSERT_NOT_NULL(node);
    CheckOpcode(node, opcodeType, opcode, labelValue);

    Register8Node<Register8Type>::Ptr reg = node->FirstChild();
    if (rType != Register8Type::Invalid)
        CheckRegister8(reg, rType, r);
}

void CheckOpcodeR8D8(ASTNode::Ptr node, OpcodeType opcodeType, std::wstring const & opcode, 
                     Register8Type rType, std::wstring const & r, std::wstring const & data8Str, uint8_t data8, std::wstring const & labelValue = L"")
{
    ASSERT_NOT_NULL(node);
    CheckOpcode(node, opcodeType, opcode, labelValue);

    Register8Node<Register8Type>::Ptr reg = node->FirstChild();
    if (rType != Register8Type::Invalid)
        CheckRegister8(reg, rType, r);
    Data8Node::Ptr data = reg;
    if ((reg != nullptr) && (rType != Register8Type::Invalid))
        data = reg->Next();
    if (!data8Str.empty())
        CheckData8(data, data8Str, data8);
}

void CheckOpcodeR16D16(ASTNode::Ptr node, OpcodeType opcodeType, std::wstring const & opcode, 
                       Register16Type rType, std::wstring const & r, std::wstring const & data16Str, uint16_t data16, std::wstring const & labelValue = L"")
{
    ASSERT_NOT_NULL(node);
    CheckOpcode(node, opcodeType, opcode, labelValue);

    Register16Node<Register16Type>::Ptr reg = node->FirstChild();
    if (rType != Register16Type::Invalid)
        CheckRegister16(reg, rType, r);
    Data16Node::Ptr data = reg;
    if ((reg != nullptr) && (rType != Register16Type::Invalid))
        data = reg->Next();
    if (!data16Str.empty())
        CheckData16(data, data16Str, data16);
}

void CheckOpcodeD8(ASTNode::Ptr node, OpcodeType opcodeType, std::wstring const & opcode, 
                   std::wstring const & data8Str, uint8_t data8, std::wstring const & labelValue = L"")
{
    ASSERT_NOT_NULL(node);
    CheckOpcode(node, opcodeType, opcode, labelValue);

    Data8Node::Ptr data = node->FirstChild();
    if (!data8Str.empty())
        CheckData8(data, data8Str, data8);
}

void CheckOpcodeD16(ASTNode::Ptr node, OpcodeType opcodeType, std::wstring const & opcode, 
                    std::wstring const & data16Str, uint16_t data16, std::wstring const & labelValue = L"")
{
    ASSERT_NOT_NULL(node);
    CheckOpcode(node, opcodeType, opcode, labelValue);

    Data16Node::Ptr data = node->FirstChild();
    if (!data16Str.empty())
        CheckData16(data, data16Str, data16);
}

void CheckOpcodeLocCounter(ASTNode::Ptr node, OpcodeType opcodeType, std::wstring const & opcode, 
                           std::wstring const & labelValue = L"")
{
    ASSERT_NOT_NULL(node);
    CheckOpcode(node, opcodeType, opcode, labelValue);

    ASTNode::Ptr childNode = node->FirstChild();
    ASSERT_NOT_NULL(childNode);
    ExpressionNode * expressionNode = dynamic_cast<ExpressionNode *>(childNode.get());
    ASTNode::Ptr child2Node = childNode->FirstChild();
    ASSERT_NOT_NULL(child2Node);
    LocCounterNode * locCounterNode = dynamic_cast<LocCounterNode *>(child2Node.get());
    ASSERT_NOT_NULL(locCounterNode);
}

void CheckOpcodeRef(ASTNode::Ptr node, OpcodeType opcodeType, std::wstring const & opcode, 
                    std::wstring const & ref = L"", std::wstring const & labelValue = L"")
{
    ASSERT_NOT_NULL(node);
    CheckOpcode(node, opcodeType, opcode, labelValue);

    ASTNode::Ptr childNode = node->FirstChild();
    ASSERT_NOT_NULL(childNode);
    ExpressionNode * expressionNode = dynamic_cast<ExpressionNode *>(childNode.get());
    ASTNode::Ptr child2Node = childNode->FirstChild();
    ASSERT_NOT_NULL(child2Node);
    RefNode * refNode = dynamic_cast<RefNode *>(child2Node.get());
    ASSERT_NOT_NULL(refNode);
    EXPECT_EQ(ref, refNode->Value());
}

void CheckOpcodeN(ASTNode::Ptr node, OpcodeType opcodeType, std::wstring const & opcode, 
                  RSTCode rstCode, std::wstring const & code, std::wstring const & labelValue = L"")
{
    ASSERT_NOT_NULL(node);
    CheckOpcode(node, opcodeType, opcode, labelValue);

    RSTNode::Ptr rstNode = node->FirstChild();
    if (rstCode != RSTCode::Invalid)
        CheckRSTCode(rstNode, rstCode, code);
}

TEST_FIXTURE(ParserTest, ConstructDefault)
{
    AssemblerMessages messages;
    std::istringstream inputStream;
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    EXPECT_EQ(L"", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
}

TEST_FIXTURE(ParserTest, ParseEmpty)
{
    AssemblerMessages messages;
    std::istringstream inputStream("");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);
    parser.Parse();

    EXPECT_EQ(L"", reportStream.str());
    EXPECT_EQ(L"Error: 1:1 - \"CPU\" expected\n", errorStream.str());
    EXPECT_EQ(size_t{ 2 }, messages.size());
    EXPECT_EQ(size_t{ 2 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
}

TEST_FIXTURE(ParserTest, ParseEnd)
{
    AssemblerMessages messages;
    std::istringstream inputStream("End");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);
    parser.Parse();

    EXPECT_EQ(L"End", reportStream.str());
    EXPECT_EQ(L"Error: 1:1 - \"CPU\" expected\n", errorStream.str());
    EXPECT_EQ(size_t{ 2 }, messages.size());
    EXPECT_EQ(size_t{ 2 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
}

TEST_FIXTURE(ParserTest, ParseSimple1)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "START:  ORG 0000; Set initial address\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"START:  ORG 0000; Set initial address\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();
    CheckOrg(node, L"0000", L"START");

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseMOV)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "                   ; Move register to register\n"
                                   "        MOV A,A    ; A->A\n"
                                   "        MOV A,B    ; B->A\n"
                                   "        MOV A,C    ; C->A\n"
                                   "        MOV A,D    ; D->A\n"
                                   "        MOV A,E    ; E->A\n"
                                   "        MOV A,H    ; H->A\n"
                                   "        MOV A,L    ; L->A\n"
                                   "        MOV B,A    ; A->B\n"
                                   "        MOV C,A    ; A->C\n"
                                   "        MOV D,A    ; A->D\n"
                                   "        MOV E,A    ; A->E\n"
                                   "        MOV H,A    ; A->H\n"
                                   "        MOV L,A    ; A->L\n"
                                   "                   ; Move memory to register\n"
                                   "        MOV A,M    ; M->A\n"
                                   "                   ; Move register to memory\n"
                                   "        MOV M,A    ; A->M\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"                   ; Move register to register\n"
              L"        MOV A,A    ; A->A\n"
              L"        MOV A,B    ; B->A\n"
              L"        MOV A,C    ; C->A\n"
              L"        MOV A,D    ; D->A\n"
              L"        MOV A,E    ; E->A\n"
              L"        MOV A,H    ; H->A\n"
              L"        MOV A,L    ; L->A\n"
              L"        MOV B,A    ; A->B\n"
              L"        MOV C,A    ; A->C\n"
              L"        MOV D,A    ; A->D\n"
              L"        MOV E,A    ; A->E\n"
              L"        MOV H,A    ; A->H\n"
              L"        MOV L,A    ; A->L\n"
              L"                   ; Move memory to register\n"
              L"        MOV A,M    ; M->A\n"
              L"                   ; Move register to memory\n"
              L"        MOV M,A    ; A->M\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeR8R8(node, L"MOV", Register8Type::A, L"A", Register8Type::A, L"A");
    node = node->Next();
    CheckOpcodeR8R8(node, L"MOV", Register8Type::A, L"A", Register8Type::B, L"B");
    node = node->Next();
    CheckOpcodeR8R8(node, L"MOV", Register8Type::A, L"A", Register8Type::C, L"C");
    node = node->Next();
    CheckOpcodeR8R8(node, L"MOV", Register8Type::A, L"A", Register8Type::D, L"D");
    node = node->Next();
    CheckOpcodeR8R8(node, L"MOV", Register8Type::A, L"A", Register8Type::E, L"E");
    node = node->Next();
    CheckOpcodeR8R8(node, L"MOV", Register8Type::A, L"A", Register8Type::H, L"H");
    node = node->Next();
    CheckOpcodeR8R8(node, L"MOV", Register8Type::A, L"A", Register8Type::L, L"L");
    node = node->Next();
    CheckOpcodeR8R8(node, L"MOV", Register8Type::B, L"B", Register8Type::A, L"A");
    node = node->Next();
    CheckOpcodeR8R8(node, L"MOV", Register8Type::C, L"C", Register8Type::A, L"A");
    node = node->Next();
    CheckOpcodeR8R8(node, L"MOV", Register8Type::D, L"D", Register8Type::A, L"A");
    node = node->Next();
    CheckOpcodeR8R8(node, L"MOV", Register8Type::E, L"E", Register8Type::A, L"A");
    node = node->Next();
    CheckOpcodeR8R8(node, L"MOV", Register8Type::H, L"H", Register8Type::A, L"A");
    node = node->Next();
    CheckOpcodeR8R8(node, L"MOV", Register8Type::L, L"L", Register8Type::A, L"A");
    node = node->Next();
    CheckOpcodeR8R8(node, L"MOV", Register8Type::A, L"A", Register8Type::M, L"M");
    node = node->Next();
    CheckOpcodeR8R8(node, L"MOV", Register8Type::M, L"M", Register8Type::A, L"A");

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseMOVInvalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        MOV M,M    ; Invalid -> Halt\n"
                                   "        MOV X,A    ; Invalid destination\n"
                                   "        MOV A,X    ; Invalid source\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        MOV M,M    ; Invalid -> Halt\n"
              L"        MOV X,A    ; Invalid destination\n"
              L"        MOV A,X    ; Invalid source\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"Error: 2:9 - Incorrect opcode: MOV M,M\nError: 3:13 - Invalid destination register: X\nError: 4:15 - Invalid source register: X\n", errorStream.str());
    EXPECT_EQ(size_t{ 3 }, messages.size());
    EXPECT_EQ(size_t{ 3 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeR8R8(node, L"MOV", Register8Type::M, L"M", Register8Type::M, L"M");
    node = node->Next();
    CheckOpcodeR8R8(node, L"MOV", Register8Type::Invalid, L"", Register8Type::A, L"A");
    node = node->Next();
    CheckOpcodeR8R8(node, L"MOV", Register8Type::A, L"A", Register8Type::Invalid, L"");

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseMVI)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        MVI B,00h  ; Move immediate register B\n"
                                   "        MVI C,01h  ; Move immediate register C\n"
                                   "        MVI D,02h  ; Move immediate register D\n"
                                   "        MVI E,03h  ; Move immediate register E\n"
                                   "        MVI H,04h  ; Move immediate register H\n"
                                   "        MVI L,05h  ; Move immediate register L\n"
                                   "        MVI A,06h  ; Move immediate register A\n"
                                   "        MVI M,07h  ; Move immediate register M\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        MVI B,00h  ; Move immediate register B\n"
              L"        MVI C,01h  ; Move immediate register C\n"
              L"        MVI D,02h  ; Move immediate register D\n"
              L"        MVI E,03h  ; Move immediate register E\n"
              L"        MVI H,04h  ; Move immediate register H\n"
              L"        MVI L,05h  ; Move immediate register L\n"
              L"        MVI A,06h  ; Move immediate register A\n"
              L"        MVI M,07h  ; Move immediate register M\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeR8D8(node, OpcodeType::MVI, L"MVI", Register8Type::B, L"B", L"00h", 0);
    node = node->Next();
    CheckOpcodeR8D8(node, OpcodeType::MVI, L"MVI", Register8Type::C, L"C", L"01h", 1);
    node = node->Next();
    CheckOpcodeR8D8(node, OpcodeType::MVI, L"MVI", Register8Type::D, L"D", L"02h", 2);
    node = node->Next();
    CheckOpcodeR8D8(node, OpcodeType::MVI, L"MVI", Register8Type::E, L"E", L"03h", 3);
    node = node->Next();
    CheckOpcodeR8D8(node, OpcodeType::MVI, L"MVI", Register8Type::H, L"H", L"04h", 4);
    node = node->Next();
    CheckOpcodeR8D8(node, OpcodeType::MVI, L"MVI", Register8Type::L, L"L", L"05h", 5);
    node = node->Next();
    CheckOpcodeR8D8(node, OpcodeType::MVI, L"MVI", Register8Type::A, L"A", L"06h", 6);
    node = node->Next();
    CheckOpcodeR8D8(node, OpcodeType::MVI, L"MVI", Register8Type::M, L"M", L"07h", 7);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseMVIInvalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        MVI X, 08h   ; Invalid register\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        MVI X, 08h   ; Invalid register\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"Error: 2:13 - Invalid register: X\n", errorStream.str());
    EXPECT_EQ(size_t{ 1 }, messages.size());
    EXPECT_EQ(size_t{ 1 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeR8D8(node, OpcodeType::MVI, L"MVI", Register8Type::Invalid, L"", L"08h", 8);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseLXI)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        LXI B,0100h  ; Load immediate register pair B&C\n"
                                   "        LXI D,0302h  ; Load immediate register pair D&E\n"
                                   "        LXI H,0504h  ; Load immediate register pair H&L\n"
                                   "        LXI SP, 0706h; Load immediate stack pointer\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        LXI B,0100h  ; Load immediate register pair B&C\n"
              L"        LXI D,0302h  ; Load immediate register pair D&E\n"
              L"        LXI H,0504h  ; Load immediate register pair H&L\n"
              L"        LXI SP, 0706h; Load immediate stack pointer\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeR16D16(node, OpcodeType::LXI, L"LXI", Register16Type::BC, L"B", L"0100h", 0x0100);
    node = node->Next();
    CheckOpcodeR16D16(node, OpcodeType::LXI, L"LXI", Register16Type::DE, L"D", L"0302h", 0x0302);
    node = node->Next();
    CheckOpcodeR16D16(node, OpcodeType::LXI, L"LXI", Register16Type::HL, L"H", L"0504h", 0x0504);
    node = node->Next();
    CheckOpcodeR16D16(node, OpcodeType::LXI, L"LXI", Register16Type::SP, L"SP", L"0706h", 0x0706);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseLXIInvalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        LXI PSW, 0101h ; Upsupported register pair\n"
                                   "        LXI X, 0101h   ; Invalid register pair\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        LXI PSW, 0101h ; Upsupported register pair\n"
              L"        LXI X, 0101h   ; Invalid register pair\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"Error: 2:9 - Opcode LXI only supports B, D, H or SP register pair, PSW specified\nError: 3:13 - Invalid register pair: X\n", errorStream.str());
    EXPECT_EQ(size_t{ 2 }, messages.size());
    EXPECT_EQ(size_t{ 2 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeR16D16(node, OpcodeType::LXI, L"LXI", Register16Type::PSW, L"PSW", L"0101h", 0x0101);
    node = node->Next();
    CheckOpcodeR16D16(node, OpcodeType::LXI, L"LXI", Register16Type::Invalid, L"", L"0101h", 0x0101);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseLDAX_STAX)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        STAX B     ; Store A direct\n"
                                   "        STAX D     ; Store A direct\n"
                                   "        LDAX B     ; Load A direct\n"
                                   "        LDAX D     ; Load A direct\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        STAX B     ; Store A direct\n"
              L"        STAX D     ; Store A direct\n"
              L"        LDAX B     ; Load A direct\n"
              L"        LDAX D     ; Load A direct\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeR16(node, OpcodeType::STAX, L"STAX", Register16Type::BC, L"B");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::STAX, L"STAX", Register16Type::DE, L"D");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::LDAX, L"LDAX", Register16Type::BC, L"B");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::LDAX, L"LDAX", Register16Type::DE, L"D");

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseLDAX_STAXInvalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        STAX H     ; Unsupported register pair\n"
                                   "        STAX SP    ; Unsupported register pair\n"
                                   "        LDAX H     ; Unsupported register pair\n"
                                   "        LDAX SP    ; Unsupported register pair\n"
                                   "        STAX X     ; Invalid register pair\n"
                                   "        LDAX X     ; Invalid register pair\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        STAX H     ; Unsupported register pair\n"
              L"        STAX SP    ; Unsupported register pair\n"
              L"        LDAX H     ; Unsupported register pair\n"
              L"        LDAX SP    ; Unsupported register pair\n"
              L"        STAX X     ; Invalid register pair\n"
              L"        LDAX X     ; Invalid register pair\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"Error: 2:9 - Opcode STAX only supports B or D register pair, H specified\nError: 3:9 - Opcode STAX only supports B or D register pair, SP specified\n"
              L"Error: 4:9 - Opcode LDAX only supports B or D register pair, H specified\nError: 5:9 - Opcode LDAX only supports B or D register pair, SP specified\n"
              L"Error: 6:14 - Invalid register pair: X\nError: 7:14 - Invalid register pair: X\n", errorStream.str());
    EXPECT_EQ(size_t{ 6 }, messages.size());
    EXPECT_EQ(size_t{ 6 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeR16(node, OpcodeType::STAX, L"STAX", Register16Type::HL, L"H");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::STAX, L"STAX", Register16Type::SP, L"SP");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::LDAX, L"LDAX", Register16Type::HL, L"H");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::LDAX, L"LDAX", Register16Type::SP, L"SP");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::STAX, L"STAX", Register16Type::Invalid, L"");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::LDAX, L"LDAX", Register16Type::Invalid, L"");

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseSTA_LDA)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        STA 0100h ; Store A direct\n"
                                   "        LDA 0302h ; Load A direct\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        STA 0100h ; Store A direct\n"
              L"        LDA 0302h ; Load A direct\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeD16(node, OpcodeType::STA, L"STA", L"0100h", 0x0100);
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::LDA, L"LDA", L"0302h", 0x0302);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseSHLD_LHLD)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        SHLD 0100h ; Store H&L direct\n"
                                   "        LHLD 0302h ; Load H&L direct\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        SHLD 0100h ; Store H&L direct\n"
              L"        LHLD 0302h ; Load H&L direct\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeD16(node, OpcodeType::SHLD, L"SHLD", L"0100h", 0x0100);
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::LHLD, L"LHLD", L"0302h", 0x0302);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseXCHG)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        XCHG    ; Exchange D & E, H & L registers\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        XCHG    ; Exchange D & E, H & L registers\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcode(node, OpcodeType::XCHG, L"XCHG");

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParsePUSH_POP)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        PUSH B     ; Push register pair B&C on stack\n"
                                   "        PUSH D     ; Push register pair D&E on stack\n"
                                   "        PUSH H     ; Push register pair H&L on stack\n"
                                   "        PUSH PSW   ; Push register pair A&F on stack\n"
                                   "        POP B      ; Pop register pair B&C off stack\n"
                                   "        POP D      ; Pop register pair D&E off stack\n"
                                   "        POP H      ; Pop register pair H&L off stack\n"
                                   "        POP PSW    ; Pop register pair A&F off stack\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        PUSH B     ; Push register pair B&C on stack\n"
              L"        PUSH D     ; Push register pair D&E on stack\n"
              L"        PUSH H     ; Push register pair H&L on stack\n"
              L"        PUSH PSW   ; Push register pair A&F on stack\n"
              L"        POP B      ; Pop register pair B&C off stack\n"
              L"        POP D      ; Pop register pair D&E off stack\n"
              L"        POP H      ; Pop register pair H&L off stack\n"
              L"        POP PSW    ; Pop register pair A&F off stack\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeR16(node, OpcodeType::PUSH, L"PUSH", Register16Type::BC, L"B");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::PUSH, L"PUSH", Register16Type::DE, L"D");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::PUSH, L"PUSH", Register16Type::HL, L"H");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::PUSH, L"PUSH", Register16Type::PSW, L"PSW");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::POP, L"POP", Register16Type::BC, L"B");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::POP, L"POP", Register16Type::DE, L"D");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::POP, L"POP", Register16Type::HL, L"H");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::POP, L"POP", Register16Type::PSW, L"PSW");

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParsePUSH_POPInvalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        PUSH SP    ; Unsupported register pair\n"
                                   "        POP SP     ; Unsupported register pair\n"
                                   "        PUSH X     ; Invalid register pair\n"
                                   "        POP X      ; Invalid register pair\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        PUSH SP    ; Unsupported register pair\n"
              L"        POP SP     ; Unsupported register pair\n"
              L"        PUSH X     ; Invalid register pair\n"
              L"        POP X      ; Invalid register pair\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"Error: 2:9 - Opcode PUSH only supports B, D, H or PSW register pair, SP specified\nError: 3:9 - Opcode POP only supports B, D, H or PSW register pair, SP specified\n"
              L"Error: 4:14 - Invalid register pair: X\nError: 5:13 - Invalid register pair: X\n", errorStream.str());
    EXPECT_EQ(size_t{ 4 }, messages.size());
    EXPECT_EQ(size_t{ 4 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeR16(node, OpcodeType::PUSH, L"PUSH", Register16Type::SP, L"SP");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::POP, L"POP", Register16Type::SP, L"SP");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::PUSH, L"PUSH", Register16Type::Invalid, L"");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::POP, L"POP", Register16Type::Invalid, L"");

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseXTHL_SPHL)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        XTHL    ; Exchange top of stack, H & L\n"
                                   "        SPHL    ; H & L to stack pointer\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        XTHL    ; Exchange top of stack, H & L\n"
              L"        SPHL    ; H & L to stack pointer\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcode(node, OpcodeType::XTHL, L"XTHL");
    node = node->Next();
    CheckOpcode(node, OpcodeType::SPHL, L"SPHL");

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseINX_DCX)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        INX B      ; Increment B&C registers\n"
                                   "        INX D      ; Increment D&E registers\n"
                                   "        INX H      ; Increment H&L registers\n"
                                   "        INX SP     ; Increment stack pointer\n"
                                   "        DCX B      ; Decrement B&C registers\n"
                                   "        DCX D      ; Decrement D&E registers\n"
                                   "        DCX H      ; Decrement H&L registers\n"
                                   "        DCX SP     ; Decrement stack pointer\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        INX B      ; Increment B&C registers\n"
              L"        INX D      ; Increment D&E registers\n"
              L"        INX H      ; Increment H&L registers\n"
              L"        INX SP     ; Increment stack pointer\n"
              L"        DCX B      ; Decrement B&C registers\n"
              L"        DCX D      ; Decrement D&E registers\n"
              L"        DCX H      ; Decrement H&L registers\n"
              L"        DCX SP     ; Decrement stack pointer\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeR16(node, OpcodeType::INX, L"INX", Register16Type::BC, L"B");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::INX, L"INX", Register16Type::DE, L"D");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::INX, L"INX", Register16Type::HL, L"H");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::INX, L"INX", Register16Type::SP, L"SP");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::DCX, L"DCX", Register16Type::BC, L"B");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::DCX, L"DCX", Register16Type::DE, L"D");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::DCX, L"DCX", Register16Type::HL, L"H");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::DCX, L"DCX", Register16Type::SP, L"SP");

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseINX_DCXInvalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        INX PSW    ; Unsupported register pair\n"
                                   "        DCX PSW    ; Unsupported register pair\n"
                                   "        INX X      ; Invalid register pair\n"
                                   "        DCX X      ; Invalid register pair\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        INX PSW    ; Unsupported register pair\n"
              L"        DCX PSW    ; Unsupported register pair\n"
              L"        INX X      ; Invalid register pair\n"
              L"        DCX X      ; Invalid register pair\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"Error: 2:9 - Opcode INX only supports B, D, H or SP register pair, PSW specified\nError: 3:9 - Opcode DCX only supports B, D, H or SP register pair, PSW specified\n"
              L"Error: 4:13 - Invalid register pair: X\nError: 5:13 - Invalid register pair: X\n", errorStream.str());
    EXPECT_EQ(size_t{ 4 }, messages.size());
    EXPECT_EQ(size_t{ 4 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeR16(node, OpcodeType::INX, L"INX", Register16Type::PSW, L"PSW");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::DCX, L"DCX", Register16Type::PSW, L"PSW");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::INX, L"INX", Register16Type::Invalid, L"");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::DCX, L"DCX", Register16Type::Invalid, L"");

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseJMP_JXX)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        JMP 0100h ; Jump unconditional\n"
                                   "        JC 0101h  ; Jump on carry\n"
                                   "        JNC 0102h ; Jump on no carry\n"
                                   "        JZ 0103h  ; Jump on zero\n"
                                   "        JNZ 0104h ; Jump on no zero\n"
                                   "        JP 0105h  ; Jump on positive\n"
                                   "        JM 0106h  ; Jump on minus\n"
                                   "        JPE 0107h ; Jump on parity even\n"
                                   "        JPO 0108h ; Jump on parity odd\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        JMP 0100h ; Jump unconditional\n"
              L"        JC 0101h  ; Jump on carry\n"
              L"        JNC 0102h ; Jump on no carry\n"
              L"        JZ 0103h  ; Jump on zero\n"
              L"        JNZ 0104h ; Jump on no zero\n"
              L"        JP 0105h  ; Jump on positive\n"
              L"        JM 0106h  ; Jump on minus\n"
              L"        JPE 0107h ; Jump on parity even\n"
              L"        JPO 0108h ; Jump on parity odd\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeD16(node, OpcodeType::JMP, L"JMP", L"0100h", 0x0100);
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::JC, L"JC", L"0101h", 0x0101);
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::JNC, L"JNC", L"0102h", 0x0102);
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::JZ, L"JZ", L"0103h", 0x0103);
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::JNZ, L"JNZ", L"0104h", 0x0104);
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::JP, L"JP", L"0105h", 0x0105);
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::JM, L"JM", L"0106h", 0x0106);
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::JPE, L"JPE", L"0107h", 0x0107);
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::JPO, L"JPO", L"0108h", 0x0108);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParsePCHL)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        PCHL    ; H & L to program counter\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        PCHL    ; H & L to program counter\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcode(node, OpcodeType::PCHL, L"PCHL");

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseCALL_CXX)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        CALL 0100h ; Call\n"
                                   "        CC 0101h   ; Call on carry\n"
                                   "        CNC 0102h  ; Call on no carry\n"
                                   "        CZ 0103h   ; Call on zero\n"
                                   "        CNZ 0104h  ; Call on no zero\n"
                                   "        CP 0105h   ; Call on positive\n"
                                   "        CM 0106h   ; Call on minus\n"
                                   "        CPE 0107h  ; Call on parity even\n"
                                   "        CPO 0108h  ; Call on parity odd\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        CALL 0100h ; Call\n"
              L"        CC 0101h   ; Call on carry\n"
              L"        CNC 0102h  ; Call on no carry\n"
              L"        CZ 0103h   ; Call on zero\n"
              L"        CNZ 0104h  ; Call on no zero\n"
              L"        CP 0105h   ; Call on positive\n"
              L"        CM 0106h   ; Call on minus\n"
              L"        CPE 0107h  ; Call on parity even\n"
              L"        CPO 0108h  ; Call on parity odd\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeD16(node, OpcodeType::CALL, L"CALL", L"0100h", 0x0100);
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::CC, L"CC", L"0101h", 0x0101);
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::CNC, L"CNC", L"0102h", 0x0102);
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::CZ, L"CZ", L"0103h", 0x0103);
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::CNZ, L"CNZ", L"0104h", 0x0104);
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::CP, L"CP", L"0105h", 0x0105);
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::CM, L"CM", L"0106h", 0x0106);
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::CPE, L"CPE", L"0107h", 0x0107);
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::CPO, L"CPO", L"0108h", 0x0108);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseRET_RXX)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        RET     ; Return\n"
                                   "        RC      ; Return on carry\n"
                                   "        RNC     ; Return on no carry\n"
                                   "        RZ      ; Return on zero\n"
                                   "        RNZ     ; Return on no zero\n"
                                   "        RP      ; Return on positive\n"
                                   "        RM      ; Return on minus\n"
                                   "        RPE     ; Return on parity even\n"
                                   "        RPO     ; Return on parity odd\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        RET     ; Return\n"
              L"        RC      ; Return on carry\n"
              L"        RNC     ; Return on no carry\n"
              L"        RZ      ; Return on zero\n"
              L"        RNZ     ; Return on no zero\n"
              L"        RP      ; Return on positive\n"
              L"        RM      ; Return on minus\n"
              L"        RPE     ; Return on parity even\n"
              L"        RPO     ; Return on parity odd\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcode(node, OpcodeType::RET, L"RET");
    node = node->Next();
    CheckOpcode(node, OpcodeType::RC, L"RC");
    node = node->Next();
    CheckOpcode(node, OpcodeType::RNC, L"RNC");
    node = node->Next();
    CheckOpcode(node, OpcodeType::RZ, L"RZ");
    node = node->Next();
    CheckOpcode(node, OpcodeType::RNZ, L"RNZ");
    node = node->Next();
    CheckOpcode(node, OpcodeType::RP, L"RP");
    node = node->Next();
    CheckOpcode(node, OpcodeType::RM, L"RM");
    node = node->Next();
    CheckOpcode(node, OpcodeType::RPE, L"RPE");
    node = node->Next();
    CheckOpcode(node, OpcodeType::RPO, L"RPO");

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseRST)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        RST 0      ; Restart\n"
                                   "        RST 1      ; Restart\n"
                                   "        RST 2      ; Restart\n"
                                   "        RST 3      ; Restart\n"
                                   "        RST 4      ; Restart\n"
                                   "        RST 5      ; Restart\n"
                                   "        RST 6      ; Restart\n"
                                   "        RST 7      ; Restart\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        RST 0      ; Restart\n"
              L"        RST 1      ; Restart\n"
              L"        RST 2      ; Restart\n"
              L"        RST 3      ; Restart\n"
              L"        RST 4      ; Restart\n"
              L"        RST 5      ; Restart\n"
              L"        RST 6      ; Restart\n"
              L"        RST 7      ; Restart\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeN(node, OpcodeType::RST, L"RST", RSTCode::RST0, L"0");
    node = node->Next();
    CheckOpcodeN(node, OpcodeType::RST, L"RST", RSTCode::RST1, L"1");
    node = node->Next();
    CheckOpcodeN(node, OpcodeType::RST, L"RST", RSTCode::RST2, L"2");
    node = node->Next();
    CheckOpcodeN(node, OpcodeType::RST, L"RST", RSTCode::RST3, L"3");
    node = node->Next();
    CheckOpcodeN(node, OpcodeType::RST, L"RST", RSTCode::RST4, L"4");
    node = node->Next();
    CheckOpcodeN(node, OpcodeType::RST, L"RST", RSTCode::RST5, L"5");
    node = node->Next();
    CheckOpcodeN(node, OpcodeType::RST, L"RST", RSTCode::RST6, L"6");
    node = node->Next();
    CheckOpcodeN(node, OpcodeType::RST, L"RST", RSTCode::RST7, L"7");

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseRSTInvalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        RST 8      ; Invalid RST code\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        RST 8      ; Invalid RST code\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"Error: 2:13 - Invalid RST code: 8\n", errorStream.str());
    EXPECT_EQ(size_t{ 1 }, messages.size());
    EXPECT_EQ(size_t{ 1 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeN(node, OpcodeType::RST, L"RST", RSTCode::Invalid, L"");

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseINR_DCR)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        INR B      ; Increment B register\n"
                                   "        INR C      ; Increment C register\n"
                                   "        INR D      ; Increment D register\n"
                                   "        INR E      ; Increment E register\n"
                                   "        INR H      ; Increment H register\n"
                                   "        INR L      ; Increment L register\n"
                                   "        INR A      ; Increment A register\n"
                                   "        INR M      ; Increment memory\n"
                                   "        DCR B      ; Decrement B register\n"
                                   "        DCR C      ; Decrement C register\n"
                                   "        DCR D      ; Decrement D register\n"
                                   "        DCR E      ; Decrement E register\n"
                                   "        DCR H      ; Decrement H register\n"
                                   "        DCR L      ; Decrement L register\n"
                                   "        DCR A      ; Decrement A register\n"
                                   "        DCR M      ; Decrement memory\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        INR B      ; Increment B register\n"
              L"        INR C      ; Increment C register\n"
              L"        INR D      ; Increment D register\n"
              L"        INR E      ; Increment E register\n"
              L"        INR H      ; Increment H register\n"
              L"        INR L      ; Increment L register\n"
              L"        INR A      ; Increment A register\n"
              L"        INR M      ; Increment memory\n"
              L"        DCR B      ; Decrement B register\n"
              L"        DCR C      ; Decrement C register\n"
              L"        DCR D      ; Decrement D register\n"
              L"        DCR E      ; Decrement E register\n"
              L"        DCR H      ; Decrement H register\n"
              L"        DCR L      ; Decrement L register\n"
              L"        DCR A      ; Decrement A register\n"
              L"        DCR M      ; Decrement memory\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeR8(node, OpcodeType::INR, L"INR", Register8Type::B, L"B");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::INR, L"INR", Register8Type::C, L"C");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::INR, L"INR", Register8Type::D, L"D");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::INR, L"INR", Register8Type::E, L"E");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::INR, L"INR", Register8Type::H, L"H");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::INR, L"INR", Register8Type::L, L"L");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::INR, L"INR", Register8Type::A, L"A");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::INR, L"INR", Register8Type::M, L"M");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::DCR, L"DCR", Register8Type::B, L"B");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::DCR, L"DCR", Register8Type::C, L"C");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::DCR, L"DCR", Register8Type::D, L"D");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::DCR, L"DCR", Register8Type::E, L"E");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::DCR, L"DCR", Register8Type::H, L"H");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::DCR, L"DCR", Register8Type::L, L"L");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::DCR, L"DCR", Register8Type::A, L"A");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::DCR, L"DCR", Register8Type::M, L"M");

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseINR_DCRInvalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        INR X      ; Invalid register\n"
                                   "        DCR X      ; Invalid register\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        INR X      ; Invalid register\n"
              L"        DCR X      ; Invalid register\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"Error: 2:13 - Invalid register: X\nError: 3:13 - Invalid register: X\n", errorStream.str());
    EXPECT_EQ(size_t{ 2 }, messages.size());
    EXPECT_EQ(size_t{ 2 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeR8(node, OpcodeType::INR, L"INR", Register8Type::Invalid, L"");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::DCR, L"DCR", Register8Type::Invalid, L"");

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseADD_ADC_SUB_SBB_ANA_ORA_XRA_CMP)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        ADD B      ; Add B register to A\n"
                                   "        ADD C      ; Add C register to A\n"
                                   "        ADD D      ; Add D register to A\n"
                                   "        ADD E      ; Add E register to A\n"
                                   "        ADD H      ; Add H register to A\n"
                                   "        ADD L      ; Add L register to A\n"
                                   "        ADD A      ; Add A register to A\n"
                                   "        ADD M      ; Add memory to A\n"
                                   "        ADC B      ; Add B register with carry to A\n"
                                   "        ADC C      ; Add C register with carry to A\n"
                                   "        ADC D      ; Add D register with carry to A\n"
                                   "        ADC E      ; Add E register with carry to A\n"
                                   "        ADC H      ; Add H register with carry to A\n"
                                   "        ADC L      ; Add L register with carry to A\n"
                                   "        ADC A      ; Add A register with carry to A\n"
                                   "        ADC M      ; Add memory with carry to A\n"
                                   "        SUB B      ; Subtract B register from A\n"
                                   "        SUB C      ; Subtract C register from A\n"
                                   "        SUB D      ; Subtract D register from A\n"
                                   "        SUB E      ; Subtract E register from A\n"
                                   "        SUB H      ; Subtract H register from A\n"
                                   "        SUB L      ; Subtract L register from A\n"
                                   "        SUB A      ; Subtract A register from A\n"
                                   "        SUB M      ; Subtract memory from A\n"
                                   "        SBB B      ; Subtract B register with borrow from A\n"
                                   "        SBB C      ; Subtract C register with borrow from A\n"
                                   "        SBB D      ; Subtract D register with borrow from A\n"
                                   "        SBB E      ; Subtract E register with borrow from A\n"
                                   "        SBB H      ; Subtract H register with borrow from A\n"
                                   "        SBB L      ; Subtract L register with borrow from A\n"
                                   "        SBB A      ; Subtract A register with borrow from A\n"
                                   "        SBB M      ; Subtract memory with borrow from A\n"
                                   "        ANA B      ; And B register with A\n"
                                   "        ANA C      ; And C register with A\n"
                                   "        ANA D      ; And D register with A\n"
                                   "        ANA E      ; And E register with A\n"
                                   "        ANA H      ; And H register with A\n"
                                   "        ANA L      ; And L register with A\n"
                                   "        ANA A      ; And A register with A\n"
                                   "        ANA M      ; And memory with A\n"
                                   "        ORA B      ; Or B register with A\n"
                                   "        ORA C      ; Or C register with A\n"
                                   "        ORA D      ; Or D register with A\n"
                                   "        ORA E      ; Or E register with A\n"
                                   "        ORA H      ; Or H register with A\n"
                                   "        ORA L      ; Or L register with A\n"
                                   "        ORA A      ; Or A register with A\n"
                                   "        ORA M      ; Or memory with A\n"
                                   "        XRA B      ; Exclusive or B register with A\n"
                                   "        XRA C      ; Exclusive or C register with A\n"
                                   "        XRA D      ; Exclusive or D register with A\n"
                                   "        XRA E      ; Exclusive or E register with A\n"
                                   "        XRA H      ; Exclusive or H register with A\n"
                                   "        XRA L      ; Exclusive or L register with A\n"
                                   "        XRA A      ; Exclusive or A register with A\n"
                                   "        XRA M      ; Exclusive or memory with A\n"
                                   "        CMP B      ; Compare B register with A\n"
                                   "        CMP C      ; Compare C register with A\n"
                                   "        CMP D      ; Compare D register with A\n"
                                   "        CMP E      ; Compare E register with A\n"
                                   "        CMP H      ; Compare H register with A\n"
                                   "        CMP L      ; Compare L register with A\n"
                                   "        CMP A      ; Compare A register with A\n"
                                   "        CMP M      ; Compare memory with A\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        ADD B      ; Add B register to A\n"
              L"        ADD C      ; Add C register to A\n"
              L"        ADD D      ; Add D register to A\n"
              L"        ADD E      ; Add E register to A\n"
              L"        ADD H      ; Add H register to A\n"
              L"        ADD L      ; Add L register to A\n"
              L"        ADD A      ; Add A register to A\n"
              L"        ADD M      ; Add memory to A\n"
              L"        ADC B      ; Add B register with carry to A\n"
              L"        ADC C      ; Add C register with carry to A\n"
              L"        ADC D      ; Add D register with carry to A\n"
              L"        ADC E      ; Add E register with carry to A\n"
              L"        ADC H      ; Add H register with carry to A\n"
              L"        ADC L      ; Add L register with carry to A\n"
              L"        ADC A      ; Add A register with carry to A\n"
              L"        ADC M      ; Add memory with carry to A\n"
              L"        SUB B      ; Subtract B register from A\n"
              L"        SUB C      ; Subtract C register from A\n"
              L"        SUB D      ; Subtract D register from A\n"
              L"        SUB E      ; Subtract E register from A\n"
              L"        SUB H      ; Subtract H register from A\n"
              L"        SUB L      ; Subtract L register from A\n"
              L"        SUB A      ; Subtract A register from A\n"
              L"        SUB M      ; Subtract memory from A\n"
              L"        SBB B      ; Subtract B register with borrow from A\n"
              L"        SBB C      ; Subtract C register with borrow from A\n"
              L"        SBB D      ; Subtract D register with borrow from A\n"
              L"        SBB E      ; Subtract E register with borrow from A\n"
              L"        SBB H      ; Subtract H register with borrow from A\n"
              L"        SBB L      ; Subtract L register with borrow from A\n"
              L"        SBB A      ; Subtract A register with borrow from A\n"
              L"        SBB M      ; Subtract memory with borrow from A\n"
              L"        ANA B      ; And B register with A\n"
              L"        ANA C      ; And C register with A\n"
              L"        ANA D      ; And D register with A\n"
              L"        ANA E      ; And E register with A\n"
              L"        ANA H      ; And H register with A\n"
              L"        ANA L      ; And L register with A\n"
              L"        ANA A      ; And A register with A\n"
              L"        ANA M      ; And memory with A\n"
              L"        ORA B      ; Or B register with A\n"
              L"        ORA C      ; Or C register with A\n"
              L"        ORA D      ; Or D register with A\n"
              L"        ORA E      ; Or E register with A\n"
              L"        ORA H      ; Or H register with A\n"
              L"        ORA L      ; Or L register with A\n"
              L"        ORA A      ; Or A register with A\n"
              L"        ORA M      ; Or memory with A\n"
              L"        XRA B      ; Exclusive or B register with A\n"
              L"        XRA C      ; Exclusive or C register with A\n"
              L"        XRA D      ; Exclusive or D register with A\n"
              L"        XRA E      ; Exclusive or E register with A\n"
              L"        XRA H      ; Exclusive or H register with A\n"
              L"        XRA L      ; Exclusive or L register with A\n"
              L"        XRA A      ; Exclusive or A register with A\n"
              L"        XRA M      ; Exclusive or memory with A\n"
              L"        CMP B      ; Compare B register with A\n"
              L"        CMP C      ; Compare C register with A\n"
              L"        CMP D      ; Compare D register with A\n"
              L"        CMP E      ; Compare E register with A\n"
              L"        CMP H      ; Compare H register with A\n"
              L"        CMP L      ; Compare L register with A\n"
              L"        CMP A      ; Compare A register with A\n"
              L"        CMP M      ; Compare memory with A\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeR8(node, OpcodeType::ADD, L"ADD", Register8Type::B, L"B");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ADD, L"ADD", Register8Type::C, L"C");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ADD, L"ADD", Register8Type::D, L"D");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ADD, L"ADD", Register8Type::E, L"E");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ADD, L"ADD", Register8Type::H, L"H");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ADD, L"ADD", Register8Type::L, L"L");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ADD, L"ADD", Register8Type::A, L"A");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ADD, L"ADD", Register8Type::M, L"M");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ADC, L"ADC", Register8Type::B, L"B");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ADC, L"ADC", Register8Type::C, L"C");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ADC, L"ADC", Register8Type::D, L"D");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ADC, L"ADC", Register8Type::E, L"E");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ADC, L"ADC", Register8Type::H, L"H");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ADC, L"ADC", Register8Type::L, L"L");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ADC, L"ADC", Register8Type::A, L"A");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ADC, L"ADC", Register8Type::M, L"M");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::SUB, L"SUB", Register8Type::B, L"B");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::SUB, L"SUB", Register8Type::C, L"C");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::SUB, L"SUB", Register8Type::D, L"D");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::SUB, L"SUB", Register8Type::E, L"E");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::SUB, L"SUB", Register8Type::H, L"H");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::SUB, L"SUB", Register8Type::L, L"L");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::SUB, L"SUB", Register8Type::A, L"A");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::SUB, L"SUB", Register8Type::M, L"M");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::SBB, L"SBB", Register8Type::B, L"B");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::SBB, L"SBB", Register8Type::C, L"C");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::SBB, L"SBB", Register8Type::D, L"D");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::SBB, L"SBB", Register8Type::E, L"E");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::SBB, L"SBB", Register8Type::H, L"H");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::SBB, L"SBB", Register8Type::L, L"L");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::SBB, L"SBB", Register8Type::A, L"A");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::SBB, L"SBB", Register8Type::M, L"M");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ANA, L"ANA", Register8Type::B, L"B");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ANA, L"ANA", Register8Type::C, L"C");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ANA, L"ANA", Register8Type::D, L"D");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ANA, L"ANA", Register8Type::E, L"E");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ANA, L"ANA", Register8Type::H, L"H");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ANA, L"ANA", Register8Type::L, L"L");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ANA, L"ANA", Register8Type::A, L"A");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ANA, L"ANA", Register8Type::M, L"M");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ORA, L"ORA", Register8Type::B, L"B");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ORA, L"ORA", Register8Type::C, L"C");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ORA, L"ORA", Register8Type::D, L"D");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ORA, L"ORA", Register8Type::E, L"E");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ORA, L"ORA", Register8Type::H, L"H");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ORA, L"ORA", Register8Type::L, L"L");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ORA, L"ORA", Register8Type::A, L"A");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ORA, L"ORA", Register8Type::M, L"M");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::XRA, L"XRA", Register8Type::B, L"B");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::XRA, L"XRA", Register8Type::C, L"C");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::XRA, L"XRA", Register8Type::D, L"D");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::XRA, L"XRA", Register8Type::E, L"E");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::XRA, L"XRA", Register8Type::H, L"H");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::XRA, L"XRA", Register8Type::L, L"L");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::XRA, L"XRA", Register8Type::A, L"A");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::XRA, L"XRA", Register8Type::M, L"M");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::CMP, L"CMP", Register8Type::B, L"B");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::CMP, L"CMP", Register8Type::C, L"C");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::CMP, L"CMP", Register8Type::D, L"D");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::CMP, L"CMP", Register8Type::E, L"E");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::CMP, L"CMP", Register8Type::H, L"H");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::CMP, L"CMP", Register8Type::L, L"L");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::CMP, L"CMP", Register8Type::A, L"A");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::CMP, L"CMP", Register8Type::M, L"M");

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseADD_ADC_SUB_SBB_ANA_ORA_XRA_CMPInvalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        ADD X      ; Invalid register\n"
                                   "        ADC X      ; Invalid register\n"
                                   "        SUB X      ; Invalid register\n"
                                   "        SBB X      ; Invalid register\n"
                                   "        ANA X      ; Invalid register\n"
                                   "        ORA X      ; Invalid register\n"
                                   "        XRA X      ; Invalid register\n"
                                   "        CMP X      ; Invalid register\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        ADD X      ; Invalid register\n"
              L"        ADC X      ; Invalid register\n"
              L"        SUB X      ; Invalid register\n"
              L"        SBB X      ; Invalid register\n"
              L"        ANA X      ; Invalid register\n"
              L"        ORA X      ; Invalid register\n"
              L"        XRA X      ; Invalid register\n"
              L"        CMP X      ; Invalid register\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"Error: 2:13 - Invalid register: X\nError: 3:13 - Invalid register: X\n"
              L"Error: 4:13 - Invalid register: X\nError: 5:13 - Invalid register: X\n"
              L"Error: 6:13 - Invalid register: X\nError: 7:13 - Invalid register: X\n"
              L"Error: 8:13 - Invalid register: X\nError: 9:13 - Invalid register: X\n", errorStream.str());
    EXPECT_EQ(size_t{ 8 }, messages.size());
    EXPECT_EQ(size_t{ 8 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeR8(node, OpcodeType::ADD, L"ADD", Register8Type::Invalid, L"");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ADC, L"ADC", Register8Type::Invalid, L"");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::SUB, L"SUB", Register8Type::Invalid, L"");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::SBB, L"SBB", Register8Type::Invalid, L"");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ANA, L"ANA", Register8Type::Invalid, L"");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::ORA, L"ORA", Register8Type::Invalid, L"");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::XRA, L"XRA", Register8Type::Invalid, L"");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::CMP, L"CMP", Register8Type::Invalid, L"");

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseADI_ACI_SUI_SBI_ANI_ORI_XRI_CPI)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        ADI 00h  ; Add immediate to A\n"
                                   "        ACI 01h  ; Add immediate to A with carry\n"
                                   "        SUI 02h  ; Subtract immediate from A\n"
                                   "        SBI 03h  ; Subtract immediate from A with borrow\n"
                                   "        ANI 04h  ; And immediate with A\n"
                                   "        ORI 05h  ; Or immediate with A\n"
                                   "        XRI 06h  ; Exclusive or immediate with A\n"
                                   "        CPI 07h  ; Compare immediate with A\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        ADI 00h  ; Add immediate to A\n"
              L"        ACI 01h  ; Add immediate to A with carry\n"
              L"        SUI 02h  ; Subtract immediate from A\n"
              L"        SBI 03h  ; Subtract immediate from A with borrow\n"
              L"        ANI 04h  ; And immediate with A\n"
              L"        ORI 05h  ; Or immediate with A\n"
              L"        XRI 06h  ; Exclusive or immediate with A\n"
              L"        CPI 07h  ; Compare immediate with A\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeD8(node, OpcodeType::ADI, L"ADI", L"00h", 0x00);
    node = node->Next();
    CheckOpcodeD8(node, OpcodeType::ACI, L"ACI", L"01h", 0x01);
    node = node->Next();
    CheckOpcodeD8(node, OpcodeType::SUI, L"SUI", L"02h", 0x02);
    node = node->Next();
    CheckOpcodeD8(node, OpcodeType::SBI, L"SBI", L"03h", 0x03);
    node = node->Next();
    CheckOpcodeD8(node, OpcodeType::ANI, L"ANI", L"04h", 0x04);
    node = node->Next();
    CheckOpcodeD8(node, OpcodeType::ORI, L"ORI", L"05h", 0x05);
    node = node->Next();
    CheckOpcodeD8(node, OpcodeType::XRI, L"XRI", L"06h", 0x06);
    node = node->Next();
    CheckOpcodeD8(node, OpcodeType::CPI, L"CPI", L"07h", 0x07);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseDAD)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        DAD B      ; Add B&C to H&L\n"
                                   "        DAD D      ; Add D&E to H&L\n"
                                   "        DAD H      ; Add H&L to H&L\n"
                                   "        DAD SP     ; Add SP to H&L\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        DAD B      ; Add B&C to H&L\n"
              L"        DAD D      ; Add D&E to H&L\n"
              L"        DAD H      ; Add H&L to H&L\n"
              L"        DAD SP     ; Add SP to H&L\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeR16(node, OpcodeType::DAD, L"DAD", Register16Type::BC, L"B");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::DAD, L"DAD", Register16Type::DE, L"D");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::DAD, L"DAD", Register16Type::HL, L"H");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::DAD, L"DAD", Register16Type::SP, L"SP");

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseDADInvalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        DAD X      ; Invalid register pair\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        DAD X      ; Invalid register pair\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"Error: 2:13 - Invalid register pair: X\n", errorStream.str());
    EXPECT_EQ(size_t{ 1 }, messages.size());
    EXPECT_EQ(size_t{ 1 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeR16(node, OpcodeType::DAD, L"DAD", Register16Type::Invalid, L"");

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseRLC_RRC_RAL_RAR_CMA_STC_CMC_DAA)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        RLC     ; Rotate A left\n"
                                   "        RRC     ; Rotate A right\n"
                                   "        RAL     ; Rotate A left through carry\n"
                                   "        RAR     ; Rotate A right through carry\n"
                                   "        CMA     ; Complement A\n"
                                   "        STC     ; Set carry\n"
                                   "        CMC     ; Complement carry\n"
                                   "        DAA     ; Decimal adjust A\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        RLC     ; Rotate A left\n"
              L"        RRC     ; Rotate A right\n"
              L"        RAL     ; Rotate A left through carry\n"
              L"        RAR     ; Rotate A right through carry\n"
              L"        CMA     ; Complement A\n"
              L"        STC     ; Set carry\n"
              L"        CMC     ; Complement carry\n"
              L"        DAA     ; Decimal adjust A\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcode(node, OpcodeType::RLC, L"RLC");
    node = node->Next();
    CheckOpcode(node, OpcodeType::RRC, L"RRC");
    node = node->Next();
    CheckOpcode(node, OpcodeType::RAL, L"RAL");
    node = node->Next();
    CheckOpcode(node, OpcodeType::RAR, L"RAR");
    node = node->Next();
    CheckOpcode(node, OpcodeType::CMA, L"CMA");
    node = node->Next();
    CheckOpcode(node, OpcodeType::STC, L"STC");
    node = node->Next();
    CheckOpcode(node, OpcodeType::CMC, L"CMC");
    node = node->Next();
    CheckOpcode(node, OpcodeType::DAA, L"DAA");

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseIN_OUT)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        IN 00h  ; Input to A\n"
                                   "        OUT 01h ; Output from A\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        IN 00h  ; Input to A\n"
              L"        OUT 01h ; Output from A\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeD8(node, OpcodeType::INP, L"IN", L"00h", 0x00);
    node = node->Next();
    CheckOpcodeD8(node, OpcodeType::OUTP, L"OUT", L"01h", 0x01);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseEI_DI_NOP_HLT)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "START:  ORG 0000; Set initial address\n"
                                   "        EI      ; Enable interrupts\n"
                                   "        DI      ; Disable interrupts\n"
                                   "NOP:    NOP     ; No-operation\n"
                                   "        HLT     ; Halt\n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"START:  ORG 0000; Set initial address\n"
              L"        EI      ; Enable interrupts\n"
              L"        DI      ; Disable interrupts\n"
              L"NOP:    NOP     ; No-operation\n"
              L"        HLT     ; Halt\n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();
    CheckOrg(node, L"0000", L"START");

    node = node->Next();
    CheckOpcode(node, OpcodeType::EI, L"EI");
    node = node->Next();
    CheckOpcode(node, OpcodeType::DI, L"DI");
    node = node->Next();
    CheckOpcode(node, OpcodeType::NOP, L"NOP", L"NOP");
    node = node->Next();
    CheckOpcode(node, OpcodeType::HLT, L"HLT");

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, Parse8085Extensions)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8085; Set CPU type\n"
                                   "        DSUB      ; \n"
                                   "        ARHL      ; \n"
                                   "        RDEL      ; \n"
                                   "        RIM       ; \n"
                                   "        LDHI 01h  ; \n"
                                   "        SIM       ; \n"
                                   "        LDSI 02h  ; \n"
                                   "        RSTV      ; \n"
                                   "        SHLX      ; \n"
                                   "        JNK 0103h ; \n"
                                   "        LHLX      ; \n"
                                   "        JK  0104h ; \n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8085; Set CPU type\n"
              L"        DSUB      ; \n"
              L"        ARHL      ; \n"
              L"        RDEL      ; \n"
              L"        RIM       ; \n"
              L"        LDHI 01h  ; \n"
              L"        SIM       ; \n"
              L"        LDSI 02h  ; \n"
              L"        RSTV      ; \n"
              L"        SHLX      ; \n"
              L"        JNK 0103h ; \n"
              L"        LHLX      ; \n"
              L"        JK  0104h ; \n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcode(node, OpcodeType::DSUB, L"DSUB");
    node = node->Next();
    CheckOpcode(node, OpcodeType::ARHL, L"ARHL");
    node = node->Next();
    CheckOpcode(node, OpcodeType::RDEL, L"RDEL");
    node = node->Next();
    CheckOpcode(node, OpcodeType::RIM, L"RIM");
    node = node->Next();
    CheckOpcodeD8(node, OpcodeType::LDHI, L"LDHI", L"01h", 0x01);
    node = node->Next();
    CheckOpcode(node, OpcodeType::SIM, L"SIM");
    node = node->Next();
    CheckOpcodeD8(node, OpcodeType::LDSI, L"LDSI", L"02h", 0x02);
    node = node->Next();
    CheckOpcode(node, OpcodeType::RSTV, L"RSTV");
    node = node->Next();
    CheckOpcode(node, OpcodeType::SHLX, L"SHLX");
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::JNK, L"JNK", L"0103h", 0x0103);
    node = node->Next();
    CheckOpcode(node, OpcodeType::LHLX, L"LHLX");
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::JK, L"JK", L"0104h", 0x0104);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, Parse8085ExtensionsOn8080)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        DSUB     ; \n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        DSUB     ; \n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"Error: 3:0 - Expected Opcode: DSUB\n", errorStream.str());
    EXPECT_EQ(size_t{ 1 }, messages.size());
    EXPECT_EQ(size_t{ 1 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    EXPECT_NULL(node);
}

TEST_FIXTURE(ParserTest, ParseJMPWithLocationCounter)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        JMP $; \n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        JMP $; \n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeLocCounter(node, OpcodeType::JMP, L"JMP");

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseJMPWithPredefinedLabel)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "START:  NOP \n"
                                   "        JMP START; \n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"START:  NOP \n"
              L"        JMP START; \n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcode(node, OpcodeType::NOP, L"NOP", L"START");
    node = node->Next();
    CheckOpcodeRef(node, OpcodeType::JMP, L"JMP", L"START");

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseJMPWithUndefinedLabel)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080; Set CPU type\n"
                                   "        JMP START; \n"
                                   "START:  NOP \n"
                                   "        END ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true, reportStream);
    Parser parser(scanner, messages, errorStream);

    parser.Parse();

    EXPECT_EQ(L"        CPU Intel8080; Set CPU type\n"
              L"        JMP START; \n"
              L"START:  NOP \n"
              L"        END ; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckOpcodeRef(node, OpcodeType::JMP, L"JMP", L"START");
    node = node->Next();
    CheckOpcode(node, OpcodeType::NOP, L"NOP", L"START");

    EXPECT_NULL(node->Next());
}

} // namespace Test

} // namespace Assembler
