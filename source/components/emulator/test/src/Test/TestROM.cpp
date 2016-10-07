#include "unit-test-c++/UnitTestC++.h"

#include "core/Util.h"
#include "emulator/ROM.h"

using namespace std;

namespace Emulator
{

namespace Test
{

class ROMTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();

    static const size_t Base = 256;
    static const size_t Size = 256;
    static const size_t Origin = 384;
};

void ROMTest::SetUp()
{
}

void ROMTest::TearDown()
{
}

TEST_FIXTURE(ROMTest, Construct)
{
    ROM memory(Base, Size);
    EXPECT_EQ(Base, memory.Offset());
    EXPECT_EQ(Size, memory.Size());
}

TEST_FIXTURE(ROMTest, LoadVector)
{
    std::vector<uint8_t> machineCode =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    ROM memory(Base, Size);
    memory.Load(machineCode, Origin);
    for (size_t index = 0; index < machineCode.size(); ++index)
    {
        EXPECT_EQ(machineCode[index + 0], memory.Fetch8(Origin + index));
    }
}

TEST_FIXTURE(ROMTest, LoadVectorInvalid)
{
    std::vector<uint8_t> machineCode =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    ROM memory(Base, Size);
    EXPECT_THROW(memory.Load(machineCode, Base - 1), std::runtime_error);
    EXPECT_THROW(memory.Load(machineCode, Base + Size), std::runtime_error);
}

TEST_FIXTURE(ROMTest, LoadInitializer)
{
    std::initializer_list<uint8_t> initializer = {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    ROM memory(Base, Size);
    memory.Load(initializer, Origin);
    for (size_t index = 0; index < initializer.size(); ++index)
    {
        EXPECT_EQ(initializer.begin()[index + 0], memory.Fetch8(Origin + index));
    }
}

TEST_FIXTURE(ROMTest, LoadInitializerInvalid)
{
    std::initializer_list<uint8_t> initializer = {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    ROM memory(Base, Size);
    EXPECT_THROW(memory.Load(initializer, Base - 1), std::runtime_error);
    EXPECT_THROW(memory.Load(initializer, Base + Size), std::runtime_error);
}

TEST_FIXTURE(ROMTest, Fetch)
{
    std::vector<uint8_t> machineCode =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    ROM memory(Base, Size);
    memory.Load(machineCode, Origin);
    EXPECT_EQ(machineCode, memory.Fetch(Origin, machineCode.size()));
}

TEST_FIXTURE(ROMTest, Fetch8)
{
    std::vector<uint8_t> machineCode =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    ROM memory(Base, Size);
    memory.Load(machineCode, Origin);
    for (size_t index = 0; index < machineCode.size(); ++index)
    {
        EXPECT_EQ(machineCode[index + 0], memory.Fetch8(Origin + index));
    }
}

TEST_FIXTURE(ROMTest, Fetch16)
{
    std::vector<uint8_t> machineCode =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    ROM memory(Base, Size);
    memory.Load(machineCode, Origin);
    for (size_t index = 0; index < machineCode.size() - 1; index += 2)
    {
        EXPECT_EQ((machineCode[index + 0] << 0) | (machineCode[index + 1] << 8), memory.Fetch16(Origin + index));
    }
}

TEST_FIXTURE(ROMTest, Fetch32)
{
    std::vector<uint8_t> machineCode =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    ROM memory(Base, Size);
    memory.Load(machineCode, Origin);
    for (size_t index = 0; index < machineCode.size() - 3; index += 4)
    {
        EXPECT_EQ((machineCode[index + 0] << 0) | (machineCode[index + 1] << 8) | 
                  (machineCode[index + 2] << 16) | (machineCode[index + 3] << 24), memory.Fetch32(Origin + index));
    }
}

TEST_FIXTURE(ROMTest, Fetch64)
{
    std::vector<uint8_t> machineCode =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    ROM memory(Base, Size);
    memory.Load(machineCode, Origin);
    for (size_t index = 0; index < machineCode.size() - 7; index += 8)
    {
        EXPECT_EQ((uint64_t(machineCode[index + 0]) <<  0) | (uint64_t(machineCode[index + 1]) << 8) | 
                  (uint64_t(machineCode[index + 2]) << 16) | (uint64_t(machineCode[index + 3]) << 24) |
                  (uint64_t(machineCode[index + 4]) << 32) | (uint64_t(machineCode[index + 5]) << 40) |
                  (uint64_t(machineCode[index + 6]) << 48) | (uint64_t(machineCode[index + 7]) << 56), memory.Fetch64(Origin + index));
    }

}

TEST_FIXTURE(ROMTest, FetchInvalidOffset)
{
    ROM memory(Base, Size);
    EXPECT_THROW(memory.Fetch(Base - 1, 1), std::runtime_error);
    EXPECT_THROW(memory.Fetch(Base, Size + 1), std::runtime_error);
    EXPECT_THROW(memory.Fetch(Base + Size, 1), std::runtime_error);
}

TEST_FIXTURE(ROMTest, Fetch8InvalidOffset)
{
    ROM memory(Base, Size);
    EXPECT_THROW(memory.Fetch8(Base - 1), std::runtime_error);
    EXPECT_THROW(memory.Fetch8(Base + Size), std::runtime_error);
}

TEST_FIXTURE(ROMTest, Fetch16InvalidOffset)
{
    ROM memory(Base, Size);
    EXPECT_THROW(memory.Fetch16(Base - 1), std::runtime_error);
    EXPECT_THROW(memory.Fetch16(Base + Size - 1), std::runtime_error);
}

TEST_FIXTURE(ROMTest, Fetch32InvalidOffset)
{
    ROM memory(Base, Size);
    EXPECT_THROW(memory.Fetch32(Base - 1), std::runtime_error);
    EXPECT_THROW(memory.Fetch32(Base + Size - 3), std::runtime_error);
}

TEST_FIXTURE(ROMTest, Fetch64InvalidOffset)
{
    ROM memory(Base, Size);
    EXPECT_THROW(memory.Fetch64(Base - 1), std::runtime_error);
    EXPECT_THROW(memory.Fetch64(Base + Size - 7), std::runtime_error);
}

TEST_FIXTURE(ROMTest, Store)
{
    ROM memory(Base, Size);
    std::vector<uint8_t> data{ 0x00 };

    EXPECT_THROW(memory.Store(Base, data), std::runtime_error);
}

TEST_FIXTURE(ROMTest, Store8)
{
    ROM memory(Base, Size);

    EXPECT_THROW(memory.Store8(Base, 0x00), std::runtime_error);
}

TEST_FIXTURE(ROMTest, Store16)
{
    ROM memory(Base, Size);

    EXPECT_THROW(memory.Store16(Base, 0x0000), std::runtime_error);
}

TEST_FIXTURE(ROMTest, Store32)
{
    ROM memory(Base, Size);

    EXPECT_THROW(memory.Store32(Base, 0x00000000), std::runtime_error);
}

TEST_FIXTURE(ROMTest, Store64)
{
    ROM memory(Base, Size);

    EXPECT_THROW(memory.Store64(Base, 0x0000000000000000), std::runtime_error);
}

TEST_FIXTURE(ROMTest, StoreInvalidOffset)
{
    ROM memory(Base, Size);
    std::vector<uint8_t> data{ 0x00 };
    EXPECT_THROW(memory.Store(Base - 1, data), std::runtime_error);
    EXPECT_THROW(memory.Store(Base + Size, data), std::runtime_error);
}

TEST_FIXTURE(ROMTest, Store8InvalidOffset)
{
    ROM memory(Base, Size);
    EXPECT_THROW(memory.Store8(Base - 1, 0x00), std::runtime_error);
    EXPECT_THROW(memory.Store8(Base + Size, 0x00), std::runtime_error);
}

TEST_FIXTURE(ROMTest, Store16InvalidOffset)
{
    ROM memory(Base, Size);
    EXPECT_THROW(memory.Store16(Base - 1, 0x00), std::runtime_error);
    EXPECT_THROW(memory.Store16(Base + Size - 1, 0x00), std::runtime_error);
}

TEST_FIXTURE(ROMTest, Store32InvalidOffset)
{
    ROM memory(Base, Size);
    EXPECT_THROW(memory.Store32(Base - 1, 0x00), std::runtime_error);
    EXPECT_THROW(memory.Store32(Base + Size - 3, 0x00), std::runtime_error);
}

TEST_FIXTURE(ROMTest, Store64InvalidOffset)
{
    ROM memory(Base, Size);
    EXPECT_THROW(memory.Store64(Base - 1, 0x00), std::runtime_error);
    EXPECT_THROW(memory.Store64(Base + Size - 7, 0x00), std::runtime_error);
}

} // namespace Test

} // namespace Emulator
