#include "unit-test-c++/UnitTestC++.h"

#include "assembler/Parser.h"
#include "assembler/AbstractSyntaxTree.h"
#include "assembler/CPUParserIntel8080_8085.h"

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

void CheckComment(StatementLineNode::Ptr node, std::wstring const & value)
{
    ASSERT_NOT_NULL(node);
    CommentNode::Ptr comment = node->Comment();
    if (value.empty())
        EXPECT_NULL(comment);
    else
    {
        ASSERT_NOT_NULL(comment);
        EXPECT_EQ(ASTNodeType::Comment, comment->NodeType());
        EXPECT_EQ(value, comment->Value());
        EXPECT_NULL(comment->FirstChild());
        EXPECT_NULL(comment->Next());
    }
}

void CheckLabel(StatementLineNode::Ptr node, std::wstring const & value)
{
    ASSERT_NOT_NULL(node);
    LabelNode::Ptr label = node->Label();
    if (value.empty())
        EXPECT_NULL(label);
    else
    {
        ASSERT_NOT_NULL(label);
        EXPECT_EQ(ASTNodeType::Label, label->NodeType());
        EXPECT_EQ(value, label->Value());
        EXPECT_NULL(label->FirstChild());
        EXPECT_NULL(label->Next());
    }
}

void CheckCPU(StatementLineNode::Ptr node, std::wstring const & value, std::wstring const & commentValue = L"")
{
    ASSERT_NOT_NULL(node);
    StatementNode::Ptr statement = node->Statement();
    ASSERT_NOT_NULL(statement);
    EXPECT_EQ(ASTNodeType::CPU, statement->NodeType());
    EXPECT_EQ(value, statement->Value());
    CheckComment(node, commentValue);
    EXPECT_NULL(statement->FirstChild());
    EXPECT_NULL(statement->Next());
}

void CheckOrg(StatementLineNode::Ptr node, std::wstring const & value, std::wstring const & labelValue = L"", std::wstring const & commentValue = L"")
{
    ASSERT_NOT_NULL(node);
    StatementNode::Ptr statement = node->Statement();
    ASSERT_NOT_NULL(statement);
    EXPECT_EQ(ASTNodeType::ORG, statement->NodeType());
    EXPECT_EQ(value, statement->Value());
    CheckLabel(node, labelValue);
    CheckComment(node, commentValue);
    EXPECT_NULL(statement->FirstChild());
    EXPECT_NULL(statement->Next());
}

void CheckEnd(StatementLineNode::Ptr node, std::wstring const & labelValue = L"", std::wstring const & commentValue = L"")
{
    ASSERT_NOT_NULL(node);
    StatementNode::Ptr statement = node->Statement();
    ASSERT_NOT_NULL(statement);
    EXPECT_EQ(ASTNodeType::END, statement->NodeType());
    EXPECT_EQ(L"END", statement->Value());
    CheckLabel(node, labelValue);
    CheckComment(node, commentValue);
    EXPECT_NULL(statement->FirstChild());
    EXPECT_NULL(statement->Next());
}

void CheckOpcodeInvalid(StatementLineNode::Ptr node, std::wstring const & opcode, std::wstring const & labelValue = L"", std::wstring const & commentValue = L"", std::wstring const & operand = L"")
{
    ASSERT_NOT_NULL(node);
    StatementNode::Ptr statement = node->Statement();
    ASSERT_NOT_NULL(statement);
    EXPECT_EQ(ASTNodeType::Opcode, statement->NodeType());
    EXPECT_EQ(opcode, statement->Value());
    OpcodeNode<OpcodeType, AddressType> * opcodeNode = dynamic_cast<OpcodeNode<OpcodeType, AddressType> *>(statement.get());
    ASSERT_NOT_NULL(opcodeNode);
    EXPECT_EQ(OpcodeType::Invalid, opcodeNode->Type());
    CheckLabel(node, labelValue);
    CheckComment(node, commentValue);
    if (operand.empty())
        ASSERT_NULL(opcodeNode->FirstChild());
    else
        EXPECT_EQ(operand, opcodeNode->FirstChild()->Value());
    EXPECT_NULL(statement->Next());
}

void CheckOpcode(StatementLineNode::Ptr node, OpcodeType opcodeType, std::wstring const & opcode, std::wstring const & labelValue = L"", std::wstring const & commentValue = L"")
{
    ASSERT_NOT_NULL(node);
    StatementNode::Ptr statement = node->Statement();
    ASSERT_NOT_NULL(statement);
    EXPECT_EQ(ASTNodeType::Opcode, statement->NodeType());
    EXPECT_EQ(opcode, statement->Value());
    OpcodeNode<OpcodeType, AddressType> * opcodeNode = dynamic_cast<OpcodeNode<OpcodeType, AddressType> *>(statement.get());
    ASSERT_NOT_NULL(opcodeNode);
    EXPECT_EQ(opcodeType, opcodeNode->Type());
    CheckLabel(node, labelValue);
    CheckComment(node, commentValue);
    EXPECT_NULL(statement->Next());
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

void CheckOpcodeR8R8(StatementLineNode::Ptr node, OpcodeType opcodeType, std::wstring const & opcode, 
                     Register8Type dddType, std::wstring const & ddd, 
                     Register8Type sssType, std::wstring const & sss, std::wstring const & labelValue = L"")
{
    ASSERT_NOT_NULL(node);
    CheckOpcode(node, opcodeType, opcode, labelValue);
    StatementNode::Ptr statement = node->Statement();
    ASSERT_NOT_NULL(statement);

    Register8Node<Register8Type>::Ptr dstReg = statement->FirstChild();
    CheckRegister8(dstReg, dddType, ddd);
    Register8Node<Register8Type>::Ptr srcReg = dstReg->Next();
    CheckRegister8(srcReg, sssType, sss);
}

void CheckOpcodeR16(StatementLineNode::Ptr node, OpcodeType opcodeType, std::wstring const & opcode, 
                    Register16Type rpType, std::wstring const & rp, std::wstring const & labelValue = L"")
{
    ASSERT_NOT_NULL(node);
    CheckOpcode(node, opcodeType, opcode, labelValue);
    StatementNode::Ptr statement = node->Statement();
    ASSERT_NOT_NULL(statement);

    Register16Node<Register16Type>::Ptr reg = statement->FirstChild();
    if (rpType != Register16Type::Invalid)
        CheckRegister16(reg, rpType, rp);
}

void CheckOpcodeR8(StatementLineNode::Ptr node, OpcodeType opcodeType, std::wstring const & opcode, 
                   Register8Type rType, std::wstring const & r, std::wstring const & labelValue = L"")
{
    ASSERT_NOT_NULL(node);
    CheckOpcode(node, opcodeType, opcode, labelValue);
    StatementNode::Ptr statement = node->Statement();
    ASSERT_NOT_NULL(statement);

    Register8Node<Register8Type>::Ptr reg = statement->FirstChild();
    if (rType != Register8Type::Invalid)
        CheckRegister8(reg, rType, r);
}

void CheckOpcodeR8D8(StatementLineNode::Ptr node, OpcodeType opcodeType, std::wstring const & opcode, 
                     Register8Type rType, std::wstring const & r, std::wstring const & data8Str, uint8_t data8, std::wstring const & labelValue = L"")
{
    ASSERT_NOT_NULL(node);
    CheckOpcode(node, opcodeType, opcode, labelValue);
    StatementNode::Ptr statement = node->Statement();
    ASSERT_NOT_NULL(statement);

    Register8Node<Register8Type>::Ptr reg = statement->FirstChild();
    if (rType != Register8Type::Invalid)
        CheckRegister8(reg, rType, r);
    Data8Node::Ptr data = reg->Next();
    if (!data8Str.empty())
        CheckData8(data, data8Str, data8);
}

void CheckOpcodeR16D16(StatementLineNode::Ptr node, OpcodeType opcodeType, std::wstring const & opcode, 
                       Register16Type rType, std::wstring const & r, std::wstring const & data16Str, uint16_t data16, std::wstring const & labelValue = L"")
{
    ASSERT_NOT_NULL(node);
    CheckOpcode(node, opcodeType, opcode, labelValue);
    StatementNode::Ptr statement = node->Statement();
    ASSERT_NOT_NULL(statement);

    Register16Node<Register16Type>::Ptr reg = statement->FirstChild();
    if (rType != Register16Type::Invalid)
        CheckRegister16(reg, rType, r);
    Data16Node::Ptr data = reg->Next();
    if (!data16Str.empty())
        CheckData16(data, data16Str, data16);
}

void CheckOpcodeD8(StatementLineNode::Ptr node, OpcodeType opcodeType, std::wstring const & opcode, 
                   std::wstring const & data8Str, uint8_t data8, std::wstring const & labelValue = L"")
{
    ASSERT_NOT_NULL(node);
    CheckOpcode(node, opcodeType, opcode, labelValue);
    StatementNode::Ptr statement = node->Statement();
    ASSERT_NOT_NULL(statement);

    Data8Node::Ptr data = statement->FirstChild();
    if (!data8Str.empty())
        CheckData8(data, data8Str, data8);
}

void CheckOpcodeD16(StatementLineNode::Ptr node, OpcodeType opcodeType, std::wstring const & opcode, 
                    std::wstring const & data16Str, uint16_t data16, std::wstring const & labelValue = L"")
{
    ASSERT_NOT_NULL(node);
    CheckOpcode(node, opcodeType, opcode, labelValue);
    StatementNode::Ptr statement = node->Statement();
    ASSERT_NOT_NULL(statement);

    Data16Node::Ptr data = statement->FirstChild();
    if (!data16Str.empty())
        CheckData16(data, data16Str, data16);
}

void CheckOpcodeLocCounter(StatementLineNode::Ptr node, OpcodeType opcodeType, std::wstring const & opcode, 
                           std::wstring const & labelValue = L"")
{
    ASSERT_NOT_NULL(node);
    CheckOpcode(node, opcodeType, opcode, labelValue);
    StatementNode::Ptr statement = node->Statement();
    ASSERT_NOT_NULL(statement);

    ASTNode::Ptr childNode = statement->FirstChild();
    ASSERT_NOT_NULL(childNode);
    ExpressionNode * expressionNode = dynamic_cast<ExpressionNode *>(childNode.get());
    ASTNode::Ptr child2Node = childNode->FirstChild();
    ASSERT_NOT_NULL(child2Node);
    LocCounterNode<AddressType> * locCounterNode = dynamic_cast<LocCounterNode<AddressType> *>(child2Node.get());
    ASSERT_NOT_NULL(locCounterNode);
}

void CheckOpcodeRef(StatementLineNode::Ptr node, OpcodeType opcodeType, std::wstring const & opcode, 
                    std::wstring const & ref = L"", std::wstring const & labelValue = L"")
{
    ASSERT_NOT_NULL(node);
    CheckOpcode(node, opcodeType, opcode, labelValue);
    StatementNode::Ptr statement = node->Statement();
    ASSERT_NOT_NULL(statement);

    ASTNode::Ptr childNode = statement->FirstChild();
    ASSERT_NOT_NULL(childNode);
    ExpressionNode * expressionNode = dynamic_cast<ExpressionNode *>(childNode.get());
    ASTNode::Ptr child2Node = childNode->FirstChild();
    ASSERT_NOT_NULL(child2Node);
    RefAddressNode<SegmentType, AddressType> * refNode = dynamic_cast<RefAddressNode<SegmentType, AddressType> *>(child2Node.get());
    ASSERT_NOT_NULL(refNode);
    EXPECT_EQ(ref, refNode->Value());
}

void CheckOpcodeN(StatementLineNode::Ptr node, OpcodeType opcodeType, std::wstring const & opcode, 
                  RSTCode rstCode, std::wstring const & code, std::wstring const & labelValue = L"")
{
    ASSERT_NOT_NULL(node);
    CheckOpcode(node, opcodeType, opcode, labelValue);
    StatementNode::Ptr statement = node->Statement();
    ASSERT_NOT_NULL(statement);

    RSTNode::Ptr rstNode = statement->FirstChild();
    if (rstCode != RSTCode::Invalid)
        CheckRSTCode(rstNode, rstCode, code);
}

void CheckCode(MachineCode const & ref, Parser const & parser, AddressType offset = 0)
{
    MachineCode machineCode;
    if (parser.GetObjectCode().HaveSegment(SegmentID::ASEG))
    {
        machineCode = parser.GetObjectCode().GetSegment(SegmentID::ASEG).Data();
        EXPECT_EQ(offset, parser.GetObjectCode().GetSegment(SegmentID::ASEG).Offset());
    }
    EXPECT_TRUE(Core::Util::Compare(ref, machineCode));
}

TEST_FIXTURE(ParserTest, ConstructDefault)
{
    AssemblerMessages messages;
    std::istringstream inputStream;
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    EXPECT_EQ(L"", reportStream.str());
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
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);
    parser.Parse();

    EXPECT_EQ(L"Error: 1:1 - \"CPU\" expected\nError: 1:1 - Identifier expected\n", reportStream.str());
    EXPECT_EQ(size_t{ 2 }, messages.size());
    EXPECT_EQ(size_t{ 2 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    EXPECT_THROW(parser.GetAST(), AssemblerException);

    MachineCode ref;
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParseEnd)
{
    AssemblerMessages messages;
    std::istringstream inputStream("End");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);
    parser.Parse();

    EXPECT_EQ(L"Error: 1:1 - \"CPU\" expected\nError: 1:1 - Identifier expected\n", reportStream.str());
    EXPECT_EQ(size_t{ 2 }, messages.size());
    EXPECT_EQ(size_t{ 2 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    EXPECT_THROW(parser.GetAST(), AssemblerException);

    MachineCode ref;
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParseSimple1)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080 ; Set CPU type\n"
                                   "START:  ORG 0000      ; Set initial address\n"
                                   "        END           ; Stop\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080       ; Set CPU type\n"
              L"0000          START:              ORG 0000            ; Set initial address\n"
              L"                                  END                 ; Stop\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());
    CheckCPU(node, L"Intel8080", L" Set CPU type");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOrg(node, L"0000", L"START", L" Set initial address");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node, L"", L" Stop");

    EXPECT_NULL(node->Next());

    MachineCode ref;
    CheckCode(ref, parser);
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
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 7F                           MOV A,A\n"
              L"0001 78                           MOV A,B\n"
              L"0002 79                           MOV A,C\n"
              L"0003 7A                           MOV A,D\n"
              L"0004 7B                           MOV A,E\n"
              L"0005 7C                           MOV A,H\n"
              L"0006 7D                           MOV A,L\n"
              L"0007 47                           MOV B,A\n"
              L"0008 4F                           MOV C,A\n"
              L"0009 57                           MOV D,A\n"
              L"000A 5F                           MOV E,A\n"
              L"000B 67                           MOV H,A\n"
              L"000C 6F                           MOV L,A\n"
              L"000D 7E                           MOV A,M\n"
              L"000E 77                           MOV M,A\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8R8(node, OpcodeType::MOV, L"MOV", Register8Type::A, L"A", Register8Type::A, L"A");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8R8(node, OpcodeType::MOV, L"MOV", Register8Type::A, L"A", Register8Type::B, L"B");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8R8(node, OpcodeType::MOV, L"MOV", Register8Type::A, L"A", Register8Type::C, L"C");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8R8(node, OpcodeType::MOV, L"MOV", Register8Type::A, L"A", Register8Type::D, L"D");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8R8(node, OpcodeType::MOV, L"MOV", Register8Type::A, L"A", Register8Type::E, L"E");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8R8(node, OpcodeType::MOV, L"MOV", Register8Type::A, L"A", Register8Type::H, L"H");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8R8(node, OpcodeType::MOV, L"MOV", Register8Type::A, L"A", Register8Type::L, L"L");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8R8(node, OpcodeType::MOV, L"MOV", Register8Type::B, L"B", Register8Type::A, L"A");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8R8(node, OpcodeType::MOV, L"MOV", Register8Type::C, L"C", Register8Type::A, L"A");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8R8(node, OpcodeType::MOV, L"MOV", Register8Type::D, L"D", Register8Type::A, L"A");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8R8(node, OpcodeType::MOV, L"MOV", Register8Type::E, L"E", Register8Type::A, L"A");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8R8(node, OpcodeType::MOV, L"MOV", Register8Type::H, L"H", Register8Type::A, L"A");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8R8(node, OpcodeType::MOV, L"MOV", Register8Type::L, L"L", Register8Type::A, L"A");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8R8(node, OpcodeType::MOV, L"MOV", Register8Type::A, L"A", Register8Type::M, L"M");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8R8(node, OpcodeType::MOV, L"MOV", Register8Type::M, L"M", Register8Type::A, L"A");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0x7F, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x47, 0x4F, 0x57, 0x5F, 0x67, 0x6F, 0x7E, 0x77};
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParseMOV_Invalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        MOV M,M\n"
                                   "        MOV X,A\n"
                                   "        MOV A,X\n"
                                   "        END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000                              MOV M,M\n"
              L"--> Error: 2:9 - Incorrect opcode: MOV M,M\n"
              L"0001                              MOV X,A\n"
              L"--> Error: 3:13 - Invalid destination register: X\n"
              L"0002                              MOV A,X\n"
              L"--> Error: 4:15 - Invalid source register: X\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 3 }, messages.size());
    EXPECT_EQ(size_t{ 3 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8R8(node, OpcodeType::MOV, L"MOV", Register8Type::M, L"M", Register8Type::M, L"M");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8R8(node, OpcodeType::MOV, L"MOV", Register8Type::Invalid, L"X", Register8Type::A, L"A");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8R8(node, OpcodeType::MOV, L"MOV", Register8Type::A, L"A", Register8Type::Invalid, L"X");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref;
    CheckCode(ref, parser);
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
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 06 00                        MVI B,00h\n"
              L"0002 0E 01                        MVI C,01h\n"
              L"0004 16 02                        MVI D,02h\n"
              L"0006 1E 03                        MVI E,03h\n"
              L"0008 26 04                        MVI H,04h\n"
              L"000A 2E 05                        MVI L,05h\n"
              L"000C 3E 06                        MVI A,06h\n"
              L"000E 36 07                        MVI M,07h\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8D8(node, OpcodeType::MVI, L"MVI", Register8Type::B, L"B", L"00h", 0);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8D8(node, OpcodeType::MVI, L"MVI", Register8Type::C, L"C", L"01h", 1);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8D8(node, OpcodeType::MVI, L"MVI", Register8Type::D, L"D", L"02h", 2);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8D8(node, OpcodeType::MVI, L"MVI", Register8Type::E, L"E", L"03h", 3);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8D8(node, OpcodeType::MVI, L"MVI", Register8Type::H, L"H", L"04h", 4);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8D8(node, OpcodeType::MVI, L"MVI", Register8Type::L, L"L", L"05h", 5);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8D8(node, OpcodeType::MVI, L"MVI", Register8Type::A, L"A", L"06h", 6);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8D8(node, OpcodeType::MVI, L"MVI", Register8Type::M, L"M", L"07h", 7);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0x06, 0x00, 0x0E, 0x01, 0x16, 0x02, 0x1E, 0x03, 0x26, 0x04, 0x2E, 0x05, 0x3E, 0x06, 0x36, 0x07 };
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParseMVI_Invalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        MVI X, 08h\n"
                                   "        END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000                              MVI X,08h\n"
              L"--> Error: 2:13 - Invalid register: X\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 1 }, messages.size());
    EXPECT_EQ(size_t{ 1 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8D8(node, OpcodeType::MVI, L"MVI", Register8Type::Invalid, L"", L"08h", 8);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref;
    CheckCode(ref, parser);
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
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 01 00 01                     LXI B,0100h\n"
              L"0003 11 02 03                     LXI D,0302h\n"
              L"0006 21 04 05                     LXI H,0504h\n"
              L"0009 31 06 07                     LXI SP,0706h\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16D16(node, OpcodeType::LXI, L"LXI", Register16Type::BC, L"B", L"0100h", 0x0100);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16D16(node, OpcodeType::LXI, L"LXI", Register16Type::DE, L"D", L"0302h", 0x0302);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16D16(node, OpcodeType::LXI, L"LXI", Register16Type::HL, L"H", L"0504h", 0x0504);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16D16(node, OpcodeType::LXI, L"LXI", Register16Type::SP, L"SP", L"0706h", 0x0706);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0x01, 0x00, 0x01, 0x11, 0x02, 0x03, 0x21, 0x04, 0x05, 0x31, 0x06, 0x07 };
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParseLXI_Invalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        LXI PSW, 0101h\n"
                                   "        LXI X, 0101h\n"
                                   "        END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000                              LXI PSW,0101h\n"
              L"--> Error: 2:9 - Opcode LXI only supports B, D, H or SP register pair, PSW specified\n"
              L"0003                              LXI X,0101h\n"
              L"--> Error: 3:13 - Invalid register pair: X\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 2 }, messages.size());
    EXPECT_EQ(size_t{ 2 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16D16(node, OpcodeType::LXI, L"LXI", Register16Type::PSW, L"PSW", L"0101h", 0x0101);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16D16(node, OpcodeType::LXI, L"LXI", Register16Type::Invalid, L"", L"0101h", 0x0101);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref;
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParseLXIInvalid2)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        LXI D, 10000h\n"
                                   "        END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 11 00 00                     LXI D,10000h\n"
              L"--> Error: 2:16 - Value out of range for 16 bit value: 65536\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 1 }, messages.size());
    EXPECT_EQ(size_t{ 1 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16D16(node, OpcodeType::LXI, L"LXI", Register16Type::DE, L"D", L"10000h", 0x0000);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0x11, 0x00, 0x00 };
    CheckCode(ref, parser);
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
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 02                           STAX B\n"
              L"0001 12                           STAX D\n"
              L"0002 0A                           LDAX B\n"
              L"0003 1A                           LDAX D\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::STAX, L"STAX", Register16Type::BC, L"B");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::STAX, L"STAX", Register16Type::DE, L"D");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::LDAX, L"LDAX", Register16Type::BC, L"B");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::LDAX, L"LDAX", Register16Type::DE, L"D");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0x02, 0x12, 0x0A, 0x1A };
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParseLDAX_STAX_Invalid)
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
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000                              STAX H\n"
              L"--> Error: 2:9 - Opcode STAX only supports B or D register pair, H specified\n"
              L"0001                              STAX SP\n"
              L"--> Error: 3:9 - Opcode STAX only supports B or D register pair, SP specified\n"
              L"0002                              LDAX H\n"
              L"--> Error: 4:9 - Opcode LDAX only supports B or D register pair, H specified\n"
              L"0003                              LDAX SP\n"
              L"--> Error: 5:9 - Opcode LDAX only supports B or D register pair, SP specified\n"
              L"0004                              STAX X\n"
              L"--> Error: 6:14 - Invalid register pair: X\n"
              L"0005                              LDAX X\n"
              L"--> Error: 7:14 - Invalid register pair: X\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 6 }, messages.size());
    EXPECT_EQ(size_t{ 6 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::STAX, L"STAX", Register16Type::HL, L"H");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::STAX, L"STAX", Register16Type::SP, L"SP");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::LDAX, L"LDAX", Register16Type::HL, L"H");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::LDAX, L"LDAX", Register16Type::SP, L"SP");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::STAX, L"STAX", Register16Type::Invalid, L"");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::LDAX, L"LDAX", Register16Type::Invalid, L"");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref;
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParseSTA_LDA)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        STA 0100h\n"
                                   "        LDA 0302h\n"
                                   "        END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 32 00 01                     STA 0100h\n"
              L"0003 3A 02 03                     LDA 0302h\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::STA, L"STA", L"0100h", 0x0100);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::LDA, L"LDA", L"0302h", 0x0302);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0x32, 0x00, 0x01, 0x3A, 0x02, 0x03 };
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParseSTA_LDA_Invalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        STA 10000h\n"
                                   "        LDA 10000h\n"
                                   "        END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 32 00 00                     STA 10000h\n"
              L"--> Error: 2:13 - Value out of range for 16 bit value: 65536\n"
              L"0003 3A 00 00                     LDA 10000h\n"
              L"--> Error: 3:13 - Value out of range for 16 bit value: 65536\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 2 }, messages.size());
    EXPECT_EQ(size_t{ 2 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::STA, L"STA", L"10000h", 0x0000);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::LDA, L"LDA", L"10000h", 0x0000);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0x32, 0x00, 0x00, 0x3A, 0x00, 0x00 };
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParseSHLD_LHLD)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        SHLD 0100h\n"
                                   "        LHLD 0302h\n"
                                   "        END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 22 00 01                     SHLD 0100h\n"
              L"0003 2A 02 03                     LHLD 0302h\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::SHLD, L"SHLD", L"0100h", 0x0100);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::LHLD, L"LHLD", L"0302h", 0x0302);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0x22, 0x00, 0x01, 0x2A, 0x02, 0x03 };
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParseSHLD_LHLD_Invalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        SHLD 10000h\n"
                                   "        LHLD 10000h\n"
                                   "        END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 22 00 00                     SHLD 10000h\n"
              L"--> Error: 2:14 - Value out of range for 16 bit value: 65536\n"
              L"0003 2A 00 00                     LHLD 10000h\n"
              L"--> Error: 3:14 - Value out of range for 16 bit value: 65536\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 2 }, messages.size());
    EXPECT_EQ(size_t{ 2 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::SHLD, L"SHLD", L"10000h", 0x0000);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::LHLD, L"LHLD", L"10000h", 0x0000);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0x22, 0x00, 0x00, 0x2A, 0x00, 0x00 };
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParseXCHG)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        XCHG\n"
                                   "        END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 EB                           XCHG\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::XCHG, L"XCHG");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0xEB };
    CheckCode(ref, parser);
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
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 C5                           PUSH B\n"
              L"0001 D5                           PUSH D\n"
              L"0002 E5                           PUSH H\n"
              L"0003 F5                           PUSH PSW\n"
              L"0004 C1                           POP B\n"
              L"0005 D1                           POP D\n"
              L"0006 E1                           POP H\n"
              L"0007 F1                           POP PSW\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::PUSH, L"PUSH", Register16Type::BC, L"B");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::PUSH, L"PUSH", Register16Type::DE, L"D");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::PUSH, L"PUSH", Register16Type::HL, L"H");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::PUSH, L"PUSH", Register16Type::PSW, L"PSW");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::POP, L"POP", Register16Type::BC, L"B");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::POP, L"POP", Register16Type::DE, L"D");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::POP, L"POP", Register16Type::HL, L"H");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::POP, L"POP", Register16Type::PSW, L"PSW");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0xC5, 0xD5, 0xE5, 0xF5, 0xC1, 0xD1, 0xE1, 0xF1 };
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParsePUSH_POP_Invalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        PUSH SP\n"
                                   "        POP SP\n"
                                   "        PUSH X\n"
                                   "        POP X\n"
                                   "        END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000                              PUSH SP\n"
              L"--> Error: 2:9 - Opcode PUSH only supports B, D, H or PSW register pair, SP specified\n"
              L"0001                              POP SP\n"
              L"--> Error: 3:9 - Opcode POP only supports B, D, H or PSW register pair, SP specified\n"
              L"0002                              PUSH X\n"
              L"--> Error: 4:14 - Invalid register pair: X\n"
              L"0003                              POP X\n"
              L"--> Error: 5:13 - Invalid register pair: X\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 4 }, messages.size());
    EXPECT_EQ(size_t{ 4 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::PUSH, L"PUSH", Register16Type::SP, L"SP");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::POP, L"POP", Register16Type::SP, L"SP");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::PUSH, L"PUSH", Register16Type::Invalid, L"");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::POP, L"POP", Register16Type::Invalid, L"");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref;
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParseXTHL_SPHL)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        XTHL\n"
                                   "        SPHL\n"
                                   "        END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 E3                           XTHL\n"
              L"0001 F9                           SPHL\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::XTHL, L"XTHL");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::SPHL, L"SPHL");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0xE3, 0xF9 };
    CheckCode(ref, parser);
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
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 03                           INX B\n"
              L"0001 13                           INX D\n"
              L"0002 23                           INX H\n"
              L"0003 33                           INX SP\n"
              L"0004 0B                           DCX B\n"
              L"0005 1B                           DCX D\n"
              L"0006 2B                           DCX H\n"
              L"0007 3B                           DCX SP\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::INX, L"INX", Register16Type::BC, L"B");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::INX, L"INX", Register16Type::DE, L"D");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::INX, L"INX", Register16Type::HL, L"H");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::INX, L"INX", Register16Type::SP, L"SP");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::DCX, L"DCX", Register16Type::BC, L"B");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::DCX, L"DCX", Register16Type::DE, L"D");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::DCX, L"DCX", Register16Type::HL, L"H");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::DCX, L"DCX", Register16Type::SP, L"SP");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0x03, 0x13, 0x23, 0x33, 0x0B, 0x1B, 0x2B, 0x3B };
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParseINX_DCX_Invalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        INX PSW\n"
                                   "        DCX PSW\n"
                                   "        INX X\n"
                                   "        DCX X\n"
                                   "        END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000                              INX PSW\n"
              L"--> Error: 2:9 - Opcode INX only supports B, D, H or SP register pair, PSW specified\n"
              L"0001                              DCX PSW\n"
              L"--> Error: 3:9 - Opcode DCX only supports B, D, H or SP register pair, PSW specified\n"
              L"0002                              INX X\n"
              L"--> Error: 4:13 - Invalid register pair: X\n"
              L"0003                              DCX X\n"
              L"--> Error: 5:13 - Invalid register pair: X\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 4 }, messages.size());
    EXPECT_EQ(size_t{ 4 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::INX, L"INX", Register16Type::PSW, L"PSW");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::DCX, L"DCX", Register16Type::PSW, L"PSW");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::INX, L"INX", Register16Type::Invalid, L"");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::DCX, L"DCX", Register16Type::Invalid, L"");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref;
    CheckCode(ref, parser);
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
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 C3 00 01                     JMP 0100h\n"
              L"0003 DA 01 01                     JC 0101h\n"
              L"0006 D2 02 01                     JNC 0102h\n"
              L"0009 CA 03 01                     JZ 0103h\n"
              L"000C C2 04 01                     JNZ 0104h\n"
              L"000F F2 05 01                     JP 0105h\n"
              L"0012 FA 06 01                     JM 0106h\n"
              L"0015 EA 07 01                     JPE 0107h\n"
              L"0018 E2 08 01                     JPO 0108h\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::JMP, L"JMP", L"0100h", 0x0100);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::JC, L"JC", L"0101h", 0x0101);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::JNC, L"JNC", L"0102h", 0x0102);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::JZ, L"JZ", L"0103h", 0x0103);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::JNZ, L"JNZ", L"0104h", 0x0104);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::JP, L"JP", L"0105h", 0x0105);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::JM, L"JM", L"0106h", 0x0106);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::JPE, L"JPE", L"0107h", 0x0107);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::JPO, L"JPO", L"0108h", 0x0108);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0xC3, 0x00, 0x01, 0xDA, 0x01, 0x01, 0xD2, 0x02, 0x01,
                     0xCA, 0x03, 0x01, 0xC2, 0x04, 0x01, 0xF2, 0x05, 0x01,
                     0xFA, 0x06, 0x01, 0xEA, 0x07, 0x01, 0xE2, 0x08, 0x01 };
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParseJMP_JXX_Invalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        JMP 10000h\n"
                                   "        JC 10000h\n"
                                   "        JNC 10000h\n"
                                   "        JZ 10000h\n"
                                   "        JNZ 10000h\n"
                                   "        JP 10000h\n"
                                   "        JM 10000h\n"
                                   "        JPE 10000h\n"
                                   "        JPO 10000h\n"
                                   "        END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 C3 00 00                     JMP 10000h\n"
              L"--> Error: 2:13 - Value out of range for 16 bit value: 65536\n"
              L"0003 DA 00 00                     JC 10000h\n"
              L"--> Error: 3:12 - Value out of range for 16 bit value: 65536\n"
              L"0006 D2 00 00                     JNC 10000h\n"
              L"--> Error: 4:13 - Value out of range for 16 bit value: 65536\n"
              L"0009 CA 00 00                     JZ 10000h\n"
              L"--> Error: 5:12 - Value out of range for 16 bit value: 65536\n"
              L"000C C2 00 00                     JNZ 10000h\n"
              L"--> Error: 6:13 - Value out of range for 16 bit value: 65536\n"
              L"000F F2 00 00                     JP 10000h\n"
              L"--> Error: 7:12 - Value out of range for 16 bit value: 65536\n"
              L"0012 FA 00 00                     JM 10000h\n"
              L"--> Error: 8:12 - Value out of range for 16 bit value: 65536\n"
              L"0015 EA 00 00                     JPE 10000h\n"
              L"--> Error: 9:13 - Value out of range for 16 bit value: 65536\n"
              L"0018 E2 00 00                     JPO 10000h\n"
              L"--> Error: 10:13 - Value out of range for 16 bit value: 65536\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 9 }, messages.size());
    EXPECT_EQ(size_t{ 9 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::JMP, L"JMP", L"10000h", 0x0000);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::JC, L"JC", L"10000h", 0x0000);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::JNC, L"JNC", L"10000h", 0x0000);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::JZ, L"JZ", L"10000h", 0x0000);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::JNZ, L"JNZ", L"10000h", 0x0000);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::JP, L"JP", L"10000h", 0x0000);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::JM, L"JM", L"10000h", 0x0000);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::JPE, L"JPE", L"10000h", 0x0000);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::JPO, L"JPO", L"10000h", 0x0000);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0xC3, 0x00, 0x00, 0xDA, 0x00, 0x00, 0xD2, 0x00, 0x00,
                     0xCA, 0x00, 0x00, 0xC2, 0x00, 0x00, 0xF2, 0x00, 0x00,
                     0xFA, 0x00, 0x00, 0xEA, 0x00, 0x00, 0xE2, 0x00, 0x00 };
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParsePCHL)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        PCHL\n"
                                   "        END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 E9                           PCHL\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::PCHL, L"PCHL");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0xE9 };
    CheckCode(ref, parser);
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
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 CD 00 01                     CALL 0100h\n"
              L"0003 DC 01 01                     CC 0101h\n"
              L"0006 D4 02 01                     CNC 0102h\n"
              L"0009 CC 03 01                     CZ 0103h\n"
              L"000C C4 04 01                     CNZ 0104h\n"
              L"000F F4 05 01                     CP 0105h\n"
              L"0012 FC 06 01                     CM 0106h\n"
              L"0015 EC 07 01                     CPE 0107h\n"
              L"0018 E4 08 01                     CPO 0108h\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::CALL, L"CALL", L"0100h", 0x0100);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::CC, L"CC", L"0101h", 0x0101);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::CNC, L"CNC", L"0102h", 0x0102);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::CZ, L"CZ", L"0103h", 0x0103);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::CNZ, L"CNZ", L"0104h", 0x0104);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::CP, L"CP", L"0105h", 0x0105);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::CM, L"CM", L"0106h", 0x0106);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::CPE, L"CPE", L"0107h", 0x0107);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::CPO, L"CPO", L"0108h", 0x0108);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0xCD, 0x00, 0x01, 0xDC, 0x01, 0x01, 0xD4, 0x02, 0x01,
                     0xCC, 0x03, 0x01, 0xC4, 0x04, 0x01, 0xF4, 0x05, 0x01,
                     0xFC, 0x06, 0x01, 0xEC, 0x07, 0x01, 0xE4, 0x08, 0x01 };
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParseCALL_CXX_Invalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        CALL 10000h\n"
                                   "        CC 10000h\n"
                                   "        CNC 10000h\n"
                                   "        CZ 10000h\n"
                                   "        CNZ 10000h\n"
                                   "        CP 10000h\n"
                                   "        CM 10000h\n"
                                   "        CPE 10000h\n"
                                   "        CPO 10000h\n"
                                   "        END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 CD 00 00                     CALL 10000h\n"
              L"--> Error: 2:14 - Value out of range for 16 bit value: 65536\n"
              L"0003 DC 00 00                     CC 10000h\n"
              L"--> Error: 3:12 - Value out of range for 16 bit value: 65536\n"
              L"0006 D4 00 00                     CNC 10000h\n"
              L"--> Error: 4:13 - Value out of range for 16 bit value: 65536\n"
              L"0009 CC 00 00                     CZ 10000h\n"
              L"--> Error: 5:12 - Value out of range for 16 bit value: 65536\n"
              L"000C C4 00 00                     CNZ 10000h\n"
              L"--> Error: 6:13 - Value out of range for 16 bit value: 65536\n"
              L"000F F4 00 00                     CP 10000h\n"
              L"--> Error: 7:12 - Value out of range for 16 bit value: 65536\n"
              L"0012 FC 00 00                     CM 10000h\n"
              L"--> Error: 8:12 - Value out of range for 16 bit value: 65536\n"
              L"0015 EC 00 00                     CPE 10000h\n"
              L"--> Error: 9:13 - Value out of range for 16 bit value: 65536\n"
              L"0018 E4 00 00                     CPO 10000h\n"
              L"--> Error: 10:13 - Value out of range for 16 bit value: 65536\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 9 }, messages.size());
    EXPECT_EQ(size_t{ 9 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::CALL, L"CALL", L"10000h", 0x0000);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::CC, L"CC", L"10000h", 0x0000);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::CNC, L"CNC", L"10000h", 0x0000);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::CZ, L"CZ", L"10000h", 0x0000);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::CNZ, L"CNZ", L"10000h", 0x0000);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::CP, L"CP", L"10000h", 0x0000);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::CM, L"CM", L"10000h", 0x0000);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::CPE, L"CPE", L"10000h", 0x0000);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::CPO, L"CPO", L"10000h", 0x0000);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0xCD, 0x00, 0x00, 0xDC, 0x00, 0x00, 0xD4, 0x00, 0x00,
                     0xCC, 0x00, 0x00, 0xC4, 0x00, 0x00, 0xF4, 0x00, 0x00,
                     0xFC, 0x00, 0x00, 0xEC, 0x00, 0x00, 0xE4, 0x00, 0x00 };
    CheckCode(ref, parser);
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
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 C9                           RET\n"
              L"0001 D8                           RC\n"
              L"0002 D0                           RNC\n"
              L"0003 C8                           RZ\n"
              L"0004 C0                           RNZ\n"
              L"0005 F0                           RP\n"
              L"0006 F8                           RM\n"
              L"0007 E8                           RPE\n"
              L"0008 E0                           RPO\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::RET, L"RET");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::RC, L"RC");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::RNC, L"RNC");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::RZ, L"RZ");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::RNZ, L"RNZ");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::RP, L"RP");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::RM, L"RM");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::RPE, L"RPE");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::RPO, L"RPO");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0xC9, 0xD8, 0xD0, 0xC8, 0xC0, 0xF0, 0xF8, 0xE8, 0xE0 };
    CheckCode(ref, parser);
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
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 C7                           RST 0\n"
              L"0001 CF                           RST 1\n"
              L"0002 D7                           RST 2\n"
              L"0003 DF                           RST 3\n"
              L"0004 E7                           RST 4\n"
              L"0005 EF                           RST 5\n"
              L"0006 F7                           RST 6\n"
              L"0007 FF                           RST 7\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeN(node, OpcodeType::RST, L"RST", RSTCode::RST0, L"0");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeN(node, OpcodeType::RST, L"RST", RSTCode::RST1, L"1");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeN(node, OpcodeType::RST, L"RST", RSTCode::RST2, L"2");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeN(node, OpcodeType::RST, L"RST", RSTCode::RST3, L"3");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeN(node, OpcodeType::RST, L"RST", RSTCode::RST4, L"4");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeN(node, OpcodeType::RST, L"RST", RSTCode::RST5, L"5");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeN(node, OpcodeType::RST, L"RST", RSTCode::RST6, L"6");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeN(node, OpcodeType::RST, L"RST", RSTCode::RST7, L"7");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0xC7, 0xCF, 0xD7, 0xDF, 0xE7, 0xEF, 0xF7, 0xFF };
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParseRST_Invalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        RST 8\n"
                                   "        END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000                              RST 8\n"
              L"--> Error: 2:13 - Invalid RST code: 8\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 1 }, messages.size());
    EXPECT_EQ(size_t{ 1 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeN(node, OpcodeType::RST, L"RST", RSTCode::Invalid, L"");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref;
    CheckCode(ref, parser);
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
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 04                           INR B\n"
              L"0001 0C                           INR C\n"
              L"0002 14                           INR D\n"
              L"0003 1C                           INR E\n"
              L"0004 24                           INR H\n"
              L"0005 2C                           INR L\n"
              L"0006 3C                           INR A\n"
              L"0007 34                           INR M\n"
              L"0008 05                           DCR B\n"
              L"0009 0D                           DCR C\n"
              L"000A 15                           DCR D\n"
              L"000B 1D                           DCR E\n"
              L"000C 25                           DCR H\n"
              L"000D 2D                           DCR L\n"
              L"000E 3D                           DCR A\n"
              L"000F 35                           DCR M\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::INR, L"INR", Register8Type::B, L"B");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::INR, L"INR", Register8Type::C, L"C");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::INR, L"INR", Register8Type::D, L"D");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::INR, L"INR", Register8Type::E, L"E");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::INR, L"INR", Register8Type::H, L"H");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::INR, L"INR", Register8Type::L, L"L");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::INR, L"INR", Register8Type::A, L"A");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::INR, L"INR", Register8Type::M, L"M");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::DCR, L"DCR", Register8Type::B, L"B");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::DCR, L"DCR", Register8Type::C, L"C");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::DCR, L"DCR", Register8Type::D, L"D");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::DCR, L"DCR", Register8Type::E, L"E");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::DCR, L"DCR", Register8Type::H, L"H");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::DCR, L"DCR", Register8Type::L, L"L");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::DCR, L"DCR", Register8Type::A, L"A");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::DCR, L"DCR", Register8Type::M, L"M");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C, 0x3C, 0x34, 0x05, 0x0D, 0x15, 0x1D, 0x25, 0x2D, 0x3D, 0x35 };
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParseINR_DCR_Invalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        INR X\n"
                                   "        DCR X\n"
                                   "        END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000                              INR X\n"
              L"--> Error: 2:13 - Invalid register: X\n"
              L"0001                              DCR X\n"
              L"--> Error: 3:13 - Invalid register: X\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 2 }, messages.size());
    EXPECT_EQ(size_t{ 2 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::INR, L"INR", Register8Type::Invalid, L"");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::DCR, L"DCR", Register8Type::Invalid, L"");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref;
    CheckCode(ref, parser);
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
                                   "        XRA B\n"
                                   "        XRA C\n"
                                   "        XRA D\n"
                                   "        XRA E\n"
                                   "        XRA H\n"
                                   "        XRA L\n"
                                   "        XRA A\n"
                                   "        XRA M\n"
                                   "        ORA B\n"
                                   "        ORA C\n"
                                   "        ORA D\n"
                                   "        ORA E\n"
                                   "        ORA H\n"
                                   "        ORA L\n"
                                   "        ORA A\n"
                                   "        ORA M\n"
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
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 80                           ADD B\n"
              L"0001 81                           ADD C\n"
              L"0002 82                           ADD D\n"
              L"0003 83                           ADD E\n"
              L"0004 84                           ADD H\n"
              L"0005 85                           ADD L\n"
              L"0006 87                           ADD A\n"
              L"0007 86                           ADD M\n"
              L"0008 88                           ADC B\n"
              L"0009 89                           ADC C\n"
              L"000A 8A                           ADC D\n"
              L"000B 8B                           ADC E\n"
              L"000C 8C                           ADC H\n"
              L"000D 8D                           ADC L\n"
              L"000E 8F                           ADC A\n"
              L"000F 8E                           ADC M\n"
              L"0010 90                           SUB B\n"
              L"0011 91                           SUB C\n"
              L"0012 92                           SUB D\n"
              L"0013 93                           SUB E\n"
              L"0014 94                           SUB H\n"
              L"0015 95                           SUB L\n"
              L"0016 97                           SUB A\n"
              L"0017 96                           SUB M\n"
              L"0018 98                           SBB B\n"
              L"0019 99                           SBB C\n"
              L"001A 9A                           SBB D\n"
              L"001B 9B                           SBB E\n"
              L"001C 9C                           SBB H\n"
              L"001D 9D                           SBB L\n"
              L"001E 9F                           SBB A\n"
              L"001F 9E                           SBB M\n"
              L"0020 A0                           ANA B\n"
              L"0021 A1                           ANA C\n"
              L"0022 A2                           ANA D\n"
              L"0023 A3                           ANA E\n"
              L"0024 A4                           ANA H\n"
              L"0025 A5                           ANA L\n"
              L"0026 A7                           ANA A\n"
              L"0027 A6                           ANA M\n"
              L"0028 A8                           XRA B\n"
              L"0029 A9                           XRA C\n"
              L"002A AA                           XRA D\n"
              L"002B AB                           XRA E\n"
              L"002C AC                           XRA H\n"
              L"002D AD                           XRA L\n"
              L"002E AF                           XRA A\n"
              L"002F AE                           XRA M\n"
              L"0030 B0                           ORA B\n"
              L"0031 B1                           ORA C\n"
              L"0032 B2                           ORA D\n"
              L"0033 B3                           ORA E\n"
              L"0034 B4                           ORA H\n"
              L"0035 B5                           ORA L\n"
              L"0036 B7                           ORA A\n"
              L"0037 B6                           ORA M\n"
              L"0038 B8                           CMP B\n"
              L"0039 B9                           CMP C\n"
              L"003A BA                           CMP D\n"
              L"003B BB                           CMP E\n"
              L"003C BC                           CMP H\n"
              L"003D BD                           CMP L\n"
              L"003E BF                           CMP A\n"
              L"003F BE                           CMP M\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ADD, L"ADD", Register8Type::B, L"B");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ADD, L"ADD", Register8Type::C, L"C");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ADD, L"ADD", Register8Type::D, L"D");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ADD, L"ADD", Register8Type::E, L"E");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ADD, L"ADD", Register8Type::H, L"H");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ADD, L"ADD", Register8Type::L, L"L");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ADD, L"ADD", Register8Type::A, L"A");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ADD, L"ADD", Register8Type::M, L"M");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ADC, L"ADC", Register8Type::B, L"B");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ADC, L"ADC", Register8Type::C, L"C");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ADC, L"ADC", Register8Type::D, L"D");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ADC, L"ADC", Register8Type::E, L"E");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ADC, L"ADC", Register8Type::H, L"H");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ADC, L"ADC", Register8Type::L, L"L");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ADC, L"ADC", Register8Type::A, L"A");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ADC, L"ADC", Register8Type::M, L"M");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::SUB, L"SUB", Register8Type::B, L"B");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::SUB, L"SUB", Register8Type::C, L"C");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::SUB, L"SUB", Register8Type::D, L"D");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::SUB, L"SUB", Register8Type::E, L"E");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::SUB, L"SUB", Register8Type::H, L"H");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::SUB, L"SUB", Register8Type::L, L"L");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::SUB, L"SUB", Register8Type::A, L"A");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::SUB, L"SUB", Register8Type::M, L"M");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::SBB, L"SBB", Register8Type::B, L"B");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::SBB, L"SBB", Register8Type::C, L"C");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::SBB, L"SBB", Register8Type::D, L"D");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::SBB, L"SBB", Register8Type::E, L"E");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::SBB, L"SBB", Register8Type::H, L"H");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::SBB, L"SBB", Register8Type::L, L"L");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::SBB, L"SBB", Register8Type::A, L"A");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::SBB, L"SBB", Register8Type::M, L"M");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ANA, L"ANA", Register8Type::B, L"B");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ANA, L"ANA", Register8Type::C, L"C");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ANA, L"ANA", Register8Type::D, L"D");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ANA, L"ANA", Register8Type::E, L"E");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ANA, L"ANA", Register8Type::H, L"H");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ANA, L"ANA", Register8Type::L, L"L");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ANA, L"ANA", Register8Type::A, L"A");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ANA, L"ANA", Register8Type::M, L"M");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::XRA, L"XRA", Register8Type::B, L"B");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::XRA, L"XRA", Register8Type::C, L"C");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::XRA, L"XRA", Register8Type::D, L"D");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::XRA, L"XRA", Register8Type::E, L"E");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::XRA, L"XRA", Register8Type::H, L"H");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::XRA, L"XRA", Register8Type::L, L"L");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::XRA, L"XRA", Register8Type::A, L"A");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::XRA, L"XRA", Register8Type::M, L"M");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ORA, L"ORA", Register8Type::B, L"B");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ORA, L"ORA", Register8Type::C, L"C");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ORA, L"ORA", Register8Type::D, L"D");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ORA, L"ORA", Register8Type::E, L"E");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ORA, L"ORA", Register8Type::H, L"H");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ORA, L"ORA", Register8Type::L, L"L");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ORA, L"ORA", Register8Type::A, L"A");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ORA, L"ORA", Register8Type::M, L"M");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::CMP, L"CMP", Register8Type::B, L"B");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::CMP, L"CMP", Register8Type::C, L"C");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::CMP, L"CMP", Register8Type::D, L"D");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::CMP, L"CMP", Register8Type::E, L"E");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::CMP, L"CMP", Register8Type::H, L"H");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::CMP, L"CMP", Register8Type::L, L"L");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::CMP, L"CMP", Register8Type::A, L"A");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::CMP, L"CMP", Register8Type::M, L"M");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x87, 0x86,
                     0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8F, 0x8E,
                     0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x97, 0x96,
                     0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9F, 0x9E,
                     0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA7, 0xA6,
                     0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAF, 0xAE,
                     0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB7, 0xB6,
                     0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBF, 0xBE };
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParseADD_ADC_SUB_SBB_ANA_ORA_XRA_CMP_Invalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        ADD X\n"
                                   "        ADC X\n"
                                   "        SUB X\n"
                                   "        SBB X\n"
                                   "        ANA X\n"
                                   "        XRA X\n"
                                   "        ORA X\n"
                                   "        CMP X\n"
                                   "        END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000                              ADD X\n"
              L"--> Error: 2:13 - Invalid register: X\n"
              L"0001                              ADC X\n"
              L"--> Error: 3:13 - Invalid register: X\n"
              L"0002                              SUB X\n"
              L"--> Error: 4:13 - Invalid register: X\n"
              L"0003                              SBB X\n"
              L"--> Error: 5:13 - Invalid register: X\n"
              L"0004                              ANA X\n"
              L"--> Error: 6:13 - Invalid register: X\n"
              L"0005                              XRA X\n"
              L"--> Error: 7:13 - Invalid register: X\n"
              L"0006                              ORA X\n"
              L"--> Error: 8:13 - Invalid register: X\n"
              L"0007                              CMP X\n"
              L"--> Error: 9:13 - Invalid register: X\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 8 }, messages.size());
    EXPECT_EQ(size_t{ 8 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ADD, L"ADD", Register8Type::Invalid, L"");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ADC, L"ADC", Register8Type::Invalid, L"");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::SUB, L"SUB", Register8Type::Invalid, L"");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::SBB, L"SBB", Register8Type::Invalid, L"");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ANA, L"ANA", Register8Type::Invalid, L"");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::XRA, L"XRA", Register8Type::Invalid, L"");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::ORA, L"ORA", Register8Type::Invalid, L"");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR8(node, OpcodeType::CMP, L"CMP", Register8Type::Invalid, L"");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref;
    CheckCode(ref, parser);
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
                                   "        XRI 05h\n"
                                   "        ORI 06h\n"
                                   "        CPI 07h\n"
                                   "        END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 C6 00                        ADI 00h\n"
              L"0002 CE 01                        ACI 01h\n"
              L"0004 D6 02                        SUI 02h\n"
              L"0006 DE 03                        SBI 03h\n"
              L"0008 E6 04                        ANI 04h\n"
              L"000A EE 05                        XRI 05h\n"
              L"000C F6 06                        ORI 06h\n"
              L"000E FE 07                        CPI 07h\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD8(node, OpcodeType::ADI, L"ADI", L"00h", 0x00);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD8(node, OpcodeType::ACI, L"ACI", L"01h", 0x01);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD8(node, OpcodeType::SUI, L"SUI", L"02h", 0x02);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD8(node, OpcodeType::SBI, L"SBI", L"03h", 0x03);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD8(node, OpcodeType::ANI, L"ANI", L"04h", 0x04);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD8(node, OpcodeType::XRI, L"XRI", L"05h", 0x05);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD8(node, OpcodeType::ORI, L"ORI", L"06h", 0x06);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD8(node, OpcodeType::CPI, L"CPI", L"07h", 0x07);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0xC6, 0x00, 0xCE, 0x01, 0xD6, 0x02, 0xDE, 0x03,
                     0xE6, 0x04, 0xEE, 0x05, 0xF6, 0x06, 0xFE, 0x07 };
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParseADI_ACI_SUI_SBI_ANI_ORI_XRI_CPI_Invalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        ADI 100h\n"
                                   "        ACI 100h\n"
                                   "        SUI 100h\n"
                                   "        SBI 100h\n"
                                   "        ANI 100h\n"
                                   "        XRI 100h\n"
                                   "        ORI 100h\n"
                                   "        CPI 100h\n"
                                   "        END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 C6 00                        ADI 100h\n"
              L"--> Error: 2:13 - Value out of range for 8 bit value: 256\n"
              L"0002 CE 00                        ACI 100h\n"
              L"--> Error: 3:13 - Value out of range for 8 bit value: 256\n"
              L"0004 D6 00                        SUI 100h\n"
              L"--> Error: 4:13 - Value out of range for 8 bit value: 256\n"
              L"0006 DE 00                        SBI 100h\n"
              L"--> Error: 5:13 - Value out of range for 8 bit value: 256\n"
              L"0008 E6 00                        ANI 100h\n"
              L"--> Error: 6:13 - Value out of range for 8 bit value: 256\n"
              L"000A EE 00                        XRI 100h\n"
              L"--> Error: 7:13 - Value out of range for 8 bit value: 256\n"
              L"000C F6 00                        ORI 100h\n"
              L"--> Error: 8:13 - Value out of range for 8 bit value: 256\n"
              L"000E FE 00                        CPI 100h\n"
              L"--> Error: 9:13 - Value out of range for 8 bit value: 256\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 8 }, messages.size());
    EXPECT_EQ(size_t{ 8 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD8(node, OpcodeType::ADI, L"ADI", L"100h", 0x00);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD8(node, OpcodeType::ACI, L"ACI", L"100h", 0x00);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD8(node, OpcodeType::SUI, L"SUI", L"100h", 0x00);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD8(node, OpcodeType::SBI, L"SBI", L"100h", 0x00);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD8(node, OpcodeType::ANI, L"ANI", L"100h", 0x00);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD8(node, OpcodeType::XRI, L"XRI", L"100h", 0x00);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD8(node, OpcodeType::ORI, L"ORI", L"100h", 0x00);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD8(node, OpcodeType::CPI, L"CPI", L"100h", 0x00);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0xC6, 0x00, 0xCE, 0x00, 0xD6, 0x00, 0xDE, 0x00,
                     0xE6, 0x00, 0xEE, 0x00, 0xF6, 0x00, 0xFE, 0x00 };
    CheckCode(ref, parser);
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
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 09                           DAD B\n"
              L"0001 19                           DAD D\n"
              L"0002 29                           DAD H\n"
              L"0003 39                           DAD SP\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::DAD, L"DAD", Register16Type::BC, L"B");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::DAD, L"DAD", Register16Type::DE, L"D");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::DAD, L"DAD", Register16Type::HL, L"H");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::DAD, L"DAD", Register16Type::SP, L"SP");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0x09, 0x19, 0x29, 0x39 };
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParseDAD_Invalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        DAD PSW\n"
                                   "        DAD X\n"
                                   "        END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000                              DAD PSW\n"
              L"--> Error: 2:9 - Opcode DAD only supports B, D, H or SP register pair, PSW specified\n"
              L"0001                              DAD X\n"
              L"--> Error: 3:13 - Invalid register pair: X\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 2 }, messages.size());
    EXPECT_EQ(size_t{ 2 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::DAD, L"DAD", Register16Type::Invalid, L"");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeR16(node, OpcodeType::DAD, L"DAD", Register16Type::Invalid, L"");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref;
    CheckCode(ref, parser);
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
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 07                           RLC\n"
              L"0001 0F                           RRC\n"
              L"0002 17                           RAL\n"
              L"0003 1F                           RAR\n"
              L"0004 2F                           CMA\n"
              L"0005 37                           STC\n"
              L"0006 3F                           CMC\n"
              L"0007 27                           DAA\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::RLC, L"RLC");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::RRC, L"RRC");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::RAL, L"RAL");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::RAR, L"RAR");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::CMA, L"CMA");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::STC, L"STC");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::CMC, L"CMC");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::DAA, L"DAA");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0x07, 0x0F, 0x17, 0x1F, 0x2F, 0x37, 0x3F, 0x27 };
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParseIN_OUT)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        IN 00h\n"
                                   "        OUT 01h\n"
                                   "        END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 DB 00                        IN 00h\n"
              L"0002 D3 01                        OUT 01h\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD8(node, OpcodeType::INP, L"IN", L"00h", 0x00);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD8(node, OpcodeType::OUTP, L"OUT", L"01h", 0x01);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0xDB, 0x00, 0xD3, 0x01 };
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParseIN_OUT_Invalid)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        IN 100h\n"
                                   "        OUT 101h\n"
                                   "        END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 DB 00                        IN 100h\n"
              L"--> Error: 2:12 - Value out of range for 8 bit value: 256\n"
              L"0002 D3 01                        OUT 101h\n"
              L"--> Error: 3:13 - Value out of range for 8 bit value: 257\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 2 }, messages.size());
    EXPECT_EQ(size_t{ 2 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD8(node, OpcodeType::INP, L"IN", L"100h", 0x00);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD8(node, OpcodeType::OUTP, L"OUT", L"101h", 0x01);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0xDB, 0x00, 0xD3, 0x01 };
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParseEI_DI_NOP_HLT)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "START:  ORG 0100h; Set initial address\n"
                                   "        EI\n"
                                   "        DI\n"
                                   "NOP:    NOP\n"
                                   "        HLT\n"
                                   "        END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0100          START:              ORG 0100h           ; Set initial address\n"
              L"0100 FB                           EI\n"
              L"0101 F3                           DI\n"
              L"0102 00       NOP:                NOP\n"
              L"0103 76                           HLT\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOrg(node, L"0100h", L"START", L" Set initial address");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::EI, L"EI");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::DI, L"DI");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::NOP, L"NOP", L"NOP");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::HLT, L"HLT");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0xFB, 0xF3, 0x00, 0x76 };
    CheckCode(ref, parser, 0x0100);
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
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8085\n"
              L"0000 08                           DSUB\n"
              L"0001 10                           ARHL\n"
              L"0002 18                           RDEL\n"
              L"0003 20                           RIM\n"
              L"0004 28 01                        LDHI 01h\n"
              L"0006 30                           SIM\n"
              L"0007 38 02                        LDSI 02h\n"
              L"0009 CB                           RSTV\n"
              L"000A D9                           SHLX\n"
              L"000B DD 03 01                     JNK 0103h\n"
              L"000E ED                           LHLX\n"
              L"000F FD 04 01                     JK 0104h\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8085");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::DSUB, L"DSUB");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::ARHL, L"ARHL");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::RDEL, L"RDEL");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::RIM, L"RIM");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD8(node, OpcodeType::LDHI, L"LDHI", L"01h", 0x01);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::SIM, L"SIM");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD8(node, OpcodeType::LDSI, L"LDSI", L"02h", 0x02);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::RSTV, L"RSTV");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::SHLX, L"SHLX");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::JNK, L"JNK", L"0103h", 0x0103);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::LHLX, L"LHLX");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeD16(node, OpcodeType::JK, L"JK", L"0104h", 0x0104);
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0x08, 0x10, 0x18, 0x20, 0x28, 0x01, 0x30, 0x38, 0x02, 0xCB, 0xD9, 0xDD, 0x03, 0x01, 0xED, 0xFD, 0x04, 0x01 };
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, Parse8085ExtensionsOn8080)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
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
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000                              DSUB\n"
              L"--> Error: 2:9 - Expected Opcode: DSUB\n"
              L"0000                              ARHL\n"
              L"--> Error: 3:9 - Expected Opcode: ARHL\n"
              L"0000                              RDEL\n"
              L"--> Error: 4:9 - Expected Opcode: RDEL\n"
              L"0000                              RIM\n"
              L"--> Error: 5:9 - Expected Opcode: RIM\n"
              L"0000                              LDHI 01h\n"
              L"--> Error: 6:9 - Expected Opcode: LDHI\n"
              L"0000                              SIM\n"
              L"--> Error: 7:9 - Expected Opcode: SIM\n"
              L"0000                              LDSI 02h\n"
              L"--> Error: 8:9 - Expected Opcode: LDSI\n"
              L"0000                              RSTV\n"
              L"--> Error: 9:9 - Expected Opcode: RSTV\n"
              L"0000                              SHLX\n"
              L"--> Error: 10:9 - Expected Opcode: SHLX\n"
              L"0000                              JNK 0103h\n"
              L"--> Error: 11:9 - Expected Opcode: JNK\n"
              L"0000                              LHLX\n"
              L"--> Error: 12:9 - Expected Opcode: LHLX\n"
              L"0000                              JK 0104h\n"
              L"--> Error: 13:9 - Expected Opcode: JK\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 12 }, messages.size());
    EXPECT_EQ(size_t{ 12 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeInvalid(node, L"DSUB");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeInvalid(node, L"ARHL");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeInvalid(node, L"RDEL");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeInvalid(node, L"RIM");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeInvalid(node, L"LDHI", L"", L"", L"01h");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeInvalid(node, L"SIM");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeInvalid(node, L"LDSI", L"", L"", L"02h");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeInvalid(node, L"RSTV");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeInvalid(node, L"SHLX");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeInvalid(node, L"JNK", L"", L"", L"0103h");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeInvalid(node, L"LHLX");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeInvalid(node, L"JK", L"", L"", L"0104h");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref;
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParseJMPWithLocationCounter)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        JMP $\n"
                                   "        END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 C3 00 00                     JMP $\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeLocCounter(node, OpcodeType::JMP, L"JMP");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0xC3, 0x00, 0x00 };
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParseJMPWithPredefinedLabel)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "START:  NOP \n"
                                   "        JMP START\n"
                                   "        END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 00       START:              NOP\n"
              L"0001 C3 00 00                     JMP START\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::NOP, L"NOP", L"START");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeRef(node, OpcodeType::JMP, L"JMP", L"START");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0x00, 0xC3, 0x00, 0x00 };
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, ParseJMPWithUndefinedLabel)
{
    AssemblerMessages messages;
    std::istringstream inputStream("        CPU Intel8080\n"
                                   "        JMP START\n"
                                   "START:  NOP\n"
                                   "        END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 C3 03 00                     JMP START\n"
              L"0003 00       START:              NOP\n"
              L"                                  END\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();
    auto node = dynamic_pointer_cast<StatementLineNode>(ast.FirstNode());

    CheckCPU(node, L"Intel8080");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcodeRef(node, OpcodeType::JMP, L"JMP", L"START");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckOpcode(node, OpcodeType::NOP, L"NOP", L"START");
    node = dynamic_pointer_cast<StatementLineNode>(node->Next());
    CheckEnd(node);

    EXPECT_NULL(node->Next());

    MachineCode ref{ 0xC3, 0x03, 0x00, 0x00 };
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, AllInstructions8080)
{
    AssemblerMessages messages;
    std::istringstream inputStream(
        "CPU Intel8080\n"   // 1
        "NOP\n"             // 2
        "LXI B, 0001h\n"    // 3
        "STAX B\n"          // 4
        "INX B\n"           // 5
        "INR B\n"           // 6
        "DCR B\n"           // 7
        "MVI B, 02h\n"      // 8
        "RLC\n"             // 9
        //8085 "DSUB\n"
        "DAD B\n"           // 10
        "LDAX B\n"          // 11
        "DCX B\n"           // 12
        "INR C\n"           // 13
        "DCR C\n"           // 14
        "MVI C, 03h\n"      // 15
        "RRC\n"             // 16
        //8085 "ARHL\n"
        "LXI D, 0004\n"     // 17
        "STAX D\n"          // 18
        "INX D\n"           // 19
        "INR D\n"           // 20
        "DCR D\n"           // 21
        "MVI D, 05h\n"      // 22
        "RAL\n"             // 23
        //8085 "RDEL\n"
        "DAD D\n"           // 24
        "LDAX D\n"          // 25
        "DCX D\n"           // 26
        "INR E\n"           // 27
        "DCR E\n"           // 28
        "MVI E, 06h\n"      // 29
        "RAR\n"             // 30
        //8085 "RIM\n"
        "LXI H, 0007h\n"    // 31
        "SHLD 0008h\n"      // 32
        "INX H\n"           // 33
        "INR H\n"           // 34
        "DCR H\n"           // 35
        "MVI H, 09h\n"      // 36
        "DAA\n"             // 37
        //8085 "LDHI 0Ah\n"
        "DAD H\n"           // 38
        "LHLD 000Bh\n"      // 39
        "DCX H\n"           // 40
        "INR L\n"           // 41
        "DCR L\n"           // 41
        "MVI L, 0Ch\n"      // 42
        "CMA\n"             // 43
        //8085 "SIM\n"
        "LXI SP, 000Dh\n"   // 44
        "STA 000Eh\n"       // 45
        "INX SP\n"          // 46
        "INR M\n"           // 47
        "DCR M\n"           // 48
        "MVI M, 0Fh\n"      // 49
        "STC\n"             // 50
        //8085 "LDSI 10h\n"
        "DAD SP\n"          // 51
        "LDA 0011h\n"       // 52
        "DCX SP\n"          // 53
        "INR A\n"           // 54
        "DCR A\n"           // 55
        "MVI A, 12h\n"      // 56
        "CMC\n"             // 57
        "MOV B,B\n"         // 58
        "MOV B,C\n"         // 59
        "MOV B,D\n"         // 60
        "MOV B,E\n"         // 61
        "MOV B,H\n"         // 62
        "MOV B,L\n"         // 63
        "MOV B,M\n"         // 64
        "MOV B,A\n"         // 65
        "MOV C,B\n"         // 66
        "MOV C,C\n"         // 67
        "MOV C,D\n"         // 68
        "MOV C,E\n"         // 69
        "MOV C,H\n"         // 70
        "MOV C,L\n"         // 71
        "MOV C,M\n"         // 72
        "MOV C,A\n"         // 73
        "MOV D,B\n"         // 74
        "MOV D,C\n"         // 75
        "MOV D,D\n"         // 76
        "MOV D,E\n"         // 77
        "MOV D,H\n"         // 78
        "MOV D,L\n"         // 79
        "MOV D,M\n"         // 80
        "MOV D,A\n"         // 81
        "MOV E,B\n"         // 82
        "MOV E,C\n"         // 83
        "MOV E,D\n"         // 84
        "MOV E,E\n"         // 85
        "MOV E,H\n"         // 86
        "MOV E,L\n"         // 87
        "MOV E,M\n"         // 88
        "MOV E,A\n"         // 89
        "MOV H,B\n"         // 90
        "MOV H,C\n"         // 91
        "MOV H,D\n"         // 92
        "MOV H,E\n"         // 93
        "MOV H,H\n"         // 94
        "MOV H,L\n"         // 95
        "MOV H,M\n"         // 96
        "MOV H,A\n"         // 97
        "MOV L,B\n"         // 98
        "MOV L,C\n"         // 99
        "MOV L,D\n"         // 100
        "MOV L,E\n"         // 101
        "MOV L,H\n"         // 102
        "MOV L,L\n"         // 103
        "MOV L,M\n"         // 104
        "MOV L,A\n"         // 105
        "MOV M,B\n"         // 106
        "MOV M,C\n"         // 107
        "MOV M,D\n"         // 108
        "MOV M,E\n"         // 109
        "MOV M,H\n"         // 110
        "MOV M,L\n"         // 111
        "HLT\n"             // 112
        "MOV M,A\n"         // 113
        "MOV A,B\n"         // 114
        "MOV A,C\n"         // 115
        "MOV A,D\n"         // 116
        "MOV A,E\n"         // 117
        "MOV A,H\n"         // 118
        "MOV A,L\n"         // 119
        "MOV A,M\n"         // 120
        "MOV A,A\n"         // 121
        "ADD B\n"           // 122
        "ADD C\n"           // 123
        "ADD D\n"           // 124
        "ADD E\n"           // 125
        "ADD H\n"           // 126
        "ADD L\n"           // 127
        "ADD M\n"           // 128
        "ADD A\n"           // 129
        "ADC B\n"           // 130
        "ADC C\n"           // 131
        "ADC D\n"           // 132
        "ADC E\n"           // 133
        "ADC H\n"           // 134
        "ADC L\n"           // 135
        "ADC M\n"           // 136
        "ADC A\n"           // 137
        "SUB B\n"           // 138
        "SUB C\n"           // 139
        "SUB D\n"           // 140
        "SUB E\n"           // 141
        "SUB H\n"           // 142
        "SUB L\n"           // 143
        "SUB M\n"           // 144
        "SUB A\n"           // 145
        "SBB B\n"           // 146
        "SBB C\n"           // 147
        "SBB D\n"           // 148
        "SBB E\n"           // 149
        "SBB H\n"           // 150
        "SBB L\n"           // 151
        "SBB M\n"           // 152
        "SBB A\n"           // 153
        "ANA B\n"           // 154
        "ANA C\n"           // 155
        "ANA D\n"           // 156
        "ANA E\n"           // 157
        "ANA H\n"           // 158
        "ANA L\n"           // 159
        "ANA M\n"           // 160
        "ANA A\n"           // 161
        "XRA B\n"           // 162
        "XRA C\n"           // 163
        "XRA D\n"           // 164
        "XRA E\n"           // 165
        "XRA H\n"           // 166
        "XRA L\n"           // 167
        "XRA M\n"           // 168
        "XRA A\n"           // 169
        "ORA B\n"           // 170
        "ORA C\n"           // 171
        "ORA D\n"           // 172
        "ORA E\n"           // 173
        "ORA H\n"           // 174
        "ORA L\n"           // 175
        "ORA M\n"           // 176
        "ORA A\n"           // 177
        "CMP B\n"           // 178
        "CMP C\n"           // 179
        "CMP D\n"           // 180
        "CMP E\n"           // 181
        "CMP H\n"           // 182
        "CMP L\n"           // 183
        "CMP M\n"           // 184
        "CMP A\n"           // 185
        "RNZ\n"             // 186
        "POP B\n"           // 187
        "JNZ 0013h\n"       // 188
        "JMP 0014h\n"       // 189
        "CNZ 0015h\n"       // 190
        "PUSH B\n"          // 191
        "ADI 16h\n"         // 192
        "RST 0\n"           // 193
        "RZ\n"              // 194
        "RET\n"             // 195
        "JZ 0017h\n"        // 196
        //8085 "RSTV\n"
        "CZ 0018h\n"        // 197
        "CALL 0019h\n"      // 198
        "ACI 1Ah\n"         // 199
        "RST 1\n"           // 200
        "RNC\n"             // 201
        "POP D\n"           // 202
        "JNC 001Bh\n"       // 203
        "OUT 1Ch\n"         // 204
        "CNC 001Dh\n"       // 205
        "PUSH D\n"          // 206
        "SUI 1Eh\n"         // 207
        "RST 2\n"           // 208
        "RC\n"              // 209
        //8085 "SHLX\n"
        "JC 001Fh\n"        // 210
        "IN 20h\n"          // 211
        "CC 0021h\n"        // 212
        //8085 "JNK 0022h\n"
        "SBI 23h\n"         // 213
        "RST 3\n"           // 214
        "RPO\n"             // 215
        "POP H\n"           // 216
        "JPO 0024h\n"       // 217
        "XTHL\n"            // 218
        "CPO 0025h\n"       // 219
        "PUSH H\n"          // 220
        "ANI 26h\n"         // 221
        "RST 4\n"           // 222
        "RPE\n"             // 223
        "PCHL\n"            // 224
        "JPE 0027h\n"       // 225
        "XCHG\n"            // 226
        "CPE 0028h\n"       // 227
        //8085 "LHLX\n"
        "XRI 29h\n"         // 228
        "RST 5\n"           // 229
        "RP\n"              // 230
        "POP PSW\n"         // 231
        "JP 002Ah\n"        // 232
        "DI\n"              // 233
        "CP 002Bh\n"        // 234
        "PUSH PSW\n"        // 235
        "ORI 2Ch\n"         // 236
        "RST 6\n"           // 237
        "RM\n"              // 238
        "SPHL\n"            // 239
        "JM 002Dh\n"        // 240
        "EI\n"              // 241
        "CM 002Eh\n"        // 242
        //8085 "JK 002Fh\n"
        "CPI 30h\n"         // 243
        "RST 7\n"           // 244
        "END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();

    MachineCode ref
    { 
        0x00, 
        0x01, 0x01, 0x00,
        0x02,
        0x03,
        0x04,
        0x05,
        0x06, 0x02,
        0x07,
        //8085 0x08,
        0x09,
        0x0A, 
        0x0B,
        0x0C,
        0x0D,
        0x0E, 0x03,
        0x0F,
        //8085 0x10,
        0x11, 0x04, 0x00,
        0x12,
        0x13,
        0x14,
        0x15,
        0x16, 0x05,
        0x17,
        //8085 0x18,
        0x19,
        0x1A,
        0x1B,
        0x1C,
        0x1D,
        0x1E, 0x06,
        0x1F,
        //8085 0x20,
        0x21, 0x07, 0x00,
        0x22, 0x08, 0x00,
        0x23, 
        0x24,
        0x25,
        0x26, 0x09,
        0x27,
        //8085 0x28, 0x0A,
        0x29, 
        0x2A, 0x0B, 0x00,
        0x2B,
        0x2C,
        0x2D,
        0x2E, 0x0C,
        0x2F,
        //8085 0x30,
        0x31, 0x0D, 0x00,
        0x32, 0x0E, 0x00,
        0x33,
        0x34,
        0x35,
        0x36, 0x0F,
        0x37,
        //8085 0x38, 0x10,
        0x39,
        0x3A, 0x11, 0x00,
        0x3B,
        0x3C,
        0x3D,
        0x3E, 0x12,
        0x3F,
        0x40,
        0x41,
        0x42,
        0x43,
        0x44,
        0x45,
        0x46,
        0x47,
        0x48,
        0x49,
        0x4A,
        0x4B,
        0x4C,
        0x4D,
        0x4E,
        0x4F,
        0x50,
        0x51,
        0x52,
        0x53,
        0x54,
        0x55,
        0x56,
        0x57,
        0x58,
        0x59,
        0x5A,
        0x5B,
        0x5C,
        0x5D,
        0x5E,
        0x5F,
        0x60,
        0x61,
        0x62,
        0x63,
        0x64,
        0x65,
        0x66,
        0x67,
        0x68,
        0x69,
        0x6A,
        0x6B,
        0x6C,
        0x6D,
        0x6E,
        0x6F,
        0x70,
        0x71,
        0x72,
        0x73,
        0x74,
        0x75,
        0x76,
        0x77,
        0x78,
        0x79,
        0x7A,
        0x7B,
        0x7C,
        0x7D,
        0x7E,
        0x7F,
        0x80,
        0x81,
        0x82,
        0x83,
        0x84,
        0x85,
        0x86,
        0x87,
        0x88,
        0x89,
        0x8A,
        0x8B,
        0x8C,
        0x8D,
        0x8E,
        0x8F,
        0x90,
        0x91,
        0x92,
        0x93,
        0x94,
        0x95,
        0x96,
        0x97,
        0x98,
        0x99,
        0x9A,
        0x9B,
        0x9C,
        0x9D,
        0x9E,
        0x9F,
        0xA0,
        0xA1,
        0xA2,
        0xA3,
        0xA4,
        0xA5,
        0xA6,
        0xA7,
        0xA8,
        0xA9,
        0xAA,
        0xAB,
        0xAC,
        0xAD,
        0xAE,
        0xAF,
        0xB0,
        0xB1,
        0xB2,
        0xB3,
        0xB4,
        0xB5,
        0xB6,
        0xB7,
        0xB8,
        0xB9,
        0xBA,
        0xBB,
        0xBC,
        0xBD,
        0xBE,
        0xBF,
        0xC0,
        0xC1,
        0xC2, 0x13, 0x00,
        0xC3, 0x14, 0x00,
        0xC4, 0x15, 0x00,
        0xC5,
        0xC6, 0x16,
        0xC7,
        0xC8,
        0xC9,
        0xCA, 0x17, 0x00,
        //8085 0xCB, 
        0xCC, 0x18, 0x00,
        0xCD, 0x19, 0x00,
        0xCE, 0x1A,
        0xCF,
        0xD0,
        0xD1,
        0xD2, 0x1B, 0x00,
        0xD3, 0x1C,
        0xD4, 0x1D, 0x00,
        0xD5, 
        0xD6, 0x1E,
        0xD7,
        0xD8,
        //8085 0xD9,
        0xDA, 0x1F, 0x00,
        0xDB, 0x20,
        0xDC, 0x21, 0x00,
        //8085 0xDD, 0x22, 0x00,
        0xDE, 0x23,
        0xDF,
        0xE0,
        0xE1,
        0xE2, 0x24, 0x00,
        0xE3,
        0xE4, 0x25, 0x00,
        0xE5,
        0xE6, 0x26,
        0xE7,
        0xE8,
        0xE9,
        0xEA, 0x27, 0x00,
        0xEB,
        0xEC, 0x28, 0x00,
        //8085 0xED,
        0xEE, 0x29,
        0xEF,
        0xF0,
        0xF1,
        0xF2, 0x2A, 0x00,
        0xF3,
        0xF4, 0x2B, 0x00,
        0xF5,
        0xF6, 0x2C,
        0xF7,
        0xF8,
        0xF9,
        0xFA, 0x2D, 0x00,
        0xFB,
        0xFC, 0x2E, 0x00,
        //8085 0xFD, 0x2F, 0x00,
        0xFE, 0x30,
        0xFF,
    };
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, AllInstructions8085)
{
    AssemblerMessages messages;
    std::istringstream inputStream(
        "CPU Intel8085\n"   // 1
        "NOP\n"             // 2
        "LXI B, 0001h\n"    // 3
        "STAX B\n"          // 4
        "INX B\n"           // 5
        "INR B\n"           // 6
        "DCR B\n"           // 7
        "MVI B, 02h\n"      // 8
        "RLC\n"             // 9
        "DSUB\n"            // 10
        "DAD B\n"           // 11
        "LDAX B\n"          // 12
        "DCX B\n"           // 13
        "INR C\n"           // 14
        "DCR C\n"           // 15
        "MVI C, 03h\n"      // 16
        "RRC\n"             // 17
        "ARHL\n"            // 18
        "LXI D, 0004\n"     // 19
        "STAX D\n"          // 20
        "INX D\n"           // 21
        "INR D\n"           // 22
        "DCR D\n"           // 23
        "MVI D, 05h\n"      // 24
        "RAL\n"             // 25
        "RDEL\n"            // 26
        "DAD D\n"           // 27
        "LDAX D\n"          // 28
        "DCX D\n"           // 29
        "INR E\n"           // 30
        "DCR E\n"           // 31
        "MVI E, 06h\n"      // 32
        "RAR\n"             // 33
        "RIM\n"             // 34
        "LXI H, 0007h\n"    // 35
        "SHLD 0008h\n"      // 36
        "INX H\n"           // 37
        "INR H\n"           // 38
        "DCR H\n"           // 39
        "MVI H, 09h\n"      // 40
        "DAA\n"             // 41
        "LDHI 0Ah\n"        // 42
        "DAD H\n"           // 43
        "LHLD 000Bh\n"      // 44
        "DCX H\n"           // 45
        "INR L\n"           // 46
        "DCR L\n"           // 47
        "MVI L, 0Ch\n"      // 48
        "CMA\n"             // 49
        "SIM\n"             // 50
        "LXI SP, 000Dh\n"   // 51
        "STA 000Eh\n"       // 52
        "INX SP\n"          // 53
        "INR M\n"           // 54
        "DCR M\n"           // 55
        "MVI M, 0Fh\n"      // 56
        "STC\n"             // 57
        "LDSI 10h\n"        // 58
        "DAD SP\n"          // 59
        "LDA 0011h\n"       // 60
        "DCX SP\n"          // 61
        "INR A\n"           // 62
        "DCR A\n"           // 63
        "MVI A, 12h\n"      // 64
        "CMC\n"             // 65
        "MOV B,B\n"         // 66
        "MOV B,C\n"         // 67
        "MOV B,D\n"         // 68
        "MOV B,E\n"         // 69
        "MOV B,H\n"         // 70
        "MOV B,L\n"         // 71
        "MOV B,M\n"         // 72
        "MOV B,A\n"         // 73
        "MOV C,B\n"         // 74
        "MOV C,C\n"         // 75
        "MOV C,D\n"         // 76
        "MOV C,E\n"         // 77
        "MOV C,H\n"         // 78
        "MOV C,L\n"         // 79
        "MOV C,M\n"         // 80
        "MOV C,A\n"         // 81
        "MOV D,B\n"         // 82
        "MOV D,C\n"         // 83
        "MOV D,D\n"         // 84
        "MOV D,E\n"         // 85
        "MOV D,H\n"         // 86
        "MOV D,L\n"         // 87
        "MOV D,M\n"         // 88
        "MOV D,A\n"         // 89
        "MOV E,B\n"         // 90
        "MOV E,C\n"         // 91
        "MOV E,D\n"         // 92
        "MOV E,E\n"         // 93
        "MOV E,H\n"         // 94
        "MOV E,L\n"         // 95
        "MOV E,M\n"         // 96
        "MOV E,A\n"         // 97
        "MOV H,B\n"         // 98
        "MOV H,C\n"         // 99
        "MOV H,D\n"         // 100
        "MOV H,E\n"         // 101
        "MOV H,H\n"         // 102
        "MOV H,L\n"         // 103
        "MOV H,M\n"         // 104
        "MOV H,A\n"         // 105
        "MOV L,B\n"         // 106
        "MOV L,C\n"         // 107
        "MOV L,D\n"         // 108
        "MOV L,E\n"         // 109
        "MOV L,H\n"         // 110
        "MOV L,L\n"         // 111
        "MOV L,M\n"         // 112
        "MOV L,A\n"         // 113
        "MOV M,B\n"         // 114
        "MOV M,C\n"         // 115
        "MOV M,D\n"         // 116
        "MOV M,E\n"         // 117
        "MOV M,H\n"         // 118
        "MOV M,L\n"         // 119
        "HLT\n"             // 120
        "MOV M,A\n"         // 121
        "MOV A,B\n"         // 122
        "MOV A,C\n"         // 123
        "MOV A,D\n"         // 124
        "MOV A,E\n"         // 125
        "MOV A,H\n"         // 126
        "MOV A,L\n"         // 127
        "MOV A,M\n"         // 128
        "MOV A,A\n"         // 129
        "ADD B\n"           // 130
        "ADD C\n"           // 131
        "ADD D\n"           // 132
        "ADD E\n"           // 133
        "ADD H\n"           // 134
        "ADD L\n"           // 135
        "ADD M\n"           // 136
        "ADD A\n"           // 137
        "ADC B\n"           // 138
        "ADC C\n"           // 139
        "ADC D\n"           // 140
        "ADC E\n"           // 141
        "ADC H\n"           // 142
        "ADC L\n"           // 143
        "ADC M\n"           // 144
        "ADC A\n"           // 145
        "SUB B\n"           // 146
        "SUB C\n"           // 147
        "SUB D\n"           // 148
        "SUB E\n"           // 149
        "SUB H\n"           // 150
        "SUB L\n"           // 151
        "SUB M\n"           // 152
        "SUB A\n"           // 153
        "SBB B\n"           // 154
        "SBB C\n"           // 155
        "SBB D\n"           // 156
        "SBB E\n"           // 157
        "SBB H\n"           // 158
        "SBB L\n"           // 159
        "SBB M\n"           // 160
        "SBB A\n"           // 161
        "ANA B\n"           // 162
        "ANA C\n"           // 163
        "ANA D\n"           // 164
        "ANA E\n"           // 165
        "ANA H\n"           // 166
        "ANA L\n"           // 167
        "ANA M\n"           // 168
        "ANA A\n"           // 169
        "XRA B\n"           // 170
        "XRA C\n"           // 171
        "XRA D\n"           // 172
        "XRA E\n"           // 173
        "XRA H\n"           // 174
        "XRA L\n"           // 175
        "XRA M\n"           // 176
        "XRA A\n"           // 177
        "ORA B\n"           // 178
        "ORA C\n"           // 179
        "ORA D\n"           // 180
        "ORA E\n"           // 181
        "ORA H\n"           // 182
        "ORA L\n"           // 183
        "ORA M\n"           // 184
        "ORA A\n"           // 185
        "CMP B\n"           // 186
        "CMP C\n"           // 187
        "CMP D\n"           // 188
        "CMP E\n"           // 189
        "CMP H\n"           // 190
        "CMP L\n"           // 191
        "CMP M\n"           // 192
        "CMP A\n"           // 193
        "RNZ\n"             // 194
        "POP B\n"           // 195
        "JNZ 0013h\n"       // 196
        "JMP 0014h\n"       // 197
        "CNZ 0015h\n"       // 198
        "PUSH B\n"          // 199
        "ADI 16h\n"         // 200
        "RST 0\n"           // 201
        "RZ\n"              // 202
        "RET\n"             // 203
        "JZ 0017h\n"        // 204
        "RSTV\n"            // 205
        "CZ 0018h\n"        // 206
        "CALL 0019h\n"      // 207
        "ACI 1Ah\n"         // 208
        "RST 1\n"           // 209
        "RNC\n"             // 210
        "POP D\n"           // 211
        "JNC 001Bh\n"       // 212
        "OUT 1Ch\n"         // 213
        "CNC 001Dh\n"       // 214
        "PUSH D\n"          // 215
        "SUI 1Eh\n"         // 216
        "RST 2\n"           // 217
        "RC\n"              // 218
        "SHLX\n"            // 219
        "JC 001Fh\n"        // 220
        "IN 20h\n"          // 221
        "CC 0021h\n"        // 222
        "JNK 0022h\n"       // 223
        "SBI 23h\n"         // 224
        "RST 3\n"           // 225
        "RPO\n"             // 226
        "POP H\n"           // 227
        "JPO 0024h\n"       // 228
        "XTHL\n"            // 229
        "CPO 0025h\n"       // 230
        "PUSH H\n"          // 231
        "ANI 26h\n"         // 232
        "RST 4\n"           // 233
        "RPE\n"             // 234
        "PCHL\n"            // 235
        "JPE 0027h\n"       // 236
        "XCHG\n"            // 237
        "CPE 0028h\n"       // 238
        "LHLX\n"            // 239
        "XRI 29h\n"         // 240
        "RST 5\n"           // 241
        "RP\n"              // 242
        "POP PSW\n"         // 243
        "JP 002Ah\n"        // 244
        "DI\n"              // 245
        "CP 002Bh\n"        // 246
        "PUSH PSW\n"        // 247
        "ORI 2Ch\n"         // 248
        "RST 6\n"           // 249
        "RM\n"              // 250
        "SPHL\n"            // 251
        "JM 002Dh\n"        // 252
        "EI\n"              // 253
        "CM 002Eh\n"        // 254
        "JK 002Fh\n"        // 255
        "CPI 30h\n"         // 256
        "RST 7\n"           // 257
        "END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();

    MachineCode ref
    { 
        0x00, 
        0x01, 0x01, 0x00,
        0x02,
        0x03,
        0x04,
        0x05,
        0x06, 0x02,
        0x07,
        0x08,
        0x09,
        0x0A, 
        0x0B,
        0x0C,
        0x0D,
        0x0E, 0x03,
        0x0F,
        0x10,
        0x11, 0x04, 0x00,
        0x12,
        0x13,
        0x14,
        0x15,
        0x16, 0x05,
        0x17,
        0x18,
        0x19,
        0x1A,
        0x1B,
        0x1C,
        0x1D,
        0x1E, 0x06,
        0x1F,
        0x20,
        0x21, 0x07, 0x00,
        0x22, 0x08, 0x00,
        0x23, 
        0x24,
        0x25,
        0x26, 0x09,
        0x27,
        0x28, 0x0A,
        0x29, 
        0x2A, 0x0B, 0x00,
        0x2B,
        0x2C,
        0x2D,
        0x2E, 0x0C,
        0x2F,
        0x30,
        0x31, 0x0D, 0x00,
        0x32, 0x0E, 0x00,
        0x33,
        0x34,
        0x35,
        0x36, 0x0F,
        0x37,
        0x38, 0x10,
        0x39,
        0x3A, 0x11, 0x00,
        0x3B,
        0x3C,
        0x3D,
        0x3E, 0x12,
        0x3F,
        0x40,
        0x41,
        0x42,
        0x43,
        0x44,
        0x45,
        0x46,
        0x47,
        0x48,
        0x49,
        0x4A,
        0x4B,
        0x4C,
        0x4D,
        0x4E,
        0x4F,
        0x50,
        0x51,
        0x52,
        0x53,
        0x54,
        0x55,
        0x56,
        0x57,
        0x58,
        0x59,
        0x5A,
        0x5B,
        0x5C,
        0x5D,
        0x5E,
        0x5F,
        0x60,
        0x61,
        0x62,
        0x63,
        0x64,
        0x65,
        0x66,
        0x67,
        0x68,
        0x69,
        0x6A,
        0x6B,
        0x6C,
        0x6D,
        0x6E,
        0x6F,
        0x70,
        0x71,
        0x72,
        0x73,
        0x74,
        0x75,
        0x76,
        0x77,
        0x78,
        0x79,
        0x7A,
        0x7B,
        0x7C,
        0x7D,
        0x7E,
        0x7F,
        0x80,
        0x81,
        0x82,
        0x83,
        0x84,
        0x85,
        0x86,
        0x87,
        0x88,
        0x89,
        0x8A,
        0x8B,
        0x8C,
        0x8D,
        0x8E,
        0x8F,
        0x90,
        0x91,
        0x92,
        0x93,
        0x94,
        0x95,
        0x96,
        0x97,
        0x98,
        0x99,
        0x9A,
        0x9B,
        0x9C,
        0x9D,
        0x9E,
        0x9F,
        0xA0,
        0xA1,
        0xA2,
        0xA3,
        0xA4,
        0xA5,
        0xA6,
        0xA7,
        0xA8,
        0xA9,
        0xAA,
        0xAB,
        0xAC,
        0xAD,
        0xAE,
        0xAF,
        0xB0,
        0xB1,
        0xB2,
        0xB3,
        0xB4,
        0xB5,
        0xB6,
        0xB7,
        0xB8,
        0xB9,
        0xBA,
        0xBB,
        0xBC,
        0xBD,
        0xBE,
        0xBF,
        0xC0,
        0xC1,
        0xC2, 0x13, 0x00,
        0xC3, 0x14, 0x00,
        0xC4, 0x15, 0x00,
        0xC5,
        0xC6, 0x16,
        0xC7,
        0xC8,
        0xC9,
        0xCA, 0x17, 0x00,
        0xCB, 
        0xCC, 0x18, 0x00,
        0xCD, 0x19, 0x00,
        0xCE, 0x1A,
        0xCF,
        0xD0,
        0xD1,
        0xD2, 0x1B, 0x00,
        0xD3, 0x1C,
        0xD4, 0x1D, 0x00,
        0xD5, 
        0xD6, 0x1E,
        0xD7,
        0xD8,
        0xD9,
        0xDA, 0x1F, 0x00,
        0xDB, 0x20,
        0xDC, 0x21, 0x00,
        0xDD, 0x22, 0x00,
        0xDE, 0x23,
        0xDF,
        0xE0,
        0xE1,
        0xE2, 0x24, 0x00,
        0xE3,
        0xE4, 0x25, 0x00,
        0xE5,
        0xE6, 0x26,
        0xE7,
        0xE8,
        0xE9,
        0xEA, 0x27, 0x00,
        0xEB,
        0xEC, 0x28, 0x00,
        0xED,
        0xEE, 0x29,
        0xEF,
        0xF0,
        0xF1,
        0xF2, 0x2A, 0x00,
        0xF3,
        0xF4, 0x2B, 0x00,
        0xF5,
        0xF6, 0x2C,
        0xF7,
        0xF8,
        0xF9,
        0xFA, 0x2D, 0x00,
        0xFB,
        0xFC, 0x2E, 0x00,
        0xFD, 0x2F, 0x00,
        0xFE, 0x30,
        0xFF,
    };
    CheckCode(ref, parser);
}

TEST_FIXTURE(ParserTest, Multiply8080)
{
    AssemblerMessages messages;
    std::istringstream inputStream(
        "CPU Intel8080\n"
        "MULT:   MVI B,0     ;INITIALIZE MOST SIGNIFICANT BYTE\n"
        "                    ;OF RESULT\n"
        "        MVI E,9     ;BIT COUNTER\n"
        "MULT0:  MOV A,C     ;ROTATE LEAST SIGNIFICANT BIT OF\n"
        "        RAR         ;MULTIPLIER TO CARRY AND SHIFT\n"
        "        MOV C,A     ;LOW-ORDER BYTE OF RESULT\n"
        "        DCR E\n"
        "        JZ DONE     ;EXIT IF COMPLETE\n"
        "        MOV A,B\n"
        "        JNC MULT1\n"
        "        ADD D       ;ADD MULTIPLICAND TO HIGH-\n"
        "                    ;ORDER BYTE OF RESULT IF BIT\n"
        "                    ;WAS A ONE\n"
        "MULT1:  RAR         ;CARRY=O HERE SHIFT HIGH-\n"
        "                    ;ORDER BYTE OF RESULT\n"
        "        MOV B,A\n"
        "        JMP MULT0\n"
        "DONE:   END\n"
        "END\n");
    std::wostringstream reportStream;
    Scanner scanner(&inputStream, true);
    Parser parser("test", scanner, messages, reportStream);

    parser.Parse();

    EXPECT_EQ(L"                                  CPU Intel8080\n"
              L"0000 06 00    MULT:               MVI B,0             ;INITIALIZE MOST SIGNIFICANT BYTE\n"
              L"                                                      ;OF RESULT\n"
              L"0002 1E 09                        MVI E,9             ;BIT COUNTER\n"
              L"0004 79       MULT0:              MOV A,C             ;ROTATE LEAST SIGNIFICANT BIT OF\n"
              L"0005 1F                           RAR                 ;MULTIPLIER TO CARRY AND SHIFT\n"
              L"0006 4F                           MOV C,A             ;LOW-ORDER BYTE OF RESULT\n"
              L"0007 1D                           DCR E\n"
              L"0008 CA 15 00                     JZ DONE             ;EXIT IF COMPLETE\n"
              L"000B 78                           MOV A,B\n"
              L"000C D2 10 00                     JNC MULT1\n"
              L"000F 82                           ADD D               ;ADD MULTIPLICAND TO HIGH-\n"
              L"                                                      ;ORDER BYTE OF RESULT IF BIT\n"
              L"                                                      ;WAS A ONE\n"
              L"0010 1F       MULT1:              RAR                 ;CARRY=O HERE SHIFT HIGH-\n"
              L"                                                      ;ORDER BYTE OF RESULT\n"
              L"0011 47                           MOV B,A\n"
              L"0012 C3 04 00                     JMP MULT0\n"
              L"              DONE:               END\n", reportStream.str());
    EXPECT_EQ(size_t{ 0 }, messages.size());
    EXPECT_EQ(size_t{ 0 }, parser.NumErrors());
    EXPECT_EQ(size_t{ 0 }, parser.NumWarnings());
    EXPECT_EQ(size_t{ 0 }, parser.NumExceptions());
    ASTree const & ast = parser.GetAST();

    MachineCode ref
    { 
        0x06, 0x00,     // MULT
        0x1E, 0x09,
        0x79,           // MULT0
        0x1F,
        0x4F,
        0x1D,
        0xCA, 0x15, 0x00,
        0x78,
        0xD2, 0x10, 0x00,
        0x82,
        0x1F,           // MULT1
        0x47,
        0xC3, 0x04, 0x00,
                        // DONE
    };
    CheckCode(ref, parser);
}

} // namespace Test

} // namespace Assembler
