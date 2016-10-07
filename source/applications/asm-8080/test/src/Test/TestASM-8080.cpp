#include "unit-test-c++/UnitTestC++.h"

#include <sstream>
#include "ASM-8080.h"
#include "asm-8080\TestData.h"

using namespace std;

namespace ASM8080
{

namespace Test
{

class ASM_8080Test : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void ASM_8080Test::SetUp()
{
}

void ASM_8080Test::TearDown()
{
}

TEST_FIXTURE(ASM_8080Test, ParseEmpty)
{
    EXPECT_TRUE(false);
}

} // namespace Test

} // namespace Simulate
