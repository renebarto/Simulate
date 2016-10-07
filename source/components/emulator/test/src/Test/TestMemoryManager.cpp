#include "unit-test-c++/UnitTestC++.h"

#include "core/Util.h"
#include "emulator/MemoryManager.h"
#include "emulator/ROM.h"
#include "emulator/RAM.h"

using namespace std;

namespace Emulator
{

namespace Test
{

class MemoryManagerTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();

    static const size_t BaseROM = 256;
    static const size_t SizeROM = 256;
    static const size_t BaseRAM = 512;
    static const size_t SizeRAM = 512;
    static const size_t OriginROM = 384;
    static const size_t OriginRAM = 768;
};

void MemoryManagerTest::SetUp()
{
}

void MemoryManagerTest::TearDown()
{
}

TEST_FIXTURE(MemoryManagerTest, Construct)
{
    MemoryManager memory;
    EXPECT_EQ(size_t(-1), memory.Offset());
    EXPECT_EQ(0, memory.Size());
}

TEST_FIXTURE(MemoryManagerTest, AddMemory)
{
    MemoryManager memory;
    memory.AddMemory(std::make_shared<ROM>(BaseROM, SizeROM));
    memory.AddMemory(std::make_shared<RAM>(BaseRAM, SizeRAM));
    EXPECT_EQ(BaseROM, memory.Offset());
    EXPECT_EQ(SizeROM + SizeRAM, memory.Size());
}

TEST_FIXTURE(MemoryManagerTest, Fetch)
{
    std::vector<uint8_t> code =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    std::vector<uint8_t> data =
    {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
        0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18
    };
    MemoryManager memory;
    ROMPtr rom = std::make_shared<ROM>(BaseROM, SizeROM);
    RAMPtr ram = std::make_shared<RAM>(BaseRAM, SizeRAM);
    memory.AddMemory(rom);
    memory.AddMemory(ram);
    rom->Load(code, OriginROM);
    ram->Load(data, OriginRAM);
    EXPECT_EQ(code, memory.Fetch(OriginROM, code.size()));
    EXPECT_EQ(data, memory.Fetch(OriginRAM, data.size()));
}

TEST_FIXTURE(MemoryManagerTest, Fetch8)
{
    std::vector<uint8_t> code =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    std::vector<uint8_t> data =
    {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
        0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18
    };
    MemoryManager memory;
    ROMPtr rom = std::make_shared<ROM>(BaseROM, SizeROM);
    RAMPtr ram = std::make_shared<RAM>(BaseRAM, SizeRAM);
    memory.AddMemory(rom);
    memory.AddMemory(ram);
    rom->Load(code, OriginROM);
    ram->Load(data, OriginRAM);
    for (size_t index = 0; index < code.size(); ++index)
    {
        EXPECT_EQ(code[index + 0], memory.Fetch8(OriginROM + index));
    }
    for (size_t index = 0; index < data.size(); ++index)
    {
        EXPECT_EQ(data[index + 0], memory.Fetch8(OriginRAM + index));
    }
}

TEST_FIXTURE(MemoryManagerTest, Fetch16)
{
    std::vector<uint8_t> code =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    std::vector<uint8_t> data =
    {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
        0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18
    };
    MemoryManager memory;
    ROMPtr rom = std::make_shared<ROM>(BaseROM, SizeROM);
    RAMPtr ram = std::make_shared<RAM>(BaseRAM, SizeRAM);
    memory.AddMemory(rom);
    memory.AddMemory(ram);
    rom->Load(code, OriginROM);
    ram->Load(data, OriginRAM);
    for (size_t index = 0; index < code.size() - 1; index += 2)
    {
        EXPECT_EQ((code[index + 0] << 0) | (code[index + 1] << 8), memory.Fetch16(OriginROM + index));
    }
    for (size_t index = 0; index < data.size() - 1; index += 2)
    {
        EXPECT_EQ((data[index + 0] << 0) | (data[index + 1] << 8), memory.Fetch16(OriginRAM + index));
    }
}

TEST_FIXTURE(MemoryManagerTest, Fetch32)
{
    std::vector<uint8_t> code =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    std::vector<uint8_t> data =
    {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
        0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18
    };
    MemoryManager memory;
    ROMPtr rom = std::make_shared<ROM>(BaseROM, SizeROM);
    RAMPtr ram = std::make_shared<RAM>(BaseRAM, SizeRAM);
    memory.AddMemory(rom);
    memory.AddMemory(ram);
    rom->Load(code, OriginROM);
    ram->Load(data, OriginRAM);
    for (size_t index = 0; index < code.size() - 3; index += 4)
    {
        EXPECT_EQ((code[index + 0] <<  0) | (code[index + 1] << 8) | 
                  (code[index + 2] << 16) | (code[index + 3] << 24), memory.Fetch32(OriginROM + index));
    }
    for (size_t index = 0; index < code.size() - 3; index += 4)
    {
        EXPECT_EQ((data[index + 0] <<  0) | (data[index + 1] << 8) | 
                  (data[index + 2] << 16) | (data[index + 3] << 24), memory.Fetch32(OriginRAM + index));
    }
}

TEST_FIXTURE(MemoryManagerTest, Fetch64)
{
    std::vector<uint8_t> code =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    std::vector<uint8_t> data =
    {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
        0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18
    };
    MemoryManager memory;
    ROMPtr rom = std::make_shared<ROM>(BaseROM, SizeROM);
    RAMPtr ram = std::make_shared<RAM>(BaseRAM, SizeRAM);
    memory.AddMemory(rom);
    memory.AddMemory(ram);
    rom->Load(code, OriginROM);
    ram->Load(data, OriginRAM);
    for (size_t index = 0; index < code.size() - 7; index += 8)
    {
        EXPECT_EQ((uint64_t(code[index + 0]) <<  0) | (uint64_t(code[index + 1]) << 8) | 
                  (uint64_t(code[index + 2]) << 16) | (uint64_t(code[index + 3]) << 24) |
                  (uint64_t(code[index + 4]) << 32) | (uint64_t(code[index + 5]) << 40) |
                  (uint64_t(code[index + 6]) << 48) | (uint64_t(code[index + 7]) << 56), memory.Fetch64(OriginROM + index));
    }
    for (size_t index = 0; index < data.size() - 7; index += 8)
    {
        EXPECT_EQ((uint64_t(data[index + 0]) <<  0) | (uint64_t(data[index + 1]) << 8) | 
                  (uint64_t(data[index + 2]) << 16) | (uint64_t(data[index + 3]) << 24) |
                  (uint64_t(data[index + 4]) << 32) | (uint64_t(data[index + 5]) << 40) |
                  (uint64_t(data[index + 6]) << 48) | (uint64_t(data[index + 7]) << 56), memory.Fetch64(OriginRAM + index));
    }
}

TEST_FIXTURE(MemoryManagerTest, FetchInvalidOffset)
{
    MemoryManager memory;
    ROMPtr rom = std::make_shared<ROM>(BaseROM, SizeROM);
    RAMPtr ram = std::make_shared<RAM>(BaseRAM, SizeRAM);
    memory.AddMemory(rom);
    memory.AddMemory(ram);
    EXPECT_THROW(memory.Fetch(BaseROM - 1, 1), std::runtime_error);
    EXPECT_NOTHROW(memory.Fetch(BaseROM + SizeROM - 1, 1));
    EXPECT_NOTHROW(memory.Fetch(BaseROM, SizeROM));
    EXPECT_NOTHROW(memory.Fetch(BaseROM + SizeROM, 1));
    EXPECT_NOTHROW(memory.Fetch(BaseRAM + SizeRAM - 1, 1));
    EXPECT_NOTHROW(memory.Fetch(BaseRAM, SizeRAM));
    EXPECT_THROW(memory.Fetch(BaseRAM, SizeRAM + 1), std::runtime_error);
    EXPECT_THROW(memory.Fetch(BaseRAM + SizeRAM, 1), std::runtime_error);
}

TEST_FIXTURE(MemoryManagerTest, Fetch8InvalidOffset)
{
    MemoryManager memory;
    ROMPtr rom = std::make_shared<ROM>(BaseROM, SizeROM);
    RAMPtr ram = std::make_shared<RAM>(BaseRAM, SizeRAM);
    memory.AddMemory(rom);
    memory.AddMemory(ram);
    EXPECT_THROW(memory.Fetch8(BaseROM - 1), std::runtime_error);
    EXPECT_NOTHROW(memory.Fetch8(BaseROM + SizeROM - 1));
    EXPECT_NOTHROW(memory.Fetch8(BaseROM + SizeROM));
    EXPECT_NOTHROW(memory.Fetch8(BaseRAM + SizeRAM - 1));
    EXPECT_THROW(memory.Fetch8(BaseRAM + SizeRAM), std::runtime_error);
}

TEST_FIXTURE(MemoryManagerTest, Fetch16InvalidOffset)
{
    MemoryManager memory;
    ROMPtr rom = std::make_shared<ROM>(BaseROM, SizeROM);
    RAMPtr ram = std::make_shared<RAM>(BaseRAM, SizeRAM);
    memory.AddMemory(rom);
    memory.AddMemory(ram);
    EXPECT_THROW(memory.Fetch16(BaseROM - 1), std::runtime_error);
    EXPECT_NOTHROW(memory.Fetch16(BaseROM + SizeROM - 1));
    EXPECT_NOTHROW(memory.Fetch16(BaseROM + SizeROM));
    EXPECT_NOTHROW(memory.Fetch16(BaseRAM + SizeRAM - 2));
    EXPECT_THROW(memory.Fetch16(BaseRAM + SizeRAM - 1), std::runtime_error);
}

TEST_FIXTURE(MemoryManagerTest, Fetch32InvalidOffset)
{
    MemoryManager memory;
    ROMPtr rom = std::make_shared<ROM>(BaseROM, SizeROM);
    RAMPtr ram = std::make_shared<RAM>(BaseRAM, SizeRAM);
    memory.AddMemory(rom);
    memory.AddMemory(ram);
    EXPECT_THROW(memory.Fetch32(BaseROM - 1), std::runtime_error);
    EXPECT_NOTHROW(memory.Fetch32(BaseROM + SizeROM - 1));
    EXPECT_NOTHROW(memory.Fetch32(BaseROM + SizeROM));
    EXPECT_NOTHROW(memory.Fetch32(BaseRAM + SizeRAM - 4));
    EXPECT_THROW(memory.Fetch32(BaseRAM + SizeRAM - 3), std::runtime_error);
}

TEST_FIXTURE(MemoryManagerTest, Fetch64InvalidOffset)
{
    MemoryManager memory;
    ROMPtr rom = std::make_shared<ROM>(BaseROM, SizeROM);
    RAMPtr ram = std::make_shared<RAM>(BaseRAM, SizeRAM);
    memory.AddMemory(rom);
    memory.AddMemory(ram);
    EXPECT_THROW(memory.Fetch64(BaseROM - 1), std::runtime_error);
    EXPECT_NOTHROW(memory.Fetch64(BaseROM + SizeROM - 1));
    EXPECT_NOTHROW(memory.Fetch64(BaseROM + SizeROM));
    EXPECT_NOTHROW(memory.Fetch64(BaseRAM + SizeRAM - 8));
    EXPECT_THROW(memory.Fetch64(BaseRAM + SizeRAM - 7), std::runtime_error);
}

TEST_FIXTURE(MemoryManagerTest, Store)
{
    std::vector<uint8_t> code =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    std::vector<uint8_t> data =
    {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
        0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18
    };
    MemoryManager memory;
    ROMPtr rom = std::make_shared<ROM>(BaseROM, SizeROM);
    RAMPtr ram = std::make_shared<RAM>(BaseRAM, SizeRAM);
    memory.AddMemory(rom);
    memory.AddMemory(ram);
    rom->Load(code, OriginROM);
    EXPECT_THROW(memory.Store(BaseROM, data), std::runtime_error);
    EXPECT_NOTHROW(memory.Store(BaseRAM, data));
    EXPECT_EQ(data, memory.Fetch(BaseRAM, data.size()));
}

TEST_FIXTURE(MemoryManagerTest, Store8)
{
    std::vector<uint8_t> code =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    std::vector<uint8_t> data =
    {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
        0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18
    };
    MemoryManager memory;
    ROMPtr rom = std::make_shared<ROM>(BaseROM, SizeROM);
    RAMPtr ram = std::make_shared<RAM>(BaseRAM, SizeRAM);
    memory.AddMemory(rom);
    memory.AddMemory(ram);
    rom->Load(code, OriginROM);
    EXPECT_THROW(memory.Store8(BaseROM, 0x00), std::runtime_error);
    for (size_t index = 0; index < data.size(); ++index)
    {
        memory.Store8(BaseRAM + index, data[index]);
    }
    EXPECT_EQ(data, memory.Fetch(BaseRAM, data.size()));
}

TEST_FIXTURE(MemoryManagerTest, Store16)
{
    std::vector<uint8_t> code =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    std::vector<uint8_t> data =
    {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
        0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18
    };
    MemoryManager memory;
    ROMPtr rom = std::make_shared<ROM>(BaseROM, SizeROM);
    RAMPtr ram = std::make_shared<RAM>(BaseRAM, SizeRAM);
    memory.AddMemory(rom);
    memory.AddMemory(ram);
    rom->Load(code, OriginROM);
    EXPECT_THROW(memory.Store16(BaseROM, 0x0000), std::runtime_error);
    for (size_t index = 0; index < data.size(); index += 2)
    {
        memory.Store16(BaseRAM + index, (data[index] | (data[index + 1] << 8)));
    }
    EXPECT_EQ(data, memory.Fetch(BaseRAM, 2 * (data.size() / 2)));
}

TEST_FIXTURE(MemoryManagerTest, Store32)
{
    std::vector<uint8_t> code =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    std::vector<uint8_t> data =
    {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
        0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18
    };
    MemoryManager memory;
    ROMPtr rom = std::make_shared<ROM>(BaseROM, SizeROM);
    RAMPtr ram = std::make_shared<RAM>(BaseRAM, SizeRAM);
    memory.AddMemory(rom);
    memory.AddMemory(ram);
    rom->Load(code, OriginROM);
    EXPECT_THROW(memory.Store32(BaseROM, 0x00000000), std::runtime_error);
    for (size_t index = 0; index < data.size(); index += 4)
    {
        memory.Store32(BaseRAM + index, (data[index] | (data[index + 1] << 8) | (data[index + 2] << 16) | (data[index + 3] << 24)));
    }
    EXPECT_EQ(data, memory.Fetch(BaseRAM, 4 * (data.size() / 4)));
}

TEST_FIXTURE(MemoryManagerTest, Store64)
{
    std::vector<uint8_t> code =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    std::vector<uint8_t> data =
    {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
        0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
        0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18
    };
    MemoryManager memory;
    ROMPtr rom = std::make_shared<ROM>(BaseROM, SizeROM);
    RAMPtr ram = std::make_shared<RAM>(BaseRAM, SizeRAM);
    memory.AddMemory(rom);
    memory.AddMemory(ram);
    rom->Load(code, OriginROM);
    EXPECT_THROW(memory.Store64(BaseROM, 0x0000000000000000), std::runtime_error);
    for (size_t index = 0; index < data.size(); index += 8)
    {
        memory.Store64(BaseRAM + index, ((data[index + 0] <<  0) | (data[index + 1] << 8) | 
                                         (data[index + 2] << 16) | (data[index + 3] << 24) | 
                                         (uint64_t(data[index + 4]) << 32) | (uint64_t(data[index + 5]) << 40) |
                                         (uint64_t(data[index + 6]) << 48) | (uint64_t(data[index + 7]) << 56)));
    }
    EXPECT_EQ(data, memory.Fetch(BaseRAM, 8 * (data.size() / 8)));
}

TEST_FIXTURE(MemoryManagerTest, StoreInvalidOffset)
{
    std::vector<uint8_t> data =
    {
        0x10, 0x22, 0x58, 0x13, 0x30, 0x19, 0x25, 0x20,
        0x05, 0x30, 0x20, 0x25, 0x19, 0x55, 0x01, 0x25,
        0x20, 0x14, 0x24, 0x00, 0x00
    };
    MemoryManager memory;
    ROMPtr rom = std::make_shared<ROM>(BaseROM, SizeROM);
    RAMPtr ram = std::make_shared<RAM>(BaseRAM, SizeRAM);
    memory.AddMemory(rom);
    memory.AddMemory(ram);
    EXPECT_THROW(memory.Store(BaseROM - 1, data), std::runtime_error);
    EXPECT_THROW(memory.Store(BaseROM + SizeROM - 1, data), std::runtime_error);
    EXPECT_NOTHROW(memory.Store(BaseROM + SizeROM, data));
    EXPECT_NOTHROW(memory.Store(BaseRAM, data));
    EXPECT_NOTHROW(memory.Store(BaseRAM + SizeRAM - data.size(), data));
    EXPECT_THROW(memory.Store(BaseRAM + SizeRAM - (data.size() - 1), data), std::runtime_error);
}

TEST_FIXTURE(MemoryManagerTest, Store8InvalidOffset)
{
    MemoryManager memory;
    ROMPtr rom = std::make_shared<ROM>(BaseROM, SizeROM);
    RAMPtr ram = std::make_shared<RAM>(BaseRAM, SizeRAM);
    memory.AddMemory(rom);
    memory.AddMemory(ram);
    EXPECT_THROW(memory.Store8(BaseROM - 1, 0x00), std::runtime_error);
    EXPECT_THROW(memory.Store8(BaseROM + SizeROM - 1, 0x00), std::runtime_error);
    EXPECT_NOTHROW(memory.Store8(BaseROM + SizeROM, 0x00));
    EXPECT_NOTHROW(memory.Store8(BaseRAM, 0x00));
    EXPECT_NOTHROW(memory.Store8(BaseRAM + SizeRAM - 1, 0x00));
    EXPECT_THROW(memory.Store8(BaseRAM + SizeRAM, 0x00), std::runtime_error);
}

TEST_FIXTURE(MemoryManagerTest, Store16InvalidOffset)
{
    MemoryManager memory;
    ROMPtr rom = std::make_shared<ROM>(BaseROM, SizeROM);
    RAMPtr ram = std::make_shared<RAM>(BaseRAM, SizeRAM);
    memory.AddMemory(rom);
    memory.AddMemory(ram);
    EXPECT_THROW(memory.Store16(BaseROM - 1, 0x0000), std::runtime_error);
    EXPECT_THROW(memory.Store16(BaseROM + SizeROM - 1, 0x0000), std::runtime_error);
    EXPECT_NOTHROW(memory.Store16(BaseROM + SizeROM, 0x0000));
    EXPECT_NOTHROW(memory.Store16(BaseRAM, 0x0000));
    EXPECT_NOTHROW(memory.Store16(BaseRAM + SizeRAM - 2, 0x0000));
    EXPECT_THROW(memory.Store16(BaseRAM + SizeRAM - 1, 0x0000), std::runtime_error);
}

TEST_FIXTURE(MemoryManagerTest, Store32InvalidOffset)
{
    MemoryManager memory;
    ROMPtr rom = std::make_shared<ROM>(BaseROM, SizeROM);
    RAMPtr ram = std::make_shared<RAM>(BaseRAM, SizeRAM);
    memory.AddMemory(rom);
    memory.AddMemory(ram);
    EXPECT_THROW(memory.Store32(BaseROM - 1, 0x00000000), std::runtime_error);
    EXPECT_THROW(memory.Store32(BaseROM + SizeROM - 1, 0x00000000), std::runtime_error);
    EXPECT_NOTHROW(memory.Store32(BaseROM + SizeROM, 0x00000000));
    EXPECT_NOTHROW(memory.Store32(BaseRAM, 0x00000000));
    EXPECT_NOTHROW(memory.Store32(BaseRAM + SizeRAM - 4, 0x00000000));
    EXPECT_THROW(memory.Store32(BaseRAM + SizeRAM - 3, 0x00000000), std::runtime_error);
}

TEST_FIXTURE(MemoryManagerTest, Store64InvalidOffset)
{
    MemoryManager memory;
    ROMPtr rom = std::make_shared<ROM>(BaseROM, SizeROM);
    RAMPtr ram = std::make_shared<RAM>(BaseRAM, SizeRAM);
    memory.AddMemory(rom);
    memory.AddMemory(ram);
    EXPECT_THROW(memory.Store64(BaseROM - 1, 0x0000000000000000), std::runtime_error);
    EXPECT_THROW(memory.Store64(BaseROM + SizeROM - 1, 0x0000000000000000), std::runtime_error);
    EXPECT_NOTHROW(memory.Store64(BaseROM + SizeROM, 0x0000000000000000));
    EXPECT_NOTHROW(memory.Store64(BaseRAM, 0x0000000000000000));
    EXPECT_NOTHROW(memory.Store64(BaseRAM + SizeRAM - 8, 0x0000000000000000));
    EXPECT_THROW(memory.Store64(BaseRAM + SizeRAM - 7, 0x0000000000000000), std::runtime_error);
}

} // namespace Test

} // namespace Emulator
