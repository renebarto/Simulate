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

using AddressType = uint16_t;

void CheckCPU(ASTNode::Ptr node, std::wstring const & value)
{
    ASSERT_NOT_NULL(node);
    EXPECT_EQ(ASTNodeType::CPU, node->NodeType());
    EXPECT_EQ(value, node->Value());
    EXPECT_NULL(node->FirstChild());
}

void CheckComment(ASTNode::Ptr node, std::wstring const & value)
{
    ASSERT_NOT_NULL(node);
    EXPECT_EQ(ASTNodeType::Comment, node->NodeType());
    EXPECT_EQ(value, node->Value());
    EXPECT_NULL(node->FirstChild());
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

void CheckEnd(ASTNode::Ptr node, std::wstring const & labelValue = L"")
{
    ASSERT_NOT_NULL(node);
    EXPECT_EQ(ASTNodeType::END, node->NodeType());
    EXPECT_EQ(L"END", node->Value());
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
    OpcodeNode<OpcodeType, AddressType> * opcodeNode = dynamic_cast<OpcodeNode<OpcodeType, AddressType> *>(node.get());
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
    EXPECT_EQ(value, uint8_t(ConvertToValue(childNode->Value())));
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
    EXPECT_EQ(value, uint16_t(ConvertToValue(childNode->Value())));
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
    CheckRegister8(dstReg, dddType, ddd);
    Register8Node<Register8Type>::Ptr srcReg = dstReg->Next();
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
    Data8Node::Ptr data = reg->Next();
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
    Data16Node::Ptr data = reg->Next();
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
    LocCounterNode<AddressType> * locCounterNode = dynamic_cast<LocCounterNode<AddressType> *>(child2Node.get());
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
    RefAddressNode<AddressType> * refNode = dynamic_cast<RefAddressNode<AddressType> *>(child2Node.get());
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
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

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
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);
    parser.Parse();

    EXPECT_EQ(L"Error: 1:1 - \"CPU\" expected\nError: 1:1 - Identifier expected\n", reportStream.str());
    EXPECT_EQ(L"Error: 1:1 - \"CPU\" expected\n", errorStream.str());
    EXPECT_EQ(size_t{ 2 }, messages.size());
    EXPECT_EQ(size_t{ 2 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    EXPECT_THROW(parser.GetAST(), AssemblerException);

    MachineCode ref;
    MachineCode const & machineCode = parser.GetMachineCode();
    EXPECT_TRUE(Core::Util::Compare(ref, machineCode));
}

TEST_FIXTURE(ParserTest, ParseEnd)
{
    AssemblerMessages messages;
    std::istringstream inputStream("End");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);
    parser.Parse();

    EXPECT_EQ(L"Error: 1:1 - \"CPU\" expected\nError: 1:1 - Identifier expected\n", reportStream.str());
    EXPECT_EQ(L"Error: 1:1 - \"CPU\" expected\n", errorStream.str());
    EXPECT_EQ(size_t{ 2 }, messages.size());
    EXPECT_EQ(size_t{ 2 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    EXPECT_THROW(parser.GetAST(), AssemblerException);

    MachineCode ref;
    MachineCode const & machineCode = parser.GetMachineCode();
    EXPECT_TRUE(Core::Util::Compare(ref, machineCode));
}

TEST_FIXTURE(ParserTest, ParseSimple1)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080 ; Set CPU type\n"
                                   "START:  ORG 0000      ; Set initial address\n"
                                   "        END           ; Stop\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    EXPECT_EQ(L"              CPU Intel8080; Set CPU type\n"
              L"              START:ORG 0000; Set initial address\n"
              L"              END; Stop\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();
    CheckCPU(node, L"Intel8080");
    node = node->Next();
    CheckComment(node, L" Set CPU type");
    node = node->Next();
    CheckOrg(node, L"0000", L"START");
    node = node->Next();
    CheckComment(node, L" Set initial address");
    node = node->Next();
    CheckEnd(node);
    node = node->Next();
    CheckComment(node, L" Stop");

    EXPECT_NULL(node->Next());

    MachineCode ref;
    MachineCode const & machineCode = parser.GetMachineCode();
    EXPECT_TRUE(Core::Util::Compare(ref, machineCode));
}

TEST_FIXTURE(ParserTest, ParseMOV)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        MOV A,A\n"
                                   "        MOV A,B\n"
                                   "        MOV A,C\n"
                                   "        MOV A,D\n"
                                   "        MOV A,E\n"
                                   "        MOV A,H\n"
                                   "        MOV A,L\n"
                                   "        MOV B,A\n"
                                   "        MOV C,A\n"
                                   "        MOV D,A\n"
                                   "        MOV E,A\n"
                                   "        MOV H,A\n"
                                   "        MOV L,A\n"
                                   "        MOV A,M\n"
                                   "        MOV M,A\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    EXPECT_EQ(L"              CPU Intel8080\n"
              L"0000 7F       MOV A,A\n"
              L"0001 78       MOV A,B\n"
              L"0002 79       MOV A,C\n"
              L"0003 7A       MOV A,D\n"
              L"0004 7B       MOV A,E\n"
              L"0005 7C       MOV A,H\n"
              L"0006 7D       MOV A,L\n"
              L"0007 47       MOV B,A\n"
              L"0008 4F       MOV C,A\n"
              L"0009 57       MOV D,A\n"
              L"000A 5F       MOV E,A\n"
              L"000B 67       MOV H,A\n"
              L"000C 6F       MOV L,A\n"
              L"000D 7E       MOV A,M\n"
              L"000E 77       MOV M,A\n"
              L"              END\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
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
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0x7F, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x47, 0x4F, 0x57, 0x5F, 0x67, 0x6F, 0x7E, 0x77};
    MachineCode const & machineCode = parser.GetMachineCode();
    EXPECT_TRUE(Core::Util::Compare(ref, machineCode));
}

TEST_FIXTURE(ParserTest, ParseMOVInvalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        MOV M,M\n"
                                   "        MOV X,A\n"
                                   "        MOV A,X\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    EXPECT_EQ(L"              CPU Intel8080\n"
              L"0000          MOV M,M\n"
              L"--> Error: 2:9 - Incorrect opcode: MOV M,M\n"
              L"0001          MOV X,A\n"
              L"--> Error: 3:13 - Invalid destination register: X\n"
              L"0002          MOV A,X\n"
              L"--> Error: 4:15 - Invalid source register: X\n"
              L"              END\n", reportStream.str());
    EXPECT_EQ(L"Error: 2:9 - Incorrect opcode: MOV M,M\nError: 3:13 - Invalid destination register: X\nError: 4:15 - Invalid source register: X\n", errorStream.str());
    EXPECT_EQ(size_t{ 3 }, messages.size());
    EXPECT_EQ(size_t{ 3 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
    CheckOpcodeR8R8(node, L"MOV", Register8Type::M, L"M", Register8Type::M, L"M");
    node = node->Next();
    CheckOpcodeR8R8(node, L"MOV", Register8Type::Invalid, L"X", Register8Type::A, L"A");
    node = node->Next();
    CheckOpcodeR8R8(node, L"MOV", Register8Type::A, L"A", Register8Type::Invalid, L"X");
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref;
    MachineCode const & machineCode = parser.GetMachineCode();
    EXPECT_TRUE(Core::Util::Compare(ref, machineCode));
}

TEST_FIXTURE(ParserTest, ParseMVI)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        MVI B,00h\n"
                                   "        MVI C,01h\n"
                                   "        MVI D,02h\n"
                                   "        MVI E,03h\n"
                                   "        MVI H,04h\n"
                                   "        MVI L,05h\n"
                                   "        MVI A,06h\n"
                                   "        MVI M,07h\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    EXPECT_EQ(L"              CPU Intel8080\n"
              L"0000 06 00    MVI B,00h\n"
              L"0002 0E 01    MVI C,01h\n"
              L"0004 16 02    MVI D,02h\n"
              L"0006 1E 03    MVI E,03h\n"
              L"0008 26 04    MVI H,04h\n"
              L"000A 2E 05    MVI L,05h\n"
              L"000C 3E 06    MVI A,06h\n"
              L"000E 36 07    MVI M,07h\n"
              L"              END\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
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
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0x06, 0x00, 0x0E, 0x01, 0x16, 0x02, 0x1E, 0x03, 0x26, 0x04, 0x2E, 0x05, 0x3E, 0x06, 0x36, 0x07 };
    MachineCode const & machineCode = parser.GetMachineCode();
    EXPECT_TRUE(Core::Util::Compare(ref, machineCode));
}

TEST_FIXTURE(ParserTest, ParseMVIInvalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        MVI X, 08h\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    EXPECT_EQ(L"              CPU Intel8080\n"
              L"0000          MVI X,08h\n"
              L"--> Error: 2:13 - Invalid register: X\n"
              L"              END\n", reportStream.str());
    EXPECT_EQ(L"Error: 2:13 - Invalid register: X\n", errorStream.str());
    EXPECT_EQ(size_t{ 1 }, messages.size());
    EXPECT_EQ(size_t{ 1 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
    CheckOpcodeR8D8(node, OpcodeType::MVI, L"MVI", Register8Type::Invalid, L"", L"08h", 8);
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref;
    MachineCode const & machineCode = parser.GetMachineCode();
    EXPECT_TRUE(Core::Util::Compare(ref, machineCode));
}

TEST_FIXTURE(ParserTest, ParseLXI)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        LXI B,0100h\n"
                                   "        LXI D,0302h\n"
                                   "        LXI H,0504h\n"
                                   "        LXI SP, 0706h\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    EXPECT_EQ(L"              CPU Intel8080\n"
              L"0000 01 00 01 LXI B,0100h\n"
              L"0003 11 02 03 LXI D,0302h\n"
              L"0006 21 04 05 LXI H,0504h\n"
              L"0009 31 06 07 LXI SP,0706h\n"
              L"              END\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
    CheckOpcodeR16D16(node, OpcodeType::LXI, L"LXI", Register16Type::BC, L"B", L"0100h", 0x0100);
    node = node->Next();
    CheckOpcodeR16D16(node, OpcodeType::LXI, L"LXI", Register16Type::DE, L"D", L"0302h", 0x0302);
    node = node->Next();
    CheckOpcodeR16D16(node, OpcodeType::LXI, L"LXI", Register16Type::HL, L"H", L"0504h", 0x0504);
    node = node->Next();
    CheckOpcodeR16D16(node, OpcodeType::LXI, L"LXI", Register16Type::SP, L"SP", L"0706h", 0x0706);
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0x01, 0x00, 0x01, 0x11, 0x02, 0x03, 0x21, 0x04, 0x05, 0x31, 0x06, 0x07 };
    MachineCode const & machineCode = parser.GetMachineCode();
    EXPECT_TRUE(Core::Util::Compare(ref, machineCode));
}

TEST_FIXTURE(ParserTest, ParseLXIInvalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        LXI PSW, 0101h\n"
                                   "        LXI X, 0101h\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    EXPECT_EQ(L"              CPU Intel8080\n"
              L"0000          LXI PSW,0101h\n"
              L"--> Error: 2:9 - Opcode LXI only supports B, D, H or SP register pair, PSW specified\n"
              L"0003          LXI X,0101h\n"
              L"--> Error: 3:13 - Invalid register pair: X\n"
              L"              END\n", reportStream.str());
    EXPECT_EQ(L"Error: 2:9 - Opcode LXI only supports B, D, H or SP register pair, PSW specified\nError: 3:13 - Invalid register pair: X\n", errorStream.str());
    EXPECT_EQ(size_t{ 2 }, messages.size());
    EXPECT_EQ(size_t{ 2 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
    CheckOpcodeR16D16(node, OpcodeType::LXI, L"LXI", Register16Type::PSW, L"PSW", L"0101h", 0x0101);
    node = node->Next();
    CheckOpcodeR16D16(node, OpcodeType::LXI, L"LXI", Register16Type::Invalid, L"", L"0101h", 0x0101);
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref;
    MachineCode const & machineCode = parser.GetMachineCode();
    EXPECT_TRUE(Core::Util::Compare(ref, machineCode));
}

TEST_FIXTURE(ParserTest, ParseLXIInvalid2)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        LXI D, 10000h\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    EXPECT_EQ(L"              CPU Intel8080\n"
              L"0000 11 00 00 LXI D,10000h\n"
              L"--> Error: 2:16 - Value out of range for 16 bit value: 65536\n"
              L"              END\n", reportStream.str());
    EXPECT_EQ(L"Error: 2:16 - Value out of range for 16 bit value: 65536\n", errorStream.str());
    EXPECT_EQ(size_t{ 1 }, messages.size());
    EXPECT_EQ(size_t{ 1 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
    CheckOpcodeR16D16(node, OpcodeType::LXI, L"LXI", Register16Type::DE, L"D", L"10000h", 0x0000);
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0x11, 0x00, 0x00 };
    MachineCode const & machineCode = parser.GetMachineCode();
    EXPECT_TRUE(Core::Util::Compare(ref, machineCode));
}

TEST_FIXTURE(ParserTest, ParseLDAX_STAX)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        STAX B\n"
                                   "        STAX D\n"
                                   "        LDAX B\n"
                                   "        LDAX D\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    EXPECT_EQ(L"              CPU Intel8080\n"
              L"0000 02       STAX B\n"
              L"0001 12       STAX D\n"
              L"0002 0A       LDAX B\n"
              L"0003 1A       LDAX D\n"
              L"              END\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::STAX, L"STAX", Register16Type::BC, L"B");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::STAX, L"STAX", Register16Type::DE, L"D");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::LDAX, L"LDAX", Register16Type::BC, L"B");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::LDAX, L"LDAX", Register16Type::DE, L"D");
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0x02, 0x12, 0x0A, 0x1A };
    MachineCode const & machineCode = parser.GetMachineCode();
    EXPECT_TRUE(Core::Util::Compare(ref, machineCode));
}

TEST_FIXTURE(ParserTest, ParseLDAX_STAXInvalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        STAX H\n"
                                   "        STAX SP\n"
                                   "        LDAX H\n"
                                   "        LDAX SP\n"
                                   "        STAX X\n"
                                   "        LDAX X\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    EXPECT_EQ(L"              CPU Intel8080\n"
              L"0000          STAX H\n"
              L"--> Error: 2:9 - Opcode STAX only supports B or D register pair, H specified\n"
              L"0001          STAX SP\n"
              L"--> Error: 3:9 - Opcode STAX only supports B or D register pair, SP specified\n"
              L"0002          LDAX H\n"
              L"--> Error: 4:9 - Opcode LDAX only supports B or D register pair, H specified\n"
              L"0003          LDAX SP\n"
              L"--> Error: 5:9 - Opcode LDAX only supports B or D register pair, SP specified\n"
              L"0004          STAX X\n"
              L"--> Error: 6:14 - Invalid register pair: X\n"
              L"0005          LDAX X\n"
              L"--> Error: 7:14 - Invalid register pair: X\n"
              L"              END\n", reportStream.str());
    EXPECT_EQ(L"Error: 2:9 - Opcode STAX only supports B or D register pair, H specified\nError: 3:9 - Opcode STAX only supports B or D register pair, SP specified\n"
              L"Error: 4:9 - Opcode LDAX only supports B or D register pair, H specified\nError: 5:9 - Opcode LDAX only supports B or D register pair, SP specified\n"
              L"Error: 6:14 - Invalid register pair: X\nError: 7:14 - Invalid register pair: X\n", errorStream.str());
    EXPECT_EQ(size_t{ 6 }, messages.size());
    EXPECT_EQ(size_t{ 6 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
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
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref;
    MachineCode const & machineCode = parser.GetMachineCode();
    EXPECT_TRUE(Core::Util::Compare(ref, machineCode));
}

TEST_FIXTURE(ParserTest, ParseSTA_LDA)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        STA 0100h\n"
                                   "        LDA 0302h\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    EXPECT_EQ(L"              CPU Intel8080\n"
              L"0000 32 00 01 STA 0100h\n"
              L"0003 3A 02 03 LDA 0302h\n"
              L"              END\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::STA, L"STA", L"0100h", 0x0100);
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::LDA, L"LDA", L"0302h", 0x0302);
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0x32, 0x00, 0x01, 0x3A, 0x02, 0x03 };
    MachineCode const & machineCode = parser.GetMachineCode();
    EXPECT_TRUE(Core::Util::Compare(ref, machineCode));
}

TEST_FIXTURE(ParserTest, ParseSTA_LDAInvalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        STA 10000h\n"
                                   "        LDA 10000h\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    EXPECT_EQ(L"              CPU Intel8080\n"
              L"0000 32 00 00 STA 10000h\n"
              L"--> Error: 2:13 - Value out of range for 16 bit value: 65536\n"
              L"0003 3A 00 00 LDA 10000h\n"
              L"--> Error: 3:13 - Value out of range for 16 bit value: 65536\n"
              L"              END\n", reportStream.str());
    EXPECT_EQ(L"Error: 2:13 - Value out of range for 16 bit value: 65536\nError: 3:13 - Value out of range for 16 bit value: 65536\n", errorStream.str());
    EXPECT_EQ(size_t{ 2 }, messages.size());
    EXPECT_EQ(size_t{ 2 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::STA, L"STA", L"10000h", 0x0000);
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::LDA, L"LDA", L"10000h", 0x0000);
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0x32, 0x00, 0x00, 0x3A, 0x00, 0x00 };
    MachineCode const & machineCode = parser.GetMachineCode();
    EXPECT_TRUE(Core::Util::Compare(ref, machineCode));
}

TEST_FIXTURE(ParserTest, ParseSHLD_LHLD)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        SHLD 0100h\n"
                                   "        LHLD 0302h\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    EXPECT_EQ(L"              CPU Intel8080\n"
              L"0000 22 00 01 SHLD 0100h\n"
              L"0003 2A 02 03 LHLD 0302h\n"
              L"              END\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::SHLD, L"SHLD", L"0100h", 0x0100);
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::LHLD, L"LHLD", L"0302h", 0x0302);
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0x22, 0x00, 0x01, 0x2A, 0x02, 0x03 };
    MachineCode const & machineCode = parser.GetMachineCode();
    EXPECT_TRUE(Core::Util::Compare(ref, machineCode));
}

TEST_FIXTURE(ParserTest, ParseSHLD_LHLDInvalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        SHLD 10000h\n"
                                   "        LHLD 10000h\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    EXPECT_EQ(L"              CPU Intel8080\n"
              L"0000 22 00 00 SHLD 10000h\n"
              L"--> Error: 2:14 - Value out of range for 16 bit value: 65536\n"
              L"0003 2A 00 00 LHLD 10000h\n"
              L"--> Error: 3:14 - Value out of range for 16 bit value: 65536\n"
              L"              END\n", reportStream.str());
    EXPECT_EQ(L"Error: 2:14 - Value out of range for 16 bit value: 65536\nError: 3:14 - Value out of range for 16 bit value: 65536\n", errorStream.str());
    EXPECT_EQ(size_t{ 2 }, messages.size());
    EXPECT_EQ(size_t{ 2 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::SHLD, L"SHLD", L"10000h", 0x0000);
    node = node->Next();
    CheckOpcodeD16(node, OpcodeType::LHLD, L"LHLD", L"10000h", 0x0000);
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0x22, 0x00, 0x00, 0x2A, 0x00, 0x00 };
    MachineCode const & machineCode = parser.GetMachineCode();
    EXPECT_TRUE(Core::Util::Compare(ref, machineCode));
}

TEST_FIXTURE(ParserTest, ParseXCHG)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        XCHG\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    EXPECT_EQ(L"              CPU Intel8080\n"
              L"0000 EB       XCHG\n"
              L"              END\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
    CheckOpcode(node, OpcodeType::XCHG, L"XCHG");
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0xEB };
    MachineCode const & machineCode = parser.GetMachineCode();
    EXPECT_TRUE(Core::Util::Compare(ref, machineCode));
}

TEST_FIXTURE(ParserTest, ParsePUSH_POP)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        PUSH B\n"
                                   "        PUSH D\n"
                                   "        PUSH H\n"
                                   "        PUSH PSW\n"
                                   "        POP B\n"
                                   "        POP D\n"
                                   "        POP H\n"
                                   "        POP PSW\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    EXPECT_EQ(L"              CPU Intel8080\n"
              L"0000 C5       PUSH B\n"
              L"0001 D5       PUSH D\n"
              L"0002 E5       PUSH H\n"
              L"0003 F5       PUSH PSW\n"
              L"0004 C1       POP B\n"
              L"0005 D1       POP D\n"
              L"0006 E1       POP H\n"
              L"0007 F1       POP PSW\n"
              L"              END\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
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
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0xC5, 0xD5, 0xE5, 0xF5, 0xC1, 0xD1, 0xE1, 0xF1 };
    MachineCode const & machineCode = parser.GetMachineCode();
    EXPECT_TRUE(Core::Util::Compare(ref, machineCode));
}

TEST_FIXTURE(ParserTest, ParsePUSH_POPInvalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        PUSH SP\n"
                                   "        POP SP\n"
                                   "        PUSH X\n"
                                   "        POP X\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    EXPECT_EQ(L"              CPU Intel8080\n"
              L"0000          PUSH SP\n"
              L"--> Error: 2:9 - Opcode PUSH only supports B, D, H or PSW register pair, SP specified\n"
              L"0001          POP SP\n"
              L"--> Error: 3:9 - Opcode POP only supports B, D, H or PSW register pair, SP specified\n"
              L"0002          PUSH X\n"
              L"--> Error: 4:14 - Invalid register pair: X\n"
              L"0003          POP X\n"
              L"--> Error: 5:13 - Invalid register pair: X\n"
              L"              END\n", reportStream.str());
    EXPECT_EQ(L"Error: 2:9 - Opcode PUSH only supports B, D, H or PSW register pair, SP specified\nError: 3:9 - Opcode POP only supports B, D, H or PSW register pair, SP specified\n"
              L"Error: 4:14 - Invalid register pair: X\nError: 5:13 - Invalid register pair: X\n", errorStream.str());
    EXPECT_EQ(size_t{ 4 }, messages.size());
    EXPECT_EQ(size_t{ 4 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::PUSH, L"PUSH", Register16Type::SP, L"SP");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::POP, L"POP", Register16Type::SP, L"SP");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::PUSH, L"PUSH", Register16Type::Invalid, L"");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::POP, L"POP", Register16Type::Invalid, L"");
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref;
    MachineCode const & machineCode = parser.GetMachineCode();
    EXPECT_TRUE(Core::Util::Compare(ref, machineCode));
}

TEST_FIXTURE(ParserTest, ParseXTHL_SPHL)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        XTHL\n"
                                   "        SPHL\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    EXPECT_EQ(L"              CPU Intel8080\n"
              L"0000 E3       XTHL\n"
              L"0001 F9       SPHL\n"
              L"              END\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
    CheckOpcode(node, OpcodeType::XTHL, L"XTHL");
    node = node->Next();
    CheckOpcode(node, OpcodeType::SPHL, L"SPHL");
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0xE3, 0xF9 };
    MachineCode const & machineCode = parser.GetMachineCode();
    EXPECT_TRUE(Core::Util::Compare(ref, machineCode));
}

TEST_FIXTURE(ParserTest, ParseINX_DCX)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        INX B\n"
                                   "        INX D\n"
                                   "        INX H\n"
                                   "        INX SP\n"
                                   "        DCX B\n"
                                   "        DCX D\n"
                                   "        DCX H\n"
                                   "        DCX SP\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    /*EXPECT_EQ(L"CPU Intel8080\n"
              L"INX B\n"
              L"INX D\n"
              L"INX H\n"
              L"INX SP\n"
              L"DCX B\n"
              L"DCX D\n"
              L"DCX H\n"
              L"DCX SP\n"
              L"END\n", reportStream.str());*/
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
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
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseINX_DCXInvalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        INX PSW\n"
                                   "        DCX PSW\n"
                                   "        INX X\n"
                                   "        DCX X\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    /*EXPECT_EQ(L"CPU Intel8080\n"
              L"INX PSW\n"
              L"DCX PSW\n"
              L"INX X\n"
              L"DCX X\n"
              L"END\n", reportStream.str());*/
    EXPECT_EQ(L"Error: 2:9 - Opcode INX only supports B, D, H or SP register pair, PSW specified\nError: 3:9 - Opcode DCX only supports B, D, H or SP register pair, PSW specified\n"
              L"Error: 4:13 - Invalid register pair: X\nError: 5:13 - Invalid register pair: X\n", errorStream.str());
    EXPECT_EQ(size_t{ 4 }, messages.size());
    EXPECT_EQ(size_t{ 4 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::INX, L"INX", Register16Type::PSW, L"PSW");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::DCX, L"DCX", Register16Type::PSW, L"PSW");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::INX, L"INX", Register16Type::Invalid, L"");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::DCX, L"DCX", Register16Type::Invalid, L"");
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseJMP_JXX)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        JMP 0100h\n"
                                   "        JC 0101h\n"
                                   "        JNC 0102h\n"
                                   "        JZ 0103h\n"
                                   "        JNZ 0104h\n"
                                   "        JP 0105h\n"
                                   "        JM 0106h\n"
                                   "        JPE 0107h\n"
                                   "        JPO 0108h\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    /*EXPECT_EQ(L"CPU Intel8080\n"
              L"JMP 0100h\n"
              L"JC 0101h\n"
              L"JNC 0102h\n"
              L"JZ 0103h\n"
              L"JNZ 0104h\n"
              L"JP 0105h\n"
              L"JM 0106h\n"
              L"JPE 0107h\n"
              L"JPO 0108h\n"
              L"END\n", reportStream.str());*/
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
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
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParsePCHL)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        PCHL\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    /*EXPECT_EQ(L"CPU Intel8080\n"
              L"PCHL\n"
              L"END\n", reportStream.str());*/
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
    CheckOpcode(node, OpcodeType::PCHL, L"PCHL");
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseCALL_CXX)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        CALL 0100h\n"
                                   "        CC 0101h\n"
                                   "        CNC 0102h\n"
                                   "        CZ 0103h\n"
                                   "        CNZ 0104h\n"
                                   "        CP 0105h\n"
                                   "        CM 0106h\n"
                                   "        CPE 0107h\n"
                                   "        CPO 0108h\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    /*EXPECT_EQ(L"CPU Intel8080\n"
              L"CALL 0100h\n"
              L"CC 0101h\n"
              L"CNC 0102h\n"
              L"CZ 0103h\n"
              L"CNZ 0104h\n"
              L"CP 0105h\n"
              L"CM 0106h\n"
              L"CPE 0107h\n"
              L"CPO 0108h\n"
              L"END\n", reportStream.str());*/
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
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
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseRET_RXX)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        RET\n"
                                   "        RC\n"
                                   "        RNC\n"
                                   "        RZ\n"
                                   "        RNZ\n"
                                   "        RP\n"
                                   "        RM\n"
                                   "        RPE\n"
                                   "        RPO\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    /*EXPECT_EQ(L"CPU Intel8080\n"
              L"RET\n"
              L"RC\n"
              L"RNC\n"
              L"RZ\n"
              L"RNZ\n"
              L"RP\n"
              L"RM\n"
              L"RPE\n"
              L"RPO\n"
              L"END\n", reportStream.str());*/
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
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
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseRST)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        RST 0\n"
                                   "        RST 1\n"
                                   "        RST 2\n"
                                   "        RST 3\n"
                                   "        RST 4\n"
                                   "        RST 5\n"
                                   "        RST 6\n"
                                   "        RST 7\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    /*EXPECT_EQ(L"CPU Intel8080\n"
              L"RST 0\n"
              L"RST 1\n"
              L"RST 2\n"
              L"RST 3\n"
              L"RST 4\n"
              L"RST 5\n"
              L"RST 6\n"
              L"RST 7\n"
              L"END\n", reportStream.str());*/
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
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
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseRSTInvalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        RST 8\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    /*EXPECT_EQ(L"CPU Intel8080\n"
              L"RST 8\n"
              L"END\n", reportStream.str());*/
    EXPECT_EQ(L"Error: 2:13 - Invalid RST code: 8\n", errorStream.str());
    EXPECT_EQ(size_t{ 1 }, messages.size());
    EXPECT_EQ(size_t{ 1 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
    CheckOpcodeN(node, OpcodeType::RST, L"RST", RSTCode::Invalid, L"");
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseINR_DCR)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        INR B\n"
                                   "        INR C\n"
                                   "        INR D\n"
                                   "        INR E\n"
                                   "        INR H\n"
                                   "        INR L\n"
                                   "        INR A\n"
                                   "        INR M\n"
                                   "        DCR B\n"
                                   "        DCR C\n"
                                   "        DCR D\n"
                                   "        DCR E\n"
                                   "        DCR H\n"
                                   "        DCR L\n"
                                   "        DCR A\n"
                                   "        DCR M\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    /*EXPECT_EQ(L"CPU Intel8080\n"
              L"INR B\n"
              L"INR C\n"
              L"INR D\n"
              L"INR E\n"
              L"INR H\n"
              L"INR L\n"
              L"INR A\n"
              L"INR M\n"
              L"DCR B\n"
              L"DCR C\n"
              L"DCR D\n"
              L"DCR E\n"
              L"DCR H\n"
              L"DCR L\n"
              L"DCR A\n"
              L"DCR M\n"
              L"END\n", reportStream.str());*/
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
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
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseINR_DCRInvalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        INR X\n"
                                   "        DCR X\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    /*EXPECT_EQ(L"CPU Intel8080\n"
              L"INR X\n"
              L"DCR X\n"
              L"END\n", reportStream.str());*/
    EXPECT_EQ(L"Error: 2:13 - Invalid register: X\nError: 3:13 - Invalid register: X\n", errorStream.str());
    EXPECT_EQ(size_t{ 2 }, messages.size());
    EXPECT_EQ(size_t{ 2 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::INR, L"INR", Register8Type::Invalid, L"");
    node = node->Next();
    CheckOpcodeR8(node, OpcodeType::DCR, L"DCR", Register8Type::Invalid, L"");
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseADD_ADC_SUB_SBB_ANA_ORA_XRA_CMP)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        ADD B\n"
                                   "        ADD C\n"
                                   "        ADD D\n"
                                   "        ADD E\n"
                                   "        ADD H\n"
                                   "        ADD L\n"
                                   "        ADD A\n"
                                   "        ADD M\n"
                                   "        ADC B\n"
                                   "        ADC C\n"
                                   "        ADC D\n"
                                   "        ADC E\n"
                                   "        ADC H\n"
                                   "        ADC L\n"
                                   "        ADC A\n"
                                   "        ADC M\n"
                                   "        SUB B\n"
                                   "        SUB C\n"
                                   "        SUB D\n"
                                   "        SUB E\n"
                                   "        SUB H\n"
                                   "        SUB L\n"
                                   "        SUB A\n"
                                   "        SUB M\n"
                                   "        SBB B\n"
                                   "        SBB C\n"
                                   "        SBB D\n"
                                   "        SBB E\n"
                                   "        SBB H\n"
                                   "        SBB L\n"
                                   "        SBB A\n"
                                   "        SBB M\n"
                                   "        ANA B\n"
                                   "        ANA C\n"
                                   "        ANA D\n"
                                   "        ANA E\n"
                                   "        ANA H\n"
                                   "        ANA L\n"
                                   "        ANA A\n"
                                   "        ANA M\n"
                                   "        ORA B\n"
                                   "        ORA C\n"
                                   "        ORA D\n"
                                   "        ORA E\n"
                                   "        ORA H\n"
                                   "        ORA L\n"
                                   "        ORA A\n"
                                   "        ORA M\n"
                                   "        XRA B\n"
                                   "        XRA C\n"
                                   "        XRA D\n"
                                   "        XRA E\n"
                                   "        XRA H\n"
                                   "        XRA L\n"
                                   "        XRA A\n"
                                   "        XRA M\n"
                                   "        CMP B\n"
                                   "        CMP C\n"
                                   "        CMP D\n"
                                   "        CMP E\n"
                                   "        CMP H\n"
                                   "        CMP L\n"
                                   "        CMP A\n"
                                   "        CMP M\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    /*EXPECT_EQ(L"CPU Intel8080\n"
              L"ADD B\n"
              L"ADD C\n"
              L"ADD D\n"
              L"ADD E\n"
              L"ADD H\n"
              L"ADD L\n"
              L"ADD A\n"
              L"ADD M\n"
              L"ADC B\n"
              L"ADC C\n"
              L"ADC D\n"
              L"ADC E\n"
              L"ADC H\n"
              L"ADC L\n"
              L"ADC A\n"
              L"ADC M\n"
              L"SUB B\n"
              L"SUB C\n"
              L"SUB D\n"
              L"SUB E\n"
              L"SUB H\n"
              L"SUB L\n"
              L"SUB A\n"
              L"SUB M\n"
              L"SBB B\n"
              L"SBB C\n"
              L"SBB D\n"
              L"SBB E\n"
              L"SBB H\n"
              L"SBB L\n"
              L"SBB A\n"
              L"SBB M\n"
              L"ANA B\n"
              L"ANA C\n"
              L"ANA D\n"
              L"ANA E\n"
              L"ANA H\n"
              L"ANA L\n"
              L"ANA A\n"
              L"ANA M\n"
              L"ORA B\n"
              L"ORA C\n"
              L"ORA D\n"
              L"ORA E\n"
              L"ORA H\n"
              L"ORA L\n"
              L"ORA A\n"
              L"ORA M\n"
              L"XRA B\n"
              L"XRA C\n"
              L"XRA D\n"
              L"XRA E\n"
              L"XRA H\n"
              L"XRA L\n"
              L"XRA A\n"
              L"XRA M\n"
              L"CMP B\n"
              L"CMP C\n"
              L"CMP D\n"
              L"CMP E\n"
              L"CMP H\n"
              L"CMP L\n"
              L"CMP A\n"
              L"CMP M\n"
              L"END\n", reportStream.str());*/
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
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
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseADD_ADC_SUB_SBB_ANA_ORA_XRA_CMPInvalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        ADD X\n"
                                   "        ADC X\n"
                                   "        SUB X\n"
                                   "        SBB X\n"
                                   "        ANA X\n"
                                   "        ORA X\n"
                                   "        XRA X\n"
                                   "        CMP X\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    /*EXPECT_EQ(L"CPU Intel8080\n"
              L"ADD X\n"
              L"ADC X\n"
              L"SUB X\n"
              L"SBB X\n"
              L"ANA X\n"
              L"ORA X\n"
              L"XRA X\n"
              L"CMP X\n"
              L"END\n", reportStream.str());*/
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

    CheckCPU(node, L"Intel8080");
    node = node->Next();
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
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseADI_ACI_SUI_SBI_ANI_ORI_XRI_CPI)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        ADI 00h\n"
                                   "        ACI 01h\n"
                                   "        SUI 02h\n"
                                   "        SBI 03h\n"
                                   "        ANI 04h\n"
                                   "        ORI 05h\n"
                                   "        XRI 06h\n"
                                   "        CPI 07h\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    /*EXPECT_EQ(L"CPU Intel8080\n"
              L"ADI 00h\n"
              L"ACI 01h\n"
              L"SUI 02h\n"
              L"SBI 03h\n"
              L"ANI 04h\n"
              L"ORI 05h\n"
              L"XRI 06h\n"
              L"CPI 07h\n"
              L"END\n", reportStream.str());*/
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
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
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseDAD)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        DAD B\n"
                                   "        DAD D\n"
                                   "        DAD H\n"
                                   "        DAD SP\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    /*EXPECT_EQ(L"CPU Intel8080\n"
              L"DAD B\n"
              L"DAD D\n"
              L"DAD H\n"
              L"DAD SP\n"
              L"END\n", reportStream.str());*/
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::DAD, L"DAD", Register16Type::BC, L"B");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::DAD, L"DAD", Register16Type::DE, L"D");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::DAD, L"DAD", Register16Type::HL, L"H");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::DAD, L"DAD", Register16Type::SP, L"SP");
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseDADInvalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        DAD X\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    /*EXPECT_EQ(L"CPU Intel8080\n"
              L"DAD X\n"
              L"END\n", reportStream.str());*/
    EXPECT_EQ(L"Error: 2:13 - Invalid register pair: X\n", errorStream.str());
    EXPECT_EQ(size_t{ 1 }, messages.size());
    EXPECT_EQ(size_t{ 1 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
    CheckOpcodeR16(node, OpcodeType::DAD, L"DAD", Register16Type::Invalid, L"");
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseRLC_RRC_RAL_RAR_CMA_STC_CMC_DAA)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        RLC\n"
                                   "        RRC\n"
                                   "        RAL\n"
                                   "        RAR\n"
                                   "        CMA\n"
                                   "        STC\n"
                                   "        CMC\n"
                                   "        DAA\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    /*EXPECT_EQ(L"CPU Intel8080\n"
              L"RLC\n"
              L"RRC\n"
              L"RAL\n"
              L"RAR\n"
              L"CMA\n"
              L"STC\n"
              L"CMC\n"
              L"DAA\n"
              L"END\n", reportStream.str());*/
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
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
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseIN_OUT)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        IN 00h\n"
                                   "        OUT 01h\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    /*EXPECT_EQ(L"CPU Intel8080\n"
              L"IN 00h\n"
              L"OUT 01h\n"
              L"END\n", reportStream.str());*/
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
    CheckOpcodeD8(node, OpcodeType::INP, L"IN", L"00h", 0x00);
    node = node->Next();
    CheckOpcodeD8(node, OpcodeType::OUTP, L"OUT", L"01h", 0x01);
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseEI_DI_NOP_HLT)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "START:  ORG 0000\n"
                                   "        EI\n"
                                   "        DI\n"
                                   "NOP:    NOP\n"
                                   "        HLT\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    /*EXPECT_EQ(L"CPU Intel8080\n"
              L"START:  ORG 0000; Set initial address\n"
              L"EI\n"
              L"DI\n"
              L"NOP:    NOP\n"
              L"HLT\n"
              L"END\n", reportStream.str());*/
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
    CheckOrg(node, L"0000", L"START");
    node = node->Next();
    CheckOpcode(node, OpcodeType::EI, L"EI");
    node = node->Next();
    CheckOpcode(node, OpcodeType::DI, L"DI");
    node = node->Next();
    CheckOpcode(node, OpcodeType::NOP, L"NOP", L"NOP");
    node = node->Next();
    CheckOpcode(node, OpcodeType::HLT, L"HLT");
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, Parse8085Extensions)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8085\n"
                                   "        DSUB\n"
                                   "        ARHL\n"
                                   "        RDEL\n"
                                   "        RIM\n"
                                   "        LDHI 01h\n"
                                   "        SIM\n"
                                   "        LDSI 02h\n"
                                   "        RSTV\n"
                                   "        SHLX\n"
                                   "        JNK 0103h\n"
                                   "        LHLX\n"
                                   "        JK  0104h\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    /*EXPECT_EQ(L"CPU Intel8085; Set CPU type\n"
              L"DSUB\n"
              L"ARHL\n"
              L"RDEL\n"
              L"RIM\n"
              L"LDHI 01h\n"
              L"SIM\n"
              L"LDSI 02h\n"
              L"RSTV\n"
              L"SHLX\n"
              L"JNK 0103h\n"
              L"LHLX\n"
              L"JK  0104h\n"
              L"END\n", reportStream.str());*/
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8085");
    node = node->Next();
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
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, Parse8085ExtensionsOn8080)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        DSUB\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    /*EXPECT_EQ(L"CPU Intel8080\n"
              L"DSUB\n"
              L"END\n", reportStream.str());*/
    EXPECT_EQ(L"Error: 3:0 - Expected Opcode: DSUB\n", errorStream.str());
    EXPECT_EQ(size_t{ 1 }, messages.size());
    EXPECT_EQ(size_t{ 1 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseJMPWithLocationCounter)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        JMP $\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    /*EXPECT_EQ(L"CPU Intel8080\n"
              L"JMP $; \n"
              L"END\n", reportStream.str());*/
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
    CheckOpcodeLocCounter(node, OpcodeType::JMP, L"JMP");
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseJMPWithPredefinedLabel)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "START:  NOP \n"
                                   "        JMP START\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    /*EXPECT_EQ(L"CPU Intel8080\n"
              L"START:  NOP \n"
              L"JMP START; \n"
              L"END\n", reportStream.str());*/
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
    CheckOpcode(node, OpcodeType::NOP, L"NOP", L"START");
    node = node->Next();
    CheckOpcodeRef(node, OpcodeType::JMP, L"JMP", L"START");
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(ParserTest, ParseJMPWithUndefinedLabel)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        JMP START\n"
                                   "START:  NOP\n"
                                   "        END\n");
    std::wostringstream reportStream;
    std::wostringstream errorStream;
    Scanner scanner(&inputStream, true);
    Parser parser(scanner, messages, errorStream, reportStream);

    parser.Parse();

    EXPECT_EQ(L"CPU Intel8080\n"
              L"JMP START\n"
              L"START:NOP\n"
              L"END\n", reportStream.str());
    EXPECT_EQ(L"", errorStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = ast.FirstNode();

    CheckCPU(node, L"Intel8080");
    node = node->Next();
    CheckOpcodeRef(node, OpcodeType::JMP, L"JMP", L"START");
    node = node->Next();
    CheckOpcode(node, OpcodeType::NOP, L"NOP", L"START");
    node = node->Next();
    CheckEnd(node);

    EXPECT_NULL(node->Next());
}

} // namespace Test

} // namespace Assembler
