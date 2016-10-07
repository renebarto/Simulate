#include "unit-test-c++/UnitTestC++.h"

#include "core/Util.h"
#include "emulator/RAM.h"

using namespace std;

namespace Emulator
{

namespace Test
{

class RAMTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();

    static const size_t Base = 256;
    static const size_t Size = 256;
    static const size_t Origin = 384;
};

void RAMTest::SetUp()
{
}

void RAMTest::TearDown()
{
}

TEST_FIXTURE(RAMTest, Construct)
{
    RAM memory(Base, Size);
    EXPECT_EQ(Base, memory.Offset());
    EXPECT_EQ(Size, memory.Size());
}

TEST_FIXTURE(RAMTest, LoadVector)
{
    std::vector<uint8_t> machineCode =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    RAM memory(Base, Size);
    memory.Load(machineCode, Origin);
    for (size_t index = 0; index < machineCode.size(); ++index)
    {
        EXPECT_EQ(machineCode[index + 0], memory.Fetch8(Origin + index));
    }
}

TEST_FIXTURE(RAMTest, LoadVectorInvalid)
{
    std::vector<uint8_t> machineCode =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    RAM memory(Base, Size);
    EXPECT_THROW(memory.Load(machineCode, Base - 1), std::runtime_error);
    EXPECT_THROW(memory.Load(machineCode, Base + Size), std::runtime_error);
}

TEST_FIXTURE(RAMTest, LoadInitializer)
{
    std::initializer_list<uint8_t> initializer = {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    RAM memory(Base, Size);
    memory.Load(initializer, Origin);
    for (size_t index = 0; index < initializer.size(); ++index)
    {
        EXPECT_EQ(initializer.begin()[index + 0], memory.Fetch8(Origin + index));
    }
}

TEST_FIXTURE(RAMTest, LoadInitializerInvalid)
{
    std::initializer_list<uint8_t> initializer = {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    RAM memory(Base, Size);
    EXPECT_THROW(memory.Load(initializer, Base - 1), std::runtime_error);
    EXPECT_THROW(memory.Load(initializer, Base + Size), std::runtime_error);
}

TEST_FIXTURE(RAMTest, Fetch)
{
    std::vector<uint8_t> machineCode =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    RAM memory(Base, Size);
    memory.Load(machineCode, Origin);
    EXPECT_EQ(machineCode, memory.Fetch(Origin, machineCode.size()));
}

TEST_FIXTURE(RAMTest, Fetch8)
{
    std::vector<uint8_t> machineCode =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    RAM memory(Base, Size);
    memory.Load(machineCode, Origin);
    for (size_t index = 0; index < machineCode.size(); ++index)
    {
        EXPECT_EQ(machineCode[index + 0], memory.Fetch8(Origin + index));
    }
}

TEST_FIXTURE(RAMTest, Fetch16)
{
    std::vector<uint8_t> machineCode =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    RAM memory(Base, Size);
    memory.Load(machineCode, Origin);
    for (size_t index = 0; index < machineCode.size() - 1; index += 2)
    {
        EXPECT_EQ((machineCode[index + 0] << 0) | (machineCode[index + 1] << 8), memory.Fetch16(Origin + index));
    }
}

TEST_FIXTURE(RAMTest, Fetch32)
{
    std::vector<uint8_t> machineCode =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    RAM memory(Base, Size);
    memory.Load(machineCode, Origin);
    for (size_t index = 0; index < machineCode.size() - 3; index += 4)
    {
        EXPECT_EQ((machineCode[index + 0] << 0) | (machineCode[index + 1] << 8) | 
                  (machineCode[index + 2] << 16) | (machineCode[index + 3] << 24), memory.Fetch32(Origin + index));
    }
}

TEST_FIXTURE(RAMTest, Fetch64)
{
    std::vector<uint8_t> machineCode =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    RAM memory(Base, Size);
    memory.Load(machineCode, Origin);
    for (size_t index = 0; index < machineCode.size() - 7; index += 8)
    {
        EXPECT_EQ((uint64_t(machineCode[index + 0]) <<  0) | (uint64_t(machineCode[index + 1]) << 8) | 
                  (uint64_t(machineCode[index + 2]) << 16) | (uint64_t(machineCode[index + 3]) << 24) |
                  (uint64_t(machineCode[index + 4]) << 32) | (uint64_t(machineCode[index + 5]) << 40) |
                  (uint64_t(machineCode[index + 6]) << 48) | (uint64_t(machineCode[index + 7]) << 56), memory.Fetch64(Origin + index));
    }

}

TEST_FIXTURE(RAMTest, FetchInvalidOffset)
{
    RAM memory(Base, Size);
    EXPECT_THROW(memory.Fetch(Base - 1, 1), std::runtime_error);
    EXPECT_THROW(memory.Fetch(Base, Size + 1), std::runtime_error);
    EXPECT_THROW(memory.Fetch(Base + Size, 1), std::runtime_error);
}

TEST_FIXTURE(RAMTest, Fetch8InvalidOffset)
{
    RAM memory(Base, Size);
    EXPECT_THROW(memory.Fetch8(Base - 1), std::runtime_error);
    EXPECT_THROW(memory.Fetch8(Base + Size), std::runtime_error);
}

TEST_FIXTURE(RAMTest, Fetch16InvalidOffset)
{
    RAM memory(Base, Size);
    EXPECT_THROW(memory.Fetch16(Base - 1), std::runtime_error);
    EXPECT_THROW(memory.Fetch16(Base + Size - 1), std::runtime_error);
}

TEST_FIXTURE(RAMTest, Fetch32InvalidOffset)
{
    RAM memory(Base, Size);
    EXPECT_THROW(memory.Fetch32(Base - 1), std::runtime_error);
    EXPECT_THROW(memory.Fetch32(Base + Size - 3), std::runtime_error);
}

TEST_FIXTURE(RAMTest, Fetch64InvalidOffset)
{
    RAM memory(Base, Size);
    EXPECT_THROW(memory.Fetch64(Base - 1), std::runtime_error);
    EXPECT_THROW(memory.Fetch64(Base + Size - 7), std::runtime_error);
}

TEST_FIXTURE(RAMTest, Store)
{
    RAM memory(Base, Size);

    std::vector<uint8_t> data =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    memory.Store(Base, data);
    for (size_t index = 0; index < data.size(); ++index)
    {
        EXPECT_EQ(data[index], memory.Fetch8(Base + index));
    }
}

TEST_FIXTURE(RAMTest, Store8)
{
    RAM memory(Base, Size);

    std::vector<uint8_t> data =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    for (size_t index = 0; index < data.size(); ++index)
    {
        memory.Store8(Base + index, data[index]);
    }
    for (size_t index = 0; index < data.size(); ++index)
    {
        EXPECT_EQ(data[index], memory.Fetch8(Base + index));
    }
}

TEST_FIXTURE(RAMTest, Store16)
{
    RAM memory(Base, Size);

    std::vector<uint16_t> data =
    {
        0x1022, 0x5813, 0x3019, 0x2520, 0x0530, 0x2025, 0x1955, 0x0125,
        0x2014, 0x2400
    };
    for (size_t index = 0; index < data.size(); ++index)
    {
        memory.Store16(Base + index * 2, data[index]);
    }
    for (size_t index = 0; index < data.size(); ++index)
    {
        EXPECT_EQ(data[index], memory.Fetch16(Base + index * 2));
    }
}

TEST_FIXTURE(RAMTest, Store32)
{
    RAM memory(Base, Size);

    std::vector<uint32_t> data =
    {
        0x10225813, 0x30192520, 0x05302025, 0x19550125, 
        0x20142400, 0x58133019, 0x25200530, 0x20251955, 
        0x01252014, 0x24001022
    };
    for (size_t index = 0; index < data.size(); ++index)
    {
        memory.Store32(Base + index * 4, data[index]);
    }
    for (size_t index = 0; index < data.size(); ++index)
    {
        EXPECT_EQ(data[index], memory.Fetch32(Base + index * 4));
    }
}

TEST_FIXTURE(RAMTest, Store64)
{
    RAM memory(Base, Size);

    std::vector<uint64_t> data =
    {
        0x1022581330192520, 0x0530202519550125, 0x2014240010225813, 0x3019252005302025, 
        0x1955012520142400
    };
    for (size_t index = 0; index < data.size(); ++index)
    {
        memory.Store64(Base + index * 8, data[index]);
    }
    for (size_t index = 0; index < data.size(); ++index)
    {
        EXPECT_EQ(data[index], memory.Fetch64(Base + index * 8));
    }
}

TEST_FIXTURE(RAMTest, StoreInvalidOffset)
{
    RAM memory(Base, Size);
    std::vector<uint8_t> data =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    EXPECT_THROW(memory.Store(Base - 1, data), std::runtime_error);
    EXPECT_THROW(memory.Store(Base + Size - (data.size() - 1), data), std::runtime_error);
}

TEST_FIXTURE(RAMTest, Store8InvalidOffset)
{
    RAM memory(Base, Size);
    EXPECT_THROW(memory.Store8(Base - 1, 0x00), std::runtime_error);
    EXPECT_THROW(memory.Store8(Base + Size, 0x00), std::runtime_error);
}

TEST_FIXTURE(RAMTest, Store16InvalidOffset)
{
    RAM memory(Base, Size);
    EXPECT_THROW(memory.Store16(Base - 1, 0x00), std::runtime_error);
    EXPECT_THROW(memory.Store16(Base + Size - 1, 0x00), std::runtime_error);
}

TEST_FIXTURE(RAMTest, Store32InvalidOffset)
{
    RAM memory(Base, Size);
    EXPECT_THROW(memory.Store32(Base - 1, 0x00), std::runtime_error);
    EXPECT_THROW(memory.Store32(Base + Size - 3, 0x00), std::runtime_error);
}

TEST_FIXTURE(RAMTest, Store64InvalidOffset)
{
    RAM memory(Base, Size);
    EXPECT_THROW(memory.Store64(Base - 1, 0x00), std::runtime_error);
    EXPECT_THROW(memory.Store64(Base + Size - 7, 0x00), std::runtime_error);
}

} // namespace Test

} // namespace Emulator
