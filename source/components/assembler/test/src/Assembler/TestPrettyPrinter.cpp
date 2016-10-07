#include "unit-test-c++/UnitTestC++.h"

#include "assembler/PrettyPrinter.h"

using namespace std;

namespace Assembler
{

namespace Test
{

class PrettyPrinterTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void PrettyPrinterTest::SetUp()
{
}

void PrettyPrinterTest::TearDown()
{
}

static const size_t Line = 1;
static const size_t Column = 2;
static const size_t Pos = 3;

TEST_FIXTURE(PrettyPrinterTest, ConstructDefault)
{
    std::wostringstream stream;
    PrettyPrinter<wchar_t> printer(stream);

    EXPECT_EQ(L"", stream.str());
}

TEST_FIXTURE(PrettyPrinterTest, StreamOperator)
{
    std::wostringstream stream;
    PrettyPrinter<wchar_t> printer(stream);

    printer << L"ABCD";

    EXPECT_EQ(L"ABCD", stream.str());
}

TEST_FIXTURE(PrettyPrinterTest, StreamOperatorWithColumnSame)
{
    std::wostringstream stream;
    PrettyPrinter<wchar_t> printer(stream);

    printer << L"ABCD";

    printer << column(5) << L"EFGH";

    EXPECT_EQ(L"ABCDEFGH", stream.str());
}

TEST_FIXTURE(PrettyPrinterTest, StreamOperatorWithColumnSmaller)
{
    std::wostringstream stream;
    PrettyPrinter<wchar_t> printer(stream);

    printer << L"ABCD";

    printer << column(1) << L"EFGH";

    EXPECT_EQ(L"ABCDEFGH", stream.str());
}

TEST_FIXTURE(PrettyPrinterTest, StreamOperatorWithColumnLarger)
{
    std::wostringstream stream;
    PrettyPrinter<wchar_t> printer(stream);

    printer << L"ABCD";

    printer << column(6) << L"EFGH";

    EXPECT_EQ(L"ABCD EFGH", stream.str());
}

TEST_FIXTURE(PrettyPrinterTest, StreamOperatorWithColumnAfterNewline)
{
    std::wostringstream stream;
    PrettyPrinter<wchar_t> printer(stream);

    printer << L"ABCD" << std::endl;

    printer << column(6) << L"EFGH";

    EXPECT_EQ(L"ABCD\n     EFGH", stream.str());
}

} // namespace Test

} // namespace Assembler
