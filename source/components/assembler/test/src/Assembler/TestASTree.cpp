#include "unit-test-c++/UnitTestC++.h"

#include "AbstractSyntaxTree.h"

using namespace std;

namespace Assembler
{

namespace Test
{

class ASTreeTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void ASTreeTest::SetUp()
{
}

void ASTreeTest::TearDown()
{
}

static const ASTNodeType NodeType1 = ASTNodeType::Opcode;
static const ASTNodeType NodeType2 = ASTNodeType::Label;
static const std::wstring NodeValue1 = L"NOP";
static const std::wstring NodeValue2 = L"TWO";
static const Location Location1(1, 1);
static const Location Location2(2, 2);

TEST_FIXTURE(ASTreeTest, TreeConstructDefault)
{
    ASTree tree;
    EXPECT_NULL(tree.FirstNode());
}

TEST_FIXTURE(ASTreeTest, TreeAddNode)
{
    auto node = std::make_shared<ASTNode>(NodeType1, NodeValue1, Location1);
    ASTree tree;
    tree.AddNode(node);
    EXPECT_EQ(node, tree.FirstNode());
}

} // namespace Test

} // namespace Assembler
