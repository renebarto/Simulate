#include "unit-test-c++/UnitTestC++.h"

#include "Nodes.h"

using namespace std;

namespace Assembler
{

namespace Test
{

class NodesTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void NodesTest::SetUp()
{
}

void NodesTest::TearDown()
{
}

static const ASTNodeType NodeType1 = ASTNodeType::Opcode;
static const ASTNodeType NodeType2 = ASTNodeType::Label;
static const std::wstring NodeValue1 = L"NOP";
static const std::wstring NodeValue2 = L"TWO";
static const Location Location1(1, 1);
static const Location Location2(2, 2);
static const uint8_t NodeOpcodeValue1 = 123;
static const int NodeIntValue1 = 4321;

TEST_FIXTURE(NodesTest, CPUNode)
{
    auto node = std::make_shared<CPUNode>(NodeValue1, Location1);

    EXPECT_EQ(ASTNodeType::CPU, node->NodeType());
    EXPECT_EQ(NodeValue1, node->Value());
    EXPECT_EQ(Location1, node->Loc());
    EXPECT_NULL(node->FirstChild());
    EXPECT_NULL(node->Next());

    EXPECT_EQ(L"CPU " + NodeValue1, node->ToString());
}

TEST_FIXTURE(NodesTest, ORGNode)
{
    auto node = std::make_shared<ORGNode>(NodeValue1, Location1);

    EXPECT_EQ(ASTNodeType::ORG, node->NodeType());
    EXPECT_EQ(NodeValue1, node->Value());
    EXPECT_EQ(Location1, node->Loc());
    EXPECT_NULL(node->FirstChild());
    EXPECT_NULL(node->Next());

    EXPECT_EQ(L"ORG " + NodeValue1, node->ToString());
}

TEST_FIXTURE(NodesTest, ENDNode)
{
    auto node = std::make_shared<ENDNode>(NodeValue1, Location1);

    EXPECT_EQ(ASTNodeType::END, node->NodeType());
    EXPECT_EQ(NodeValue1, node->Value());
    EXPECT_EQ(Location1, node->Loc());
    EXPECT_NULL(node->FirstChild());
    EXPECT_NULL(node->Next());

    EXPECT_EQ(L"END", node->ToString());
}

TEST_FIXTURE(NodesTest, LabelNode)
{
    auto node = std::make_shared<LabelNode>(NodeValue1, Location1);

    EXPECT_EQ(ASTNodeType::Label, node->NodeType());
    EXPECT_EQ(NodeValue1, node->Value());
    EXPECT_EQ(Location1, node->Loc());
    EXPECT_NULL(node->FirstChild());
    EXPECT_NULL(node->Next());

    EXPECT_EQ(NodeValue1 + L":", node->ToString());
}

TEST_FIXTURE(NodesTest, StatementNode)
{
    auto label = std::make_shared<LabelNode>(NodeValue2, Location2);
    auto node = std::make_shared<StatementNode>(NodeType1, NodeValue1, label, Location1);

    EXPECT_EQ(NodeType1, node->NodeType());
    EXPECT_EQ(NodeValue1, node->Value());
    EXPECT_EQ(Location1, node->Loc());
    EXPECT_NULL(node->FirstChild());
    EXPECT_NULL(node->Next());

    StatementNode * statementNode = dynamic_cast<StatementNode *>(node.get());
    ASSERT_NOT_NULL(statementNode);
    EXPECT_EQ(label, statementNode->Label());

    EXPECT_EQ(NodeValue1, node->ToString());
}

TEST_FIXTURE(NodesTest, OpcodeNode)
{
    auto label = std::make_shared<LabelNode>(NodeValue2, Location2);
    auto node = std::make_shared<OpcodeNode<uint8_t, int>>(NodeIntValue1, NodeOpcodeValue1, NodeValue1, label, Location1);

    EXPECT_EQ(ASTNodeType::Opcode, node->NodeType());
    EXPECT_EQ(NodeValue1, node->Value());
    EXPECT_EQ(Location1, node->Loc());
    EXPECT_NULL(node->FirstChild());
    EXPECT_NULL(node->Next());

    OpcodeNode<uint8_t, int> * opcodeNode = dynamic_cast<OpcodeNode<uint8_t, int> *>(node.get());
    ASSERT_NOT_NULL(opcodeNode);
    EXPECT_EQ(label, opcodeNode->Label());
    EXPECT_EQ(NodeOpcodeValue1, opcodeNode->Type());
    EXPECT_EQ(NodeIntValue1, opcodeNode->Address());

    EXPECT_EQ(NodeValue1, node->ToString());
}

TEST_FIXTURE(NodesTest, Register8Node)
{
    auto node = std::make_shared<Register8Node<int>>(NodeIntValue1, NodeValue1, Location1);

    EXPECT_EQ(ASTNodeType::Register8, node->NodeType());
    EXPECT_EQ(NodeValue1, node->Value());
    EXPECT_EQ(Location1, node->Loc());
    EXPECT_NULL(node->FirstChild());
    EXPECT_NULL(node->Next());

    Register8Node<int> * regNode = dynamic_cast<Register8Node<int> *>(node.get());
    ASSERT_NOT_NULL(regNode);
    EXPECT_EQ(NodeIntValue1, regNode->RegisterType());

    EXPECT_EQ(NodeValue1, node->ToString());
}

TEST_FIXTURE(NodesTest, Register16Node)
{
    auto node = std::make_shared<Register16Node<int>>(NodeIntValue1, NodeValue1, Location1);

    EXPECT_EQ(ASTNodeType::Register16, node->NodeType());
    EXPECT_EQ(NodeValue1, node->Value());
    EXPECT_EQ(Location1, node->Loc());
    EXPECT_NULL(node->FirstChild());
    EXPECT_NULL(node->Next());

    Register16Node<int> * regNode = dynamic_cast<Register16Node<int> *>(node.get());
    ASSERT_NOT_NULL(regNode);
    EXPECT_EQ(NodeIntValue1, regNode->RegisterType());

    EXPECT_EQ(NodeValue1, node->ToString());
}

TEST_FIXTURE(NodesTest, LocCounterNode)
{
    auto node = std::make_shared<LocCounterNode<int>>(NodeIntValue1, NodeValue1, Location1);

    EXPECT_EQ(ASTNodeType::LocCounter, node->NodeType());
    EXPECT_EQ(NodeValue1, node->Value());
    EXPECT_EQ(Location1, node->Loc());
    EXPECT_NULL(node->FirstChild());
    EXPECT_NULL(node->Next());

    LocCounterNode<int> * locCounterNode = dynamic_cast<LocCounterNode<int> *>(node.get());
    ASSERT_NOT_NULL(locCounterNode);
    EXPECT_EQ(NodeIntValue1, locCounterNode->Address());

    EXPECT_EQ(NodeValue1, node->ToString());
}

TEST_FIXTURE(NodesTest, Data8Node)
{
    auto node = std::make_shared<Data8Node>(NodeValue1, Location1);

    EXPECT_EQ(ASTNodeType::Data8, node->NodeType());
    EXPECT_EQ(NodeValue1, node->Value());
    EXPECT_EQ(Location1, node->Loc());
    EXPECT_NULL(node->FirstChild());
    EXPECT_NULL(node->Next());

    Data8Node * dataNode = dynamic_cast<Data8Node *>(node.get());
    ASSERT_NOT_NULL(dataNode);
    EXPECT_EQ(NodeValue1, dataNode->Data());

    EXPECT_EQ(NodeValue1, node->ToString());
}

TEST_FIXTURE(NodesTest, Data16Node)
{
    auto node = std::make_shared<Data16Node>(NodeValue1, Location1);

    EXPECT_EQ(ASTNodeType::Data16, node->NodeType());
    EXPECT_EQ(NodeValue1, node->Value());
    EXPECT_EQ(Location1, node->Loc());
    EXPECT_NULL(node->FirstChild());
    EXPECT_NULL(node->Next());

    Data16Node * dataNode = dynamic_cast<Data16Node *>(node.get());
    ASSERT_NOT_NULL(dataNode);
    EXPECT_EQ(NodeValue1, dataNode->Data());

    EXPECT_EQ(NodeValue1, node->ToString());
}

} // namespace Test

} // namespace Assembler
