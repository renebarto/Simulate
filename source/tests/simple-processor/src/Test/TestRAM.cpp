#include "unit-test-c++/UnitTestC++.h"

#include "core/Util.h"
#include "simple-processor/ram.h"

using namespace std;

namespace Simulate
{

namespace Test
{

static const size_t Base = 256;
static const size_t Size = 256;

class RAMTest : public UnitTestCpp::TestFixture
{
public:
    RAMTest()
        : memory(Base, Size)
    {}

	virtual void SetUp();
	virtual void TearDown();

    RAM<uint8_t> memory;
};

void RAMTest::SetUp()
{
}

void RAMTest::TearDown()
{
}

TEST_FIXTURE(RAMTest, Construct)
{
    EXPECT_EQ(Base, memory.Base());
    EXPECT_EQ(Size, memory.Size());
}

TEST_FIXTURE(RAMTest, ConstructVector)
{
    std::vector<uint8_t> machineCode =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    RAM<uint8_t> memory(Base, Size, machineCode);
    std::vector<uint8_t> actual = memory.Fetch(Base, Size);
    ASSERT_TRUE(Core::Util::Compare(machineCode, actual, machineCode.size()));
}

TEST_FIXTURE(RAMTest, ConstructInitializer)
{
    std::initializer_list<uint8_t> initializer = {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    RAM<uint8_t> memory(Base, Size, initializer);
    std::vector<uint8_t> expected(initializer);
    std::vector<uint8_t> actual = memory.Fetch(Base, Size);
    ASSERT_TRUE(Core::Util::Compare(expected, actual, expected.size()));
}

TEST_FIXTURE(RAMTest, Clear)
{
    std::vector<uint8_t> machineCode =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    RAM<uint8_t> memory(Base, Size, machineCode);
    std::vector<uint8_t> expected(Size);
    std::copy(machineCode.begin(), machineCode.end(), expected.begin());
    std::vector<uint8_t> actual = memory.Fetch(Base, Size);
    ASSERT_TRUE(Core::Util::Compare(expected, actual));

    memory.Clear();
    expected = std::vector<uint8_t>(Size);
    actual = memory.Fetch(Base, Size);
    ASSERT_TRUE(Core::Util::Compare(expected, actual));
}

TEST_FIXTURE(RAMTest, FetchVector)
{
    std::vector<uint8_t> expected(Size);
    std::vector<uint8_t> actual = memory.Fetch(Base, Size);
    ASSERT_TRUE(Core::Util::Compare(expected, actual));

    for (size_t index = 0; index < expected.size(); ++index)
    {
        expected[index] = uint8_t(index);
    }
    RAM<uint8_t> filledMemory(Base, Size, expected);
    actual = filledMemory.Fetch(Base, Size);
    ASSERT_TRUE(Core::Util::Compare(expected, actual));
}

TEST_FIXTURE(RAMTest, FetchVectorInvalidRegion)
{
    EXPECT_THROW(memory.Fetch(Base - 1, Size), MemoryRangeOutOfRangeException);
    EXPECT_THROW(memory.Fetch(Base + 1, Size), MemoryRangeOutOfRangeException);
    EXPECT_THROW(memory.Fetch(Base, Size + 1), MemoryRangeOutOfRangeException);
}

TEST_FIXTURE(RAMTest, StoreVector)
{
    std::vector<uint8_t> expected(Size);
    std::vector<uint8_t> actual = memory.Fetch(Base, Size);
    ASSERT_TRUE(Core::Util::Compare(expected, actual));

    for (size_t index = 0; index < expected.size(); ++index)
    {
        expected[index] = uint8_t(index);
    }
    memory.Store(Base, expected);
    actual = memory.Fetch(Base, Size);
    ASSERT_TRUE(Core::Util::Compare(expected, actual));
}

TEST_FIXTURE(RAMTest, StoreVectorInvalidRegion)
{
    std::vector<uint8_t> data(Size);
    std::vector<uint8_t> dataLong(Size + 1);
    EXPECT_THROW(memory.Store(Base - 1, data), MemoryRangeOutOfRangeException);
    EXPECT_THROW(memory.Store(Base + 1, data), MemoryRangeOutOfRangeException);
    EXPECT_THROW(memory.Store(Base, dataLong), MemoryRangeOutOfRangeException);
}

TEST_FIXTURE(RAMTest, Fetch)
{
    EXPECT_EQ(uint8_t{ 0 }, memory.Fetch(Base));
    EXPECT_EQ(uint8_t{ 0 }, memory.Fetch(Base + Size - 1));
}

TEST_FIXTURE(RAMTest, Store)
{
    EXPECT_EQ(uint8_t{ 0 }, memory.Fetch(Base));
    EXPECT_EQ(uint8_t{ 0 }, memory.Fetch(Base + Size - 1));

    for (size_t address = Base; address < Base + Size; ++address)
    {
        memory.Store(address, address % 256);
    }

    EXPECT_EQ(uint8_t{ 0 }, memory.Fetch(Base));
    EXPECT_EQ(uint8_t{ 255 }, memory.Fetch(Base + Size - 1));
}

TEST_FIXTURE(RAMTest, FetchOutsideValidRegion)
{
    EXPECT_THROW(memory.Fetch(Base - 1), MemoryOutOfRangeException);
    EXPECT_THROW(memory.Fetch(Base + Size), MemoryOutOfRangeException);
}

TEST_FIXTURE(RAMTest, StoreOutsideValidRegion)
{
    EXPECT_THROW(memory.Store(Base - 1, uint8_t{ 0 }), MemoryOutOfRangeException);
    EXPECT_THROW(memory.Store(Base + Size, uint8_t{ 0 }), MemoryOutOfRangeException);
}

TEST_FIXTURE(RAMTest, ListCode)
{
    std::vector<uint8_t> machineCode =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    RAM<uint8_t> memory(Base, Size, machineCode);

    std::string expected = "Base: 256 Size: 256\n"
        "Address   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 0 1 2 3 4 5 6 7 8 9 A B C D E F \n"
        "00000100  10 22 58 13 30 19 25 20 05 30 20 25 19 55 01 25 ? \" X ? 0 ? %   ? 0   % ? U ? % \n"
        "00000110  20 14 24 00 00 00 00 00 00 00 00 00 00 00 00 00   ? $ ? ? ? ? ? ? ? ? ? ? ? ? ? \n"
        "00000120  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? \n"
        "00000130  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? \n"
        "00000140  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? \n"
        "00000150  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? \n"
        "00000160  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? \n"
        "00000170  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? \n"
        "00000180  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? \n"
        "00000190  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? \n"
        "000001A0  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? \n"
        "000001B0  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? \n"
        "000001C0  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? \n"
        "000001D0  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? \n"
        "000001E0  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? \n"
        "000001F0  00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? \n\n";
    std::ostringstream stream;
    memory.DisplayContents(stream);
    std::string actual = stream.str();
    EXPECT_EQ(expected, actual);

}

} // namespace Test

} // namespace Simulate
