#include "unit-test-c++/UnitTestC++.h"

#include "osal.h"

using namespace std;

namespace OSAL
{

namespace Test
{

class OSALTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void OSALTest::SetUp()
{
}

void OSALTest::TearDown()
{
}

TEST_FIXTURE(OSALTest, SwapBytesUint8)
{
	uint8_t source = 0x12;
	uint8_t expected = 0x12;
	uint8_t actual = OSAL::SwapBytes(source);
	EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(OSALTest, SwapBytesInt8)
{
	int8_t source = 0x12;
	int8_t expected = 0x12;
	int8_t actual = OSAL::SwapBytes(source);
	EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(OSALTest, SwapBytesUint16)
{
	uint16_t source = 0x1234;
	uint16_t expected = 0x3412;
	uint16_t actual = OSAL::SwapBytes(source);
	EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(OSALTest, SwapBytesInt16)
{
	int16_t source = 0x1234;
	int16_t expected = 0x3412;
	int16_t actual = OSAL::SwapBytes(source);
	EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(OSALTest, SwapBytesUint32)
{
	uint32_t source = 0x12345678;
	uint32_t expected = 0x78563412;
	uint32_t actual = OSAL::SwapBytes(source);
	EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(OSALTest, SwapBytesInt32)
{
	int32_t source = 0x12345678;
	int32_t expected = 0x78563412;
	int32_t actual = OSAL::SwapBytes(source);
	EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(OSALTest, SwapBytesUint64)
{
	uint64_t source = 0x123456789ABCDEF0;
	uint64_t expected = 0xF0DEBC9A78563412;
	uint64_t actual = OSAL::SwapBytes(source);
	EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(OSALTest, SwapBytesInt64)
{
	int64_t source = 0x123456789ABCDEF0;
	int64_t expected = 0xF0DEBC9A78563412;
	int64_t actual = OSAL::SwapBytes(source);
	EXPECT_EQ(expected, actual);
}

} // namespace Test

} // namespace OSAL
