#include "unit-test-c++/UnitTestC++.h"

#include "assembler/CharClass.h"

using namespace std;

namespace Assembler
{

namespace Test
{

class CharClassTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void CharClassTest::SetUp()
{
}

void CharClassTest::TearDown()
{
}

TEST_FIXTURE(CharClassTest, ConstructDefault)
{
    const std::wstring Name = L"name";
    CharSet set;
    set.Fill();
    const size_t ID = 1;
    CharClass _class(Name, set, ID);
    EXPECT_EQ(Name, _class.GetName());
    EXPECT_EQ(set, _class.GetCharSet());
    EXPECT_EQ(ID, _class.GetClassID());
}

} // namespace Test

} // namespace Assembler
