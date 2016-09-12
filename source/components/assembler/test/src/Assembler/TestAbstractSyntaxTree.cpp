#include "unit-test-c++/UnitTestC++.h"

#include "AbstractSyntaxTree.h"

using namespace std;

namespace Assembler
{

namespace Test
{

class AbstractSyntaxTreeTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void AbstractSyntaxTreeTest::SetUp()
{
}

void AbstractSyntaxTreeTest::TearDown()
{
}

static const ASTNodeType NodeType1 = ASTNodeType::Opcode;
static const ASTNodeType NodeType2 = ASTNodeType::Label;
static const std::wstring NodeValue1 = L"NOP";
static const std::wstring NodeValue2 = L"TWO";
static const Location Location1(1, 1);
static const Location Location2(2, 2);

TEST_FIXTURE(AbstractSyntaxTreeTest, TreeConstructDefault)
{
    ASTree tree;
    EXPECT_NULL(tree.FirstNode());
}

TEST_FIXTURE(AbstractSyntaxTreeTest, TreeAddNode)
{
    auto node = std::make_shared<ASTNode>(NodeType1, NodeValue1, Location1);
    ASTree tree;
    tree.AddNode(node);
    EXPECT_EQ(node, tree.FirstNode());
}

TEST_FIXTURE(AbstractSyntaxTreeTest, NodeConstruct)
{
    auto node = std::make_shared<ASTNode>(NodeType1, NodeValue1, Location1);

    EXPECT_EQ(NodeType1, node->NodeType());
    EXPECT_EQ(NodeValue1, node->Value());
    EXPECT_EQ(Location1, node->Loc());
    EXPECT_NULL(node->FirstChild());
    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(AbstractSyntaxTreeTest, AddChild)
{
    auto node = std::make_shared<ASTNode>(NodeType1, NodeValue1, Location1);
    auto node2 = std::make_shared<ASTNode>(NodeType2, NodeValue2, Location2);

    node->AddChild(node2);
    EXPECT_EQ(NodeType1, node->NodeType());
    EXPECT_EQ(NodeValue1, node->Value());
    EXPECT_EQ(Location1, node->Loc());
    EXPECT_EQ(node2, node->FirstChild());
    EXPECT_NULL(node->Next());
}

TEST_FIXTURE(AbstractSyntaxTreeTest, AddSibling)
{
    auto node = std::make_shared<ASTNode>(NodeType1, NodeValue1, Location1);
    auto node2 = std::make_shared<ASTNode>(NodeType2, NodeValue2, Location2);

    node->AddSibling(node2);
    EXPECT_EQ(NodeType1, node->NodeType());
    EXPECT_EQ(NodeValue1, node->Value());
    EXPECT_EQ(Location1, node->Loc());
    EXPECT_NULL(node->FirstChild());
    EXPECT_EQ(node2, node->Next());
}

} // namespace Test

} // namespace Assembler
