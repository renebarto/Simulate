#include "unit-test-c++/UnitTestC++.h"

#include "Position.h"
using namespace std;

namespace Coco
{

namespace Test
{

class PositionTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void PositionTest::SetUp()
{
}

void PositionTest::TearDown()
{
}

static const size_t BeginOffset = 1;
static const size_t EndOffset = 2;
static const size_t Column = 3;
static const size_t Line = 4;

TEST_FIXTURE(PositionTest, ConstructDefault)
{
    Position pos;
    EXPECT_EQ(size_t{ 0 }, pos.BeginOffset());
    EXPECT_EQ(size_t{ 0 }, pos.EndOffset());
    EXPECT_EQ(size_t{ 0 }, pos.Column());
    EXPECT_EQ(size_t{ 0 }, pos.Line());
    EXPECT_EQ(Position::Null, pos);
}

TEST_FIXTURE(PositionTest, Construct)
{
    Position pos(BeginOffset, EndOffset, Column, Line);
    EXPECT_EQ(BeginOffset, pos.BeginOffset());
    EXPECT_EQ(EndOffset, pos.EndOffset());
    EXPECT_EQ(Column, pos.Column());
    EXPECT_EQ(Line, pos.Line());
    EXPECT_NE(Position::Null, pos);
}

TEST_FIXTURE(PositionTest, ConstructCopy)
{
    Position pos(BeginOffset, EndOffset, Column, Line);
    Position other(pos);

    EXPECT_EQ(BeginOffset, pos.BeginOffset());
    EXPECT_EQ(EndOffset, pos.EndOffset());
    EXPECT_EQ(Column, pos.Column());
    EXPECT_EQ(Line, pos.Line());
    EXPECT_NE(Position::Null, pos);

    EXPECT_EQ(BeginOffset, other.BeginOffset());
    EXPECT_EQ(EndOffset, other.EndOffset());
    EXPECT_EQ(Column, other.Column());
    EXPECT_EQ(Line, other.Line());
    EXPECT_NE(Position::Null, other);
}

TEST_FIXTURE(PositionTest, Assign)
{
    Position pos(BeginOffset, EndOffset, Column, Line);
    Position other;
    
    other = pos;

    EXPECT_EQ(BeginOffset, pos.BeginOffset());
    EXPECT_EQ(EndOffset, pos.EndOffset());
    EXPECT_EQ(Column, pos.Column());
    EXPECT_EQ(Line, pos.Line());
    EXPECT_NE(Position::Null, pos);

    EXPECT_EQ(BeginOffset, other.BeginOffset());
    EXPECT_EQ(EndOffset, other.EndOffset());
    EXPECT_EQ(Column, other.Column());
    EXPECT_EQ(Line, other.Line());
    EXPECT_NE(Position::Null, other);
}

TEST_FIXTURE(PositionTest, OperatorBool)
{
    Position ref1(BeginOffset, EndOffset, Column, Line);
    Position ref2;
    
    EXPECT_TRUE(bool(ref1));
    EXPECT_FALSE(bool(ref2));
}

TEST_FIXTURE(PositionTest, OperatorNot)
{
    Position ref1(BeginOffset, EndOffset, Column, Line);
    Position ref2;
    
    EXPECT_FALSE(!ref1);
    EXPECT_TRUE(!ref2);
}

TEST_FIXTURE(PositionTest, OperatorEquals)
{
    Position pos(BeginOffset, EndOffset, Column, Line);
    Position ref1(BeginOffset, EndOffset, Column, 0);
    Position ref2(BeginOffset, EndOffset, 0, 0);
    Position ref3(BeginOffset, 0, 0, 0);
    Position ref4(0, 0, 0, 0);
    Position ref5;
    Position ref6(BeginOffset, EndOffset, Column, Line);

    EXPECT_TRUE(pos == pos);
    EXPECT_FALSE(pos == ref1);
    EXPECT_FALSE(ref1 == pos);
    EXPECT_FALSE(pos == ref2);
    EXPECT_FALSE(ref2 == pos);
    EXPECT_FALSE(pos == ref3);
    EXPECT_FALSE(ref3 == pos);
    EXPECT_FALSE(pos == ref4);
    EXPECT_FALSE(ref4 == pos);
    EXPECT_FALSE(pos == ref5);
    EXPECT_FALSE(ref5 == pos);
    EXPECT_TRUE(pos == ref6);
    EXPECT_TRUE(ref6 == pos);
    EXPECT_TRUE(ref4 == ref5);
    EXPECT_TRUE(ref5 == ref4);
}

TEST_FIXTURE(PositionTest, OperatorNotEquals)
{
    Position pos(BeginOffset, EndOffset, Column, Line);
    Position ref1(BeginOffset, EndOffset, Column, 0);
    Position ref2(BeginOffset, EndOffset, 0, 0);
    Position ref3(BeginOffset, 0, 0, 0);
    Position ref4(0, 0, 0, 0);
    Position ref5;
    Position ref6(BeginOffset, EndOffset, Column, Line);

    EXPECT_FALSE(pos != pos);
    EXPECT_TRUE(pos != ref1);
    EXPECT_TRUE(ref1 != pos);
    EXPECT_TRUE(pos != ref2);
    EXPECT_TRUE(ref2 != pos);
    EXPECT_TRUE(pos != ref3);
    EXPECT_TRUE(ref3 != pos);
    EXPECT_TRUE(pos != ref4);
    EXPECT_TRUE(ref4 != pos);
    EXPECT_TRUE(pos != ref5);
    EXPECT_TRUE(ref5 != pos);
    EXPECT_FALSE(pos != ref6);
    EXPECT_FALSE(ref6 != pos);
    EXPECT_FALSE(ref4 != ref5);
    EXPECT_FALSE(ref5 != ref4);
}

} // namespace Test

} // namespace Simulate
