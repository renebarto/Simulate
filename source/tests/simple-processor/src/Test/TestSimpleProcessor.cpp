#include "unit-test-c++/UnitTestC++.h"

#include "simple-processor/ram.h"
#include "simple-processor/processor.h"
#include "simple-processor/stringreader.h"
#include "simple-processor/stringwriter.h"
#include "simple-processor/Assertions.h"

using namespace std;

namespace Simulate
{

namespace Test
{

class ProcessorAccessor : public SimpleProcessor
{
public:
    ProcessorAccessor(IMemory<uint8_t> & memory, CharReader & reader, CharWriter & writer)
        : SimpleProcessor(1000, memory, reader, writer)
    {
    }
    SimpleProcessor::Registers & GetRegisters() { return registers; }
};

class SimpleProcessorTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();

    StringReader reader;
    StringWriter writer;
};

void SimpleProcessorTest::SetUp()
{
}

void SimpleProcessorTest::TearDown()
{
}

TEST_FIXTURE(SimpleProcessorTest, Construct)
{
    RAM<uint8_t> memory(0, 256);
    ProcessorAccessor processor(memory, reader, writer);

}

struct TestEntry
{
    SimpleProcessor::Opcode opcode;
    std::string mnemonic;
};

static std::vector<TestEntry> TestEntries =
{
    {SimpleProcessor::Opcode::ACI, "ACI"},
    {SimpleProcessor::Opcode::ACX, "ACX"},
    {SimpleProcessor::Opcode::ADC, "ADC"},
    {SimpleProcessor::Opcode::ADD, "ADD"},
    {SimpleProcessor::Opcode::ADI, "ADI"},
    {SimpleProcessor::Opcode::ADX, "ADX"},
    {SimpleProcessor::Opcode::ANA, "ANA"},
    {SimpleProcessor::Opcode::ANI, "ANI"},
    {SimpleProcessor::Opcode::ANX, "ANX"},
    {SimpleProcessor::Opcode::BCC, "BCC"},
    {SimpleProcessor::Opcode::BCS, "BCS"},
    {SimpleProcessor::Opcode::BNG, "BNG"},
    {SimpleProcessor::Opcode::BNZ, "BNZ"},
    {SimpleProcessor::Opcode::BPZ, "BPZ"},
    {SimpleProcessor::Opcode::BRN, "BRN"},
    {SimpleProcessor::Opcode::BZE, "BZE"},
    {SimpleProcessor::Opcode::CLA, "CLA"},
    {SimpleProcessor::Opcode::CLC, "CLC"},
    {SimpleProcessor::Opcode::CLX, "CLX"},
    {SimpleProcessor::Opcode::CMC, "CMC"},
    {SimpleProcessor::Opcode::CMP, "CMP"},
    {SimpleProcessor::Opcode::CPI, "CPI"},
    {SimpleProcessor::Opcode::CPX, "CPX"},
    {SimpleProcessor::Opcode::DEC, "DEC"},
    {SimpleProcessor::Opcode::DEX, "DEX"},
    {SimpleProcessor::Opcode::HLT, "HLT"},
    {SimpleProcessor::Opcode::INA, "INA"},
    {SimpleProcessor::Opcode::INB, "INB"},
    {SimpleProcessor::Opcode::INC, "INC"},
    {SimpleProcessor::Opcode::INH, "INH"},
    {SimpleProcessor::Opcode::INI, "INI"},
    {SimpleProcessor::Opcode::INX, "INX"},
    {SimpleProcessor::Opcode::JSR, "JSR"},
    {SimpleProcessor::Opcode::LDA, "LDA"},
    {SimpleProcessor::Opcode::LDI, "LDI"},
    {SimpleProcessor::Opcode::LDX, "LDX"},
    {SimpleProcessor::Opcode::LSI, "LSI"},
    {SimpleProcessor::Opcode::LSP, "LSP"},
    {SimpleProcessor::Opcode::NOP, "NOP"},
    {SimpleProcessor::Opcode::ORA, "ORA"},
    {SimpleProcessor::Opcode::ORI, "ORI"},
    {SimpleProcessor::Opcode::ORX, "ORX"},
    {SimpleProcessor::Opcode::OTA, "OTA"},
    {SimpleProcessor::Opcode::OTB, "OTB"},
    {SimpleProcessor::Opcode::OTC, "OTC"},
    {SimpleProcessor::Opcode::OTH, "OTH"},
    {SimpleProcessor::Opcode::OTI, "OTI"},
    {SimpleProcessor::Opcode::POP, "POP"},
    {SimpleProcessor::Opcode::PSH, "PSH"},
    {SimpleProcessor::Opcode::RET, "RET"},
    {SimpleProcessor::Opcode::SBC, "SBC"},
    {SimpleProcessor::Opcode::SBI, "SBI"},
    {SimpleProcessor::Opcode::SBX, "SBX"},
    {SimpleProcessor::Opcode::SCI, "SCI"},
    {SimpleProcessor::Opcode::SCX, "SCX"},
    {SimpleProcessor::Opcode::SHL, "SHL"},
    {SimpleProcessor::Opcode::SHR, "SHR"},
    {SimpleProcessor::Opcode::STA, "STA"},
    {SimpleProcessor::Opcode::STX, "STX"},
    {SimpleProcessor::Opcode::SUB, "SUB"},
    {SimpleProcessor::Opcode::TAX, "TAX"},
    {SimpleProcessor::Opcode::BAD_OPCODE, "ABC"},
};

static const std::vector<uint8_t> MachineCode =
{
    0x0A, 0x16, 0x3A, 0x0D, 0x1E, 0x13, 0x19, 0x14,
    0x05, 0x1E, 0x14, 0x19, 0x13, 0x37, 0x01, 0x19,
    0x14, 0x0E, 0x18, 0x00, 0x00
};

TEST_FIXTURE(SimpleProcessorTest, LookupOpcodeByString)
{
    RAM<uint8_t> memory(0, 256);
    ProcessorAccessor processor(memory, reader, writer);

    for (auto entry : TestEntries)
    {
        ASSERT_EQ(entry.opcode, processor.LookupOpcode(entry.mnemonic));
    }
}

TEST_FIXTURE(SimpleProcessorTest, LookupOpcodeByOpcodeByte)
{
    RAM<uint8_t> memory(0, 256);
    ProcessorAccessor processor(memory, reader, writer);

    for (auto entry : TestEntries)
    {
        if (entry.opcode != SimpleProcessor::Opcode::BAD_OPCODE)
        {
            SimpleProcessor::InstructionInfo info = processor.LookupOpcode(uint8_t(entry.opcode));
            size_t expectedSize = entry.opcode >= SimpleProcessor::Opcode::LDA ? size_t{ 2 } : size_t{ 1 };
            EXPECT_EQ(expectedSize, info.instructionSize);
            EXPECT_EQ(entry.mnemonic, info.instructionMnemonic);
        }
        else
        {
            EXPECT_THROW(processor.LookupOpcode(uint8_t(entry.opcode)), IllegalInstructionException);
        }

    }
}

TEST_FIXTURE(SimpleProcessorTest, FetchAndExecute)
{
}

TEST_FIXTURE(SimpleProcessorTest, FetchInstruction)
{
    RAM<uint8_t> memory(0, 256, MachineCode);
    ProcessorAccessor processor(memory, reader, writer);

    processor.Reset();
    processor.FetchInstruction();
    EXPECT_EQ(MachineCode[0], processor.GetRegisters().ir);
}

TEST_FIXTURE(SimpleProcessorTest, Execute)
{
    RAM<uint8_t> memory(0, 256);
    ProcessorAccessor processor(memory, reader, writer);

    processor.Reset();
    processor.ClearMemory();
    processor.Execute(uint8_t(SimpleProcessor::Opcode::NOP));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 1);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 0xFF;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::CLA));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 1);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().flags = Flags::C;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::C, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::CLC));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 1);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().x = 0xFF;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0xFF, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::CLX));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 1);

    processor.Reset();
    processor.ClearMemory();
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::CMC));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::C, State::Uninitialized, 1);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 0xFF;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::INC));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::Z, State::Uninitialized, 1);
    processor.Reset();
    processor.GetRegisters().a = 0;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::INC));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 1, 0, 0, 0, 0, Flags::P, State::Uninitialized, 1);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 2;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 2, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::DEC));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 1, 0, 0, 0, 0, Flags::P, State::Uninitialized, 1);
    processor.Reset();
    processor.GetRegisters().a = 1;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 1, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::DEC));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::Z, State::Uninitialized, 1);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().x = 0xFF;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0xFF, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::INX));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::Z, State::Uninitialized, 1);
    processor.Reset();
    processor.GetRegisters().x = 0;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::INX));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 1, 0, 0, 0, Flags::P, State::Uninitialized, 1);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().x = 2;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 2, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::DEX));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 1, 0, 0, 0, Flags::P, State::Uninitialized, 1);
    processor.Reset();
    processor.GetRegisters().x = 1;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 1, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::DEX));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::Z, State::Uninitialized, 1);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 1;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 1, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::TAX));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 1, 1, 0, 0, 0, Flags::None, State::Uninitialized, 1);

    processor.Reset();
    processor.ClearMemory();
    reader.SetContents("65");
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::INI));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 'A', 0, 0, 0, 0, Flags::P, State::Uninitialized, 1);
    processor.Reset();
    reader.SetContents("255");
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::INI));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0, 0, 0, 0, Flags::None, State::Uninitialized, 1);
    processor.Reset();
    reader.SetContents("");
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::INI));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::Z, State::NoData, 1);
    processor.Reset();
    reader.SetContents("xxx");
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::INI));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::Z, State::BadData, 1);

    processor.Reset();
    processor.ClearMemory();
    reader.SetContents("41");
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::INH));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 'A', 0, 0, 0, 0, Flags::P, State::Uninitialized, 1);
    processor.Reset();
    reader.SetContents("FF");
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::INH));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0, 0, 0, 0, Flags::None, State::Uninitialized, 1);
    processor.Reset();
    reader.SetContents("");
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::INH));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::Z, State::NoData, 1);
    processor.Reset();
    reader.SetContents("xxx");
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::INH));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::Z, State::BadData, 1);

    processor.Reset();
    processor.ClearMemory();
    reader.SetContents("01000001");
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::INB));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 'A', 0, 0, 0, 0, Flags::P, State::Uninitialized, 1);
    processor.Reset();
    reader.SetContents("11111111");
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::INB));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0, 0, 0, 0, Flags::None, State::Uninitialized, 1);
    processor.Reset();
    reader.SetContents("");
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::INB));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::Z, State::NoData, 1);
    processor.Reset();
    reader.SetContents("xxx");
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::INB));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::Z, State::BadData, 1);

    processor.Reset();
    processor.ClearMemory();
    reader.SetContents("A");
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::INA));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 'A', 0, 0, 0, 0, Flags::P, State::Uninitialized, 1);
    processor.Reset();
    reader.SetContents("");
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::INA));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::Z, State::NoData, 1);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 'A';
    writer.ClearContents();
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 'A', 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::OTI));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 'A', 0, 0, 0, 0, Flags::None, State::Uninitialized, 1);
    EXPECT_EQ("65", writer.GetContents());
    processor.Reset();
    processor.GetRegisters().a = 0xFF;
    writer.ClearContents();
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::OTI));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0, 0, 0, 0, Flags::None, State::Uninitialized, 1);
    EXPECT_EQ("-1", writer.GetContents());

    writer.ClearContents();
    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 'A';
    writer.ClearContents();
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 'A', 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::OTC));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 'A', 0, 0, 0, 0, Flags::None, State::Uninitialized, 1);
    EXPECT_EQ("65", writer.GetContents());
    processor.Reset();
    processor.GetRegisters().a = 0xFF;
    writer.ClearContents();
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::OTC));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0, 0, 0, 0, Flags::None, State::Uninitialized, 1);
    EXPECT_EQ("255", writer.GetContents());

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 'A';
    writer.ClearContents();
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 'A', 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::OTH));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 'A', 0, 0, 0, 0, Flags::None, State::Uninitialized, 1);
    EXPECT_EQ("41", writer.GetContents());

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 'A';
    writer.ClearContents();
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 'A', 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::OTB));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 'A', 0, 0, 0, 0, Flags::None, State::Uninitialized, 1);
    EXPECT_EQ("01000001", writer.GetContents());

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 'A';
    writer.ClearContents();
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 'A', 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::OTA));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 'A', 0, 0, 0, 0, Flags::None, State::Uninitialized, 1);
    EXPECT_EQ("A", writer.GetContents());

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().sp = 0x00;
    processor.GetRegisters().a = 0x01;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 1, 0, 0x00, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::PSH));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 1, 0, 0xFF, 0, 0, Flags::None, State::Uninitialized, 1);
    AssertMemory(__FILE__, __LINE__, processor.GetMemory(), 0xFF, 1);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().sp = 0xFF;
    processor.Store(0xFF, 1);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0xFF, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::POP));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 1, 0, 0x00, 0, 0, Flags::P, State::Uninitialized, 1);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 0x30;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x30, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::SHL));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x60, 0, 0, 0, 0, Flags::P, State::Uninitialized, 1);
    processor.Reset();
    processor.GetRegisters().a = 0x60;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x60, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::SHL));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xC0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 1);
    processor.Reset();
    processor.GetRegisters().a = 0xC0;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xC0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::SHL));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x80, 0, 0, 0, 0, Flags::C, State::Uninitialized, 1);
    processor.Reset();
    processor.GetRegisters().a = 0x80;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x80, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::SHL));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x00, 0, 0, 0, 0, Flags::C | Flags::Z, State::Uninitialized, 1);
    processor.Reset();
    processor.GetRegisters().a = 0x00;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x00, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::SHL));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x00, 0, 0, 0, 0, Flags::Z, State::Uninitialized, 1);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 0x06;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x06, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::SHR));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x03, 0, 0, 0, 0, Flags::P, State::Uninitialized, 1);
    processor.Reset();
    processor.GetRegisters().a = 0x03;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x03, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::SHR));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x01, 0, 0, 0, 0, Flags::P | Flags::C, State::Uninitialized, 1);
    processor.Reset();
    processor.GetRegisters().a = 0x01;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x01, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::SHR));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x00, 0, 0, 0, 0, Flags::C | Flags::Z, State::Uninitialized, 1);
    processor.Reset();
    processor.GetRegisters().a = 0x00;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x00, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::SHR));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x00, 0, 0, 0, 0, Flags::Z, State::Uninitialized, 1);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().sp = 0xFF;
    processor.Store(0xFF, 0x80);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0xFF, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::RET));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0x00, 0x80, 0, Flags::None, State::Uninitialized, 1);

    processor.Reset();
    processor.ClearMemory();
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::HLT));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Halted, 1);

    processor.Reset();
    processor.ClearMemory();
    processor.Store(0x00, 0x7F);
    processor.Store(0x7F, 0x12);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::LDA));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x12, 0, 0, 0x01, 0, Flags::P, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().x = 0x80;
    processor.Store(0x00, 0x7F);
    processor.Store(0xFF, 0x12);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0x80, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::LDX));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x12, 0x80, 0, 0x01, 0, Flags::P, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.Store(0x00, 0x7F);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::LDI));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x7F, 0, 0, 0x01, 0, Flags::P, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.Store(0x00, 0x7F);
    processor.Store(0x7F, 0x12);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::LSP));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0x12, 0x01, 0, Flags::None, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.Store(0x00, 0x7F);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::LSI));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0x7F, 0x01, 0, Flags::None, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 0x12;
    processor.Store(0x00, 0x7F);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x12, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::STA));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x12, 0, 0, 0x01, 0, Flags::None, State::Uninitialized, 2);
    AssertMemory(__FILE__, __LINE__, processor.GetMemory(), 0x7F, 0x12);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 0x12;
    processor.GetRegisters().x = 0x80;
    processor.Store(0x00, 0x7F);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x12, 0x80, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::STX));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x12, 0x80, 0, 0x01, 0, Flags::None, State::Uninitialized, 2);
    AssertMemory(__FILE__, __LINE__, processor.GetMemory(), 0xFF, 0x12);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 0x01;
    processor.Store(0x00, 0x7F);
    processor.Store(0x7F, 0xFE);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x01, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ADD));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0, 0, 0x01, 0, Flags::None, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0x02;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x02, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ADD));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x00, 0, 0, 0x01, 0, Flags::C | Flags::Z, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0x03;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x03, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ADD));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x01, 0, 0, 0x01, 0, Flags::C | Flags::P, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 0x01;
    processor.GetRegisters().x = 0x80;
    processor.Store(0x00, 0x7F);
    processor.Store(0xFF, 0xFE);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x01, 0x80, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ADX));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0x80, 0, 0x01, 0, Flags::None, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0x02;
    processor.GetRegisters().x = 0x80;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x02, 0x80, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ADX));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x00, 0x80, 0, 0x01, 0, Flags::C | Flags::Z, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0x03;
    processor.GetRegisters().x = 0x80;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x03, 0x80, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ADX));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x01, 0x80, 0, 0x01, 0, Flags::C | Flags::P, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 0x01;
    processor.Store(0x00, 0xFE);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x01, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ADI));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0, 0, 0x01, 0, Flags::None, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0x02;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x02, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ADI));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x00, 0, 0, 0x01, 0, Flags::C | Flags::Z, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0x03;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x03, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ADI));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x01, 0, 0, 0x01, 0, Flags::C | Flags::P, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 0x01;
    processor.Store(0x00, 0x7F);
    processor.Store(0x7F, 0xFE);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x01, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ADC));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0, 0, 0x01, 0, Flags::None, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0x02;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x02, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ADC));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x00, 0, 0, 0x01, 0, Flags::C | Flags::Z, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0x03;
    processor.GetRegisters().flags = Flags::C;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x03, 0, 0, 0, 0, Flags::C, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ADC));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x02, 0, 0, 0x01, 0, Flags::C | Flags::P, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 0x01;
    processor.GetRegisters().x = 0x80;
    processor.Store(0x00, 0x7F);
    processor.Store(0xFF, 0xFE);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x01, 0x80, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ACX));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0x80, 0, 0x01, 0, Flags::None, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0x02;
    processor.GetRegisters().x = 0x80;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x02, 0x80, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ACX));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x00, 0x80, 0, 0x01, 0, Flags::C | Flags::Z, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0x03;
    processor.GetRegisters().x = 0x80;
    processor.GetRegisters().flags = Flags::C;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x03, 0x80, 0, 0, 0, Flags::C, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ACX));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x02, 0x80, 0, 0x01, 0, Flags::C | Flags::P, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 0x01;
    processor.Store(0x00, 0x7E);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x01, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ACI));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x7F, 0, 0, 0x01, 0, Flags::P, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0x82;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x82, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ACI));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x00, 0, 0, 0x01, 0,Flags::C | Flags::Z, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0x83;
    processor.GetRegisters().flags = Flags::C;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x83, 0, 0, 0, 0, Flags::C, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ACI));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x02, 0, 0, 0x01, 0, Flags::C | Flags::P, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 0xFF;
    processor.Store(0x00, 0x7F);
    processor.Store(0x7F, 0xFE);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::SUB));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x01, 0, 0, 0x01, 0, Flags::P, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0xFE;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFE, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::SUB));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x00, 0, 0, 0x01, 0, Flags::Z, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0xFD;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFD, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::SUB));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0, 0, 0x01, 0, Flags::C, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 0xFF;
    processor.GetRegisters().x = 0x80;
    processor.Store(0x00, 0x7F);
    processor.Store(0xFF, 0xFE);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0x80, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::SBX));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x01, 0x80, 0, 0x01, 0, Flags::P, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0xFE;
    processor.GetRegisters().x = 0x80;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFE, 0x80, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::SBX));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x00, 0x80, 0, 0x01, 0, Flags::Z, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0xFD;
    processor.GetRegisters().x = 0x80;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFD, 0x80, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::SBX));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0x80, 0, 0x01, 0, Flags::C, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 0xFF;
    processor.Store(0x00, 0xFE);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::SBI));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x01, 0, 0, 0x01, 0, Flags::P, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0xFE;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFE, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::SBI));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x00, 0, 0, 0x01, 0, Flags::Z, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0xFD;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFD, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::SBI));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0, 0, 0x01, 0, Flags::C, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 0xFF;
    processor.Store(0x00, 0x7F);
    processor.Store(0x7F, 0xFE);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::SBC));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x01, 0, 0, 0x01, 0, Flags::P, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0xFE;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFE, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::SBC));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x00, 0, 0, 0x01, 0, Flags::Z, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0xFD;
    processor.GetRegisters().flags = Flags::C;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFD, 0, 0, 0, 0, Flags::C, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::SBC));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFE, 0, 0, 0x01, 0, Flags::C, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 0xFF;
    processor.GetRegisters().x = 0x80;
    processor.Store(0x00, 0x7F);
    processor.Store(0xFF, 0xFE);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0x80, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::SCX));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x01, 0x80, 0, 0x01, 0, Flags::P, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0xFE;
    processor.GetRegisters().x = 0x80;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFE, 0x80, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::SCX));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x00, 0x80, 0, 0x01, 0, Flags::Z, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0xFD;
    processor.GetRegisters().x = 0x80;
    processor.GetRegisters().flags = Flags::C;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFD, 0x80, 0, 0, 0, Flags::C, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::SCX));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFE, 0x80, 0, 0x01, 0, Flags::C, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 0xFF;
    processor.Store(0x00, 0xFE);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::SCI));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x01, 0, 0, 0x01, 0, Flags::P, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0xFE;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFE, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::SCI));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x00, 0, 0, 0x01, 0, Flags::Z, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0xFD;
    processor.GetRegisters().flags = Flags::C;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFD, 0, 0, 0, 0, Flags::C, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::SCI));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFE, 0, 0, 0x01, 0, Flags::C, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 0xFF;
    processor.Store(0x00, 0x7F);
    processor.Store(0x7F, 0xFE);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::CMP));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0, 0, 0x01, 0, Flags::P, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0xFE;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFE, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::CMP));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFE, 0, 0, 0x01, 0, Flags::Z, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0xFD;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFD, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::CMP));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFD, 0, 0, 0x01, 0, Flags::C, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 0xFF;
    processor.GetRegisters().x = 0x80;
    processor.Store(0x00, 0x7F);
    processor.Store(0xFF, 0xFE);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0x80, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::CPX));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0x80, 0, 0x01, 0, Flags::P, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0xFE;
    processor.GetRegisters().x = 0x80;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFE, 0x80, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::CPX));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFE, 0x80, 0, 0x01, 0, Flags::Z, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0xFD;
    processor.GetRegisters().x = 0x80;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFD, 0x80, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::CPX));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFD, 0x80, 0, 0x01, 0, Flags::C, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().a = 0xFF;
    processor.Store(0x00, 0xFE);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::CPI));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0, 0, 0x01, 0, Flags::P, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0xFE;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFE, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::CPI));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFE, 0, 0, 0x01, 0, Flags::Z, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().a = 0xFD;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFD, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::CPI));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFD, 0, 0, 0x01, 0, Flags::C, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().flags = Flags::C;
    processor.GetRegisters().a = 0xFF;
    processor.Store(0x00, 0x7F);
    processor.Store(0x7F, 0x01);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0, 0, 0, 0, Flags::C, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ANA));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x01, 0, 0, 0x01, 0, Flags::P, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().flags = Flags::C;
    processor.GetRegisters().a = 0xFE;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFE, 0, 0, 0, 0, Flags::C, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ANA));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x00, 0, 0, 0x01, 0, Flags::Z, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().flags = Flags::C;
    processor.GetRegisters().a = 0xFF;
    processor.GetRegisters().x = 0x80;
    processor.Store(0x00, 0x7F);
    processor.Store(0xFF, 0x01);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0x80, 0, 0, 0, Flags::C, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ANX));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x01, 0x80, 0, 0x01, 0, Flags::P, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().flags = Flags::C;
    processor.GetRegisters().a = 0xFE;
    processor.GetRegisters().x = 0x80;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFE, 0x80, 0, 0, 0, Flags::C, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ANX));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x00, 0x80, 0, 0x01, 0, Flags::Z, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().flags = Flags::C;
    processor.GetRegisters().a = 0xFF;
    processor.Store(0x00, 0x01);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0, 0, 0, 0, Flags::C, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ANI));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x01, 0, 0, 0x01, 0, Flags::P, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().flags = Flags::C;
    processor.GetRegisters().a = 0xFE;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFE, 0, 0, 0, 0, Flags::C, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ANI));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x00, 0, 0, 0x01, 0, Flags::Z, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().flags = Flags::C;
    processor.GetRegisters().a = 0x02;
    processor.Store(0x00, 0x7F);
    processor.Store(0x7F, 0x01);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x02, 0, 0, 0, 0, Flags::C, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ORA));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x03, 0, 0, 0x01, 0, Flags::P, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().flags = Flags::C;
    processor.GetRegisters().a = 0xFE;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFE, 0, 0, 0, 0, Flags::C, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ORA));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0, 0, 0x01, 0, Flags::None, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().flags = Flags::C;
    processor.GetRegisters().a = 0x02;
    processor.GetRegisters().x = 0x80;
    processor.Store(0x00, 0x7F);
    processor.Store(0xFF, 0x01);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x02, 0x80, 0, 0, 0, Flags::C, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ORX));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x03, 0x80, 0, 0x01, 0, Flags::P, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().flags = Flags::C;
    processor.GetRegisters().a = 0xFE;
    processor.GetRegisters().x = 0x80;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFE, 0x80, 0, 0, 0, Flags::C, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ORX));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0x80, 0, 0x01, 0, Flags::None, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.GetRegisters().flags = Flags::C;
    processor.GetRegisters().a = 0x02;
    processor.Store(0x00, 0x01);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x02, 0, 0, 0, 0, Flags::C, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ORI));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0x03, 0, 0, 0x01, 0, Flags::P, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().flags = Flags::C;
    processor.GetRegisters().a = 0xFE;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFE, 0, 0, 0, 0, Flags::C, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::ORI));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0xFF, 0, 0, 0x01, 0, Flags::None, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.Store(0x00, 0xFE);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::BRN));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0xFE, 0, Flags::None, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.Store(0x00, 0xFE);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::BZE));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0x01, 0, Flags::None, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().flags = Flags::Z;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::Z, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::BZE));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0xFE, 0, Flags::Z, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.Store(0x00, 0xFE);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::BNZ));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0xFE, 0, Flags::None, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().flags = Flags::Z;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::Z, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::BNZ));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0x01, 0, Flags::Z, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.Store(0x00, 0xFE);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::BPZ));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0x01, 0, Flags::None, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().flags = Flags::P;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::P, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::BPZ));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0xFE, 0, Flags::P, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.Store(0x00, 0xFE);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::BNG));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0xFE, 0, Flags::None, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().flags = Flags::P;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::P, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::BNG));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0x01, 0, Flags::P, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.Store(0x00, 0xFE);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::BCC));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0xFE, 0, Flags::None, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().flags = Flags::C;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::C, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::BCC));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0x01, 0, Flags::C, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.Store(0x00, 0xFE);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::BCS));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0x01, 0, Flags::None, State::Uninitialized, 2);
    processor.Reset();
    processor.GetRegisters().flags = Flags::C;
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::C, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::BCS));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0xFE, 0, Flags::C, State::Uninitialized, 2);

    processor.Reset();
    processor.ClearMemory();
    processor.Store(0x00, 0xFE);
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0, 0, 0, Flags::None, State::Uninitialized, 0);
    processor.Execute(uint8_t(SimpleProcessor::Opcode::JSR));
    AssertRegisters(__FILE__, __LINE__, processor.GetRegisters(), 0, 0, 0xFF, 0xFE, 0, Flags::None, State::Uninitialized, 2);
    AssertMemory(__FILE__, __LINE__, processor.GetMemory(), 0xFF, 0x01);
}

} // namespace Test

} // namespace Simulate
