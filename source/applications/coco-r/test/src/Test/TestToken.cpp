#include "unit-test-c++/UnitTestC++.h"

using namespace std;

namespace Coco
{

namespace Test
{

class TokenTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void TokenTest::SetUp()
{
}

void TokenTest::TearDown()
{
}

TEST_FIXTURE(TokenTest, Construct)
{
    FAIL();
}

} // namespace Test

} // namespace Simulate
