#include "unit-test-c++/UnitTestC++.h"

#include "emulator/CPUEmulatorIntel8080.h"

using namespace std;

namespace Emulator
{

namespace Test
{

class CPUEmulatorIntel8080Test : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void CPUEmulatorIntel8080Test::SetUp()
{
}

void CPUEmulatorIntel8080Test::TearDown()
{
}

TEST_FIXTURE(CPUEmulatorIntel8080Test, Construct)
{
    std::wostringstream stream;
    Assembler::PrettyPrinter<wchar_t> printer(stream);
    ObjectFile::ObjectCode code("test");
    CPUEmulatorIntel8080 emulator(code, printer);
}

} // namespace Test

} // namespace Simulate
