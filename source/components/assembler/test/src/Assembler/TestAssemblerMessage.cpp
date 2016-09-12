#include "unit-test-c++/UnitTestC++.h"

#include "AssemblerMessage.h"

using namespace std;

namespace Assembler
{

namespace Test
{

class AssemblerMessageTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void AssemblerMessageTest::SetUp()
{
}

void AssemblerMessageTest::TearDown()
{
}

static const size_t Line = 1;
static const size_t Column = 2;
static const std::wstring Message = L"ABC";

TEST_FIXTURE(AssemblerMessageTest, ConstructDefault)
{
    Location location(Line, Column);
    AssemblerMessage message(location, Message);
    EXPECT_EQ(Line, message.GetLocation().GetLine());
    EXPECT_EQ(Column, message.GetLocation().GetColumn());
    EXPECT_EQ(Message, message.GetMessage());
}

} // namespace Test

} // namespace Assembler
