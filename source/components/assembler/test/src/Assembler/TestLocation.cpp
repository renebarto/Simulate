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

TEST_FIXTURE(LocationTest, ConstructDefault)
{
    Location location(Line, Column);
    EXPECT_EQ(Line, location.GetLine());
    EXPECT_EQ(Column, location.GetColumn());
}

TEST_FIXTURE(LocationTest, ConstructCopy)
{
    Location location(Line, Column);
    Location other(location);
    EXPECT_EQ(Line, location.GetLine());
    EXPECT_EQ(Column, location.GetColumn());
    EXPECT_EQ(Line, other.GetLine());
    EXPECT_EQ(Column, other.GetColumn());
}

TEST_FIXTURE(LocationTest, Assign)
{
    Location location(Line, Column);
    Location other;
    
    other = location;
    EXPECT_EQ(Line, location.GetLine());
    EXPECT_EQ(Column, location.GetColumn());
    EXPECT_EQ(Line, other.GetLine());
    EXPECT_EQ(Column, other.GetColumn());
}

} // namespace Test

} // namespace Assembler
