#include "unit-test-c++/UnitTestC++.h"

#include "Location.h"

using namespace std;

namespace Assembler
{

namespace Test
{

class LocationTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void LocationTest::SetUp()
{
}

void LocationTest::TearDown()
{
}

static const size_t Line = 1;
static const size_t Column = 2;
static const size_t Pos = 3;

TEST_FIXTURE(LocationTest, ConstructDefault)
{
    Location location;
    EXPECT_EQ(size_t{ 1 }, location.GetLine());
    EXPECT_EQ(size_t{ 0 }, location.GetColumn());
    EXPECT_EQ(size_t{ unsigned long long(-1) }, location.GetCharPos());
}

TEST_FIXTURE(LocationTest, Construct)
{
    Location location(Line, Column, Pos);
    EXPECT_EQ(Line, location.GetLine());
    EXPECT_EQ(Column, location.GetColumn());
    EXPECT_EQ(Pos, location.GetCharPos());
}

TEST_FIXTURE(LocationTest, ConstructCopy)
{
    Location location(Line, Column, Pos);
    Location other(location);
    EXPECT_EQ(Line, location.GetLine());
    EXPECT_EQ(Column, location.GetColumn());
    EXPECT_EQ(Pos, location.GetCharPos());
    EXPECT_EQ(Line, other.GetLine());
    EXPECT_EQ(Column, other.GetColumn());
    EXPECT_EQ(Pos, other.GetCharPos());
}

TEST_FIXTURE(LocationTest, Assign)
{
    Location location(Line, Column, Pos);
    Location other;
    
    other = location;
    EXPECT_EQ(Line, location.GetLine());
    EXPECT_EQ(Column, location.GetColumn());
    EXPECT_EQ(Pos, location.GetCharPos());
    EXPECT_EQ(Line, other.GetLine());
    EXPECT_EQ(Column, other.GetColumn());
    EXPECT_EQ(Pos, other.GetCharPos());
}

TEST_FIXTURE(LocationTest, PreIncrement)
{
    Location location(Line, Column, Pos);
    Location other = ++location;
    
    EXPECT_EQ(Line, location.GetLine());
    EXPECT_EQ(Column + 1, location.GetColumn());
    EXPECT_EQ(Pos + 1, location.GetCharPos());
    EXPECT_EQ(Line, other.GetLine());
    EXPECT_EQ(Column + 1, other.GetColumn());
    EXPECT_EQ(Pos + 1, other.GetCharPos());
}

TEST_FIXTURE(LocationTest, PostIncrement)
{
    Location location(Line, Column, Pos);
    Location other = location++;
    
    EXPECT_EQ(Line, location.GetLine());
    EXPECT_EQ(Column + 1, location.GetColumn());
    EXPECT_EQ(Pos + 1, location.GetCharPos());
    EXPECT_EQ(Line, other.GetLine());
    EXPECT_EQ(Column, other.GetColumn());
    EXPECT_EQ(Pos, other.GetCharPos());
}

TEST_FIXTURE(LocationTest, NewLine)
{
    Location location(Line, Column, Pos);
    location.NewLine();
    
    EXPECT_EQ(Line + 1, location.GetLine());
    EXPECT_EQ(size_t{ 0 }, location.GetColumn());
    EXPECT_EQ(Pos, location.GetCharPos());
}

} // namespace Test

} // namespace Assembler
