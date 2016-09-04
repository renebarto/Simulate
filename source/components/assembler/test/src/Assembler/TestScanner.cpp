#include "unit-test-c++/UnitTestC++.h"

#include "Scanner.h"

using namespace std;

namespace Assembler
{

namespace Test
{

class ScannerTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void ScannerTest::SetUp()
{
}

void ScannerTest::TearDown()
{
}

TEST_FIXTURE(ScannerTest, ConstructStream)
{
    std::istringstream stream("ABC");
    Scanner scanner(&stream, true);
}

TEST_FIXTURE(ScannerTest, ConstructStreamNull)
{
    EXPECT_THROW(Scanner scanner(nullptr, true), std::invalid_argument);
}

} // namespace Test

} // namespace Assembler
