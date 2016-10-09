#include "unit-test-c++/UnitTestC++.h"

#include "object-file/ObjectCode.h"
#include "core/Util.h"

using namespace std;

namespace ObjectFile
{

namespace Test
{

class CodeSegmentTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void CodeSegmentTest::SetUp()
{
}

void CodeSegmentTest::TearDown()
{
}

static const SegmentID ID = SegmentID::CSEG;
static const std::string Name = "Code";
static const SegmentID ID2 = SegmentID::DSEG;
static const std::string Name2 = "Data";
static const size_t Offset = 0x8000;
static const std::vector<uint8_t> Data = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };
static const size_t Size = Data.size();

TEST_FIXTURE(CodeSegmentTest, Construct)
{
    CodeSegment<size_t> segment(ID, Name);
    EXPECT_EQ(ID, segment.ID());
    EXPECT_EQ(Name, segment.Name());
    EXPECT_EQ(size_t{ 0 }, segment.Offset());
    EXPECT_EQ(size_t{ 0 }, segment.Size());
    EXPECT_EQ(size_t{ 0 }, segment.Data().size());
}

TEST_FIXTURE(CodeSegmentTest, ConstructCopy)
{
    CodeSegment<size_t> segment2(ID2, Name2);
    segment2.SetOffset(Offset);
    segment2.SetData(Data);
    CodeSegment<size_t> segment(segment2);
    EXPECT_EQ(ID2, segment.ID());
    EXPECT_EQ(Name2, segment.Name());
    EXPECT_EQ(Offset, segment.Offset());
    EXPECT_EQ(Size, segment.Size());
    EXPECT_TRUE(Core::Util::Compare(Data, segment.Data()));

    EXPECT_EQ(ID2, segment2.ID());
    EXPECT_EQ(Name2, segment2.Name());
    EXPECT_EQ(Offset, segment2.Offset());
    EXPECT_EQ(Size, segment2.Size());
    EXPECT_TRUE(Core::Util::Compare(Data, segment2.Data()));
}

TEST_FIXTURE(CodeSegmentTest, ConstructMove)
{
    CodeSegment<size_t> segment2(ID2, Name2);
    segment2.SetOffset(Offset);
    segment2.SetData(Data);
    CodeSegment<size_t> segment(std::move(segment2));
    EXPECT_EQ(ID2, segment.ID());
    EXPECT_EQ(Name2, segment.Name());
    EXPECT_EQ(Offset, segment.Offset());
    EXPECT_EQ(Size, segment.Size());
    EXPECT_TRUE(Core::Util::Compare(Data, segment.Data()));

    EXPECT_EQ(SegmentID::Invalid, segment2.ID());
    EXPECT_EQ("", segment2.Name());
    EXPECT_EQ(size_t{ 0 }, segment2.Offset());
    EXPECT_EQ(size_t{ 0 }, segment2.Size());
    EXPECT_EQ(size_t{ 0 }, segment2.Data().size());
}

TEST_FIXTURE(CodeSegmentTest, Assign)
{
    CodeSegment<size_t> segment2(ID2, Name2);
    segment2.SetOffset(Offset);
    segment2.SetData(Data);
    CodeSegment<size_t> segment(ID, Name);
    segment = segment2;
    EXPECT_EQ(ID2, segment.ID());
    EXPECT_EQ(Name2, segment.Name());
    EXPECT_EQ(Offset, segment.Offset());
    EXPECT_EQ(Size, segment.Size());
    EXPECT_TRUE(Core::Util::Compare(Data, segment.Data()));

    EXPECT_EQ(ID2, segment2.ID());
    EXPECT_EQ(Name2, segment2.Name());
    EXPECT_EQ(Offset, segment2.Offset());
    EXPECT_EQ(Size, segment2.Size());
    EXPECT_TRUE(Core::Util::Compare(Data, segment2.Data()));
}

TEST_FIXTURE(CodeSegmentTest, AssignMove)
{
    CodeSegment<size_t> segment2(ID2, Name2);
    segment2.SetOffset(Offset);
    segment2.SetData(Data);
    CodeSegment<size_t> segment(ID, Name);
    segment = std::move(segment2);
    EXPECT_EQ(ID2, segment.ID());
    EXPECT_EQ(Name2, segment.Name());
    EXPECT_EQ(Offset, segment.Offset());
    EXPECT_EQ(Size, segment.Size());
    EXPECT_TRUE(Core::Util::Compare(Data, segment.Data()));

    EXPECT_EQ(SegmentID::Invalid, segment2.ID());
    EXPECT_EQ("", segment2.Name());
    EXPECT_EQ(size_t{ 0 }, segment2.Offset());
    EXPECT_EQ(size_t{ 0 }, segment2.Size());
    EXPECT_EQ(size_t{ 0 }, segment2.Data().size());
}

TEST_FIXTURE(CodeSegmentTest, SetOffset)
{
    CodeSegment<size_t> segment(ID, Name);
    EXPECT_EQ(ID, segment.ID());
    EXPECT_EQ(Name, segment.Name());
    EXPECT_EQ(size_t{ 0 }, segment.Offset());
    EXPECT_EQ(size_t{ 0 }, segment.Size());
    EXPECT_EQ(size_t{ 0 }, segment.Data().size());

    segment.SetOffset(Offset);
    EXPECT_EQ(ID, segment.ID());
    EXPECT_EQ(Name, segment.Name());
    EXPECT_EQ(Offset, segment.Offset());
    EXPECT_EQ(uint16_t{ 0 }, segment.Size());
    EXPECT_EQ(size_t{ 0 }, segment.Data().size());
}

TEST_FIXTURE(CodeSegmentTest, SetData)
{
    CodeSegment<size_t> segment(ID, Name);
    EXPECT_EQ(ID, segment.ID());
    EXPECT_EQ(Name, segment.Name());
    EXPECT_EQ(size_t{ 0 }, segment.Offset());
    EXPECT_EQ(size_t{ 0 }, segment.Size());

    segment.SetData(Data);
    EXPECT_EQ(ID, segment.ID());
    EXPECT_EQ(Name, segment.Name());
    EXPECT_EQ(size_t{ 0 }, segment.Offset());
    EXPECT_EQ(Size, segment.Size());
    EXPECT_EQ(Size, segment.Data().size());
    EXPECT_TRUE(Core::Util::Compare(Data, segment.Data()));
}

} // namespace Test

} // namespace ObjectFile
