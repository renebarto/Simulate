#include "unit-test-c++/UnitTestC++.h"

#include "TestData.h"

#include <codecvt>
#include <locale>
#include "Buffer.h"
#include "CharSet.h"
#include "Exceptions.h"

using namespace std;

namespace Assembler
{

namespace Test
{

class BufferTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void BufferTest::SetUp()
{
    //std::ofstream streamWriter;
    //std::wofstream streamWriterW;

    //streamWriter.open(TestData::TestASCIISmall(), std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    //for (int value = 0; value < 256; value++)
    //{
    //    char ch = char(value);
    //    streamWriter.write(&ch, 1);
    //}
    //streamWriter.close();

    //streamWriter.open(TestData::TestUTF8SmallBOM(), std::ios::out | std::ios::binary | std::ios_base::trunc);

    //char smarker[3];
    //smarker[0] = char(0xEF);
    //smarker[1] = char(0xBB);
    //smarker[2] = char(0xBF);

    //streamWriter.write(smarker, 3);
    //streamWriter.close();

    //std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);

    //streamWriterW.open(TestData::TestUTF8SmallBOM(), std::ios_base::out | std::ios::binary | std::ios_base::app);
    //streamWriterW.imbue(utf8_locale); 

    //for (wchar_t ch = 0; ch < CharSet::CharMax; ch++)
    //    streamWriterW.write(&ch, 1);
    //streamWriterW.close();

    //streamWriterW.open(TestData::TestUTF8Small(), std::ios_base::out | std::ios::binary | std::ios_base::trunc);
    //streamWriterW.imbue(utf8_locale); 

    //for (wchar_t ch = 0; ch < CharSet::CharMax; ch++)
    //    streamWriterW.write(&ch, 1);
    //streamWriterW.close();
}

void BufferTest::TearDown()
{
}

static const std::string SmallString = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

TEST_FIXTURE(BufferTest, ConstructDefault)
{
    std::istringstream stream(SmallString, std::ios_base::in | std::ios_base::binary);

    Buffer buffer(&stream, true);
    EXPECT_EQ(size_t{ 0 }, buffer.GetPos());
    EXPECT_EQ(L'A', buffer.Read());
    EXPECT_EQ(L'B', buffer.Peek());
}

TEST_FIXTURE(BufferTest, ConstructNullStream)
{
    EXPECT_THROW(Buffer buffer(nullptr, false), AssemblerException);
}

TEST_FIXTURE(BufferTest, ConstructFileNonExistent)
{
    EXPECT_THROW(Buffer buffer(TestData::TestNonExistent()), std::runtime_error);
}

TEST_FIXTURE(BufferTest, ConstructFileASCII)
{
    Buffer buffer(TestData::TestASCIISmall());
    EXPECT_EQ(size_t{ 0 }, buffer.GetPos());
    EXPECT_EQ(L'\x00', buffer.Read());
    EXPECT_EQ(L'\x01', buffer.Peek());
}

TEST_FIXTURE(BufferTest, ConstructFileUTF8)
{
    Buffer buffer(TestData::TestUTF8Small(), true);
    EXPECT_EQ(size_t{ 0 }, buffer.GetPos());
    EXPECT_EQ(L'\x00', buffer.Read());
    EXPECT_EQ(L'\x01', buffer.Peek());
}

TEST_FIXTURE(BufferTest, ConstructFileUTF8BOM)
{
    Buffer buffer(TestData::TestUTF8SmallBOM(), true);
    EXPECT_EQ(size_t{ 0 }, buffer.GetPos());
    EXPECT_EQ(L'\x00', buffer.Read());
    EXPECT_EQ(L'\x01', buffer.Peek());
}

TEST_FIXTURE(BufferTest, ReadASCII)
{
    Buffer buffer(TestData::TestASCIISmall());
    for (int i = 0; i < 256; i++)
    {
        wchar_t ch = buffer.Read();
        EXPECT_EQ(wchar_t(i), ch);
    }
}

TEST_FIXTURE(BufferTest, ReadUTF8)
{
    Buffer buffer(TestData::TestUTF8Small(), true);
    for (int i = 0; i < CharSet::CharMax; i++)
    {
        wchar_t ch = buffer.Read();
        EXPECT_EQ(wchar_t(i), ch);
    }
}

TEST_FIXTURE(BufferTest, ReadUTF8BOM)
{
    Buffer buffer(TestData::TestUTF8SmallBOM()); // Use auto detection
    for (int i = 0; i < CharSet::CharMax; i++)
    {
        wchar_t ch = buffer.Read();
        EXPECT_EQ(wchar_t(i), ch);
    }
}

TEST_FIXTURE(BufferTest, ReadString)
{
    Buffer buffer(TestData::TestASCIISmall());
    std::wstring expected = L"@ABCD";
    buffer.SetPos(64);
    std::wstring actual = buffer.ReadString(expected.length());
    EXPECT_EQ(expected, actual);

    expected = L"EFGH";
    actual = buffer.ReadString(expected.length());
    EXPECT_EQ(expected, actual);

    expected = L"{|}";
    buffer.SetPos(123);
    actual = buffer.ReadString(expected.length());
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(BufferTest, PeekString)
{
    Buffer buffer(TestData::TestASCIISmall());
    std::wstring expected = L"@ABCD";
    buffer.SetPos(64);
    std::wstring actual = buffer.PeekString(expected.length());
    EXPECT_EQ(expected, actual);

    actual = buffer.ReadString(expected.length());
    EXPECT_EQ(expected, actual);

    expected = L"EFGH";
    actual = buffer.PeekString(expected.length());
    EXPECT_EQ(expected, actual);

    expected = L"{|}";
    buffer.SetPos(123);
    actual = buffer.PeekString(expected.length());
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(BufferTest, PeekStringBeginEnd)
{
    Buffer buffer(TestData::TestASCIISmall());
    std::wstring expected = L"@ABCD";
    std::wstring actual = buffer.PeekString(64, 69);
    EXPECT_EQ(expected, actual);

    expected = L"\x00\x01\x02\x03";
    actual = buffer.ReadString(expected.length());
    EXPECT_EQ(expected, actual);

    expected = L"EFGH";
    actual = buffer.PeekString(69, 73);
    EXPECT_EQ(expected, actual);

    expected = L"{|}";
    actual = buffer.PeekString(123, 126);
    EXPECT_EQ(expected, actual);
}

} // namespace Test

} // namespace Assembler
