#include "unit-test-c++/UnitTestC++.h"

#include <fstream>
#include "Reader.h"
#include "asm-8080\TestData.h"

using namespace std;

namespace ASM8080
{

namespace Test
{

class ReaderTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void ReaderTest::SetUp()
{
}

void ReaderTest::TearDown()
{
}

TEST_FIXTURE(ReaderTest, Construct)
{
    std::ifstream stream(TestData::InputFile());
    Reader reader(stream);
}

} // namespace Test

} // namespace Simulate
