#include "unit-test-c++/UnitTestC++.h"

#include "assembler/Location.h"

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
    EXPECT_EQ(size_t{ 0 }, location.GetLine());
    EXPECT_EQ(size_t{ 0 }, location.GetColumn());
    EXPECT_EQ(size_t{ 0 }, location.GetCharPos());
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

TEST_FIXTURE(LocationTest, Equal)
{
    Location location(Line, Column, Pos);
    Location ref1(Line, Column, Pos);
    Location ref2(0, Column, Pos);
    Location ref3(Line, 0, Pos);
    Location ref4(Line, Column, 0);
    
    EXPECT_TRUE(location == ref1);
    EXPECT_FALSE(location == ref2);
    EXPECT_FALSE(location == ref3);
    EXPECT_FALSE(location == ref4);
}

TEST_FIXTURE(LocationTest, NotEqual)
{
    Location location(Line, Column, Pos);
    Location ref1(Line, Column, Pos);
    Location ref2(0, Column, Pos);
    Location ref3(Line, 0, Pos);
    Location ref4(Line, Column, 0);
    
    EXPECT_FALSE(location != ref1);
    EXPECT_TRUE(location != ref2);
    EXPECT_TRUE(location != ref3);
    EXPECT_TRUE(location != ref4);
}

TEST_FIXTURE(LocationTest, StreamOperatorString)
{
    std::ostringstream stream;

    Location location(Line, Column, Pos);
    stream << location;

    std::ostringstream ref;
    ref << Line << ":" << Column;
    EXPECT_EQ(ref.str(), stream.str());
}

TEST_FIXTURE(LocationTest, StreamOperatorwString)
{
    std::wostringstream stream;

    Location location(Line, Column, Pos);
    stream << location;

    std::wostringstream ref;
    ref << Line << L":" << Column;
    EXPECT_EQ(ref.str(), stream.str());
}

} // namespace Test

} // namespace Assembler
