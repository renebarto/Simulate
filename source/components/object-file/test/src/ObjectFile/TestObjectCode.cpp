#include "unit-test-c++/UnitTestC++.h"

#include "object-file/ObjectCode.h"
#include "core/Util.h"

using namespace std;

namespace ObjectFile
{

namespace Test
{

class ObjectCodeTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void ObjectCodeTest::SetUp()
{
}

void ObjectCodeTest::TearDown()
{
}

static const std::string ModuleName = "Module";
static const SegmentID DefaultSegID = SegmentID::ASEG;
static const std::string DefaultSegName = "ASEG";
static const SegmentID SegID = SegmentID::CSEG;
static const std::string SegName = "Code";
static const uint16_t Offset = 0x4000;
static const std::vector<uint8_t> Data = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };

TEST_FIXTURE(ObjectCodeTest, Construct)
{
    ObjectCode code(ModuleName);
    EXPECT_EQ(ModuleName, code.ModuleName());
    EXPECT_TRUE(code.HaveSegment(DefaultSegID));
    EXPECT_TRUE(code.HaveSegment(DefaultSegName));
    EXPECT_FALSE(code.HaveSegment(SegID));
    EXPECT_FALSE(code.HaveSegment(SegName));
    EXPECT_EQ(size_t{ 1 }, code.GetSegments().size());
    EXPECT_NOTHROW(code.GetSegment(DefaultSegID));
    EXPECT_NOTHROW(code.GetSegment(DefaultSegName));
    EXPECT_THROW(code.GetSegment(SegID), std::invalid_argument);
    EXPECT_THROW(code.GetSegment(SegName), std::invalid_argument);
    EXPECT_EQ(DefaultSegID, code.GetSegment(DefaultSegID).ID());
    EXPECT_EQ(DefaultSegName, code.GetSegment(DefaultSegID).Name());
    EXPECT_EQ(0, code.GetSegment(DefaultSegID).Offset());
    EXPECT_EQ(0, code.GetSegment(DefaultSegID).Size());
    EXPECT_EQ(0, code.GetSegment(DefaultSegID).Data().size());
}

TEST_FIXTURE(ObjectCodeTest, AddSegmentIDName)
{
    ObjectCode code(ModuleName);
    EXPECT_EQ(ModuleName, code.ModuleName());
    EXPECT_TRUE(code.HaveSegment(DefaultSegID));
    EXPECT_TRUE(code.HaveSegment(DefaultSegName));
    EXPECT_FALSE(code.HaveSegment(SegID));
    EXPECT_FALSE(code.HaveSegment(SegName));
    EXPECT_EQ(size_t{ 1 }, code.GetSegments().size());
    EXPECT_NOTHROW(code.GetSegment(DefaultSegID));
    EXPECT_NOTHROW(code.GetSegment(DefaultSegName));
    EXPECT_THROW(code.GetSegment(SegID), std::invalid_argument);
    EXPECT_THROW(code.GetSegment(SegName), std::invalid_argument);

    code.AddSegment(SegID, SegName);
    EXPECT_EQ(ModuleName, code.ModuleName());
    EXPECT_TRUE(code.HaveSegment(DefaultSegID));
    EXPECT_TRUE(code.HaveSegment(DefaultSegName));
    EXPECT_TRUE(code.HaveSegment(SegID));
    EXPECT_TRUE(code.HaveSegment(SegName));
    EXPECT_EQ(size_t{ 2 }, code.GetSegments().size());
    EXPECT_NOTHROW(code.GetSegment(DefaultSegID));
    EXPECT_NOTHROW(code.GetSegment(DefaultSegName));
    EXPECT_NOTHROW(code.GetSegment(SegID));
    EXPECT_NOTHROW(code.GetSegment(SegName));
}

TEST_FIXTURE(ObjectCodeTest, AddSegment)
{
    ObjectCode code(ModuleName);
    EXPECT_EQ(ModuleName, code.ModuleName());
    EXPECT_TRUE(code.HaveSegment(DefaultSegID));
    EXPECT_TRUE(code.HaveSegment(DefaultSegName));
    EXPECT_FALSE(code.HaveSegment(SegID));
    EXPECT_FALSE(code.HaveSegment(SegName));
    EXPECT_EQ(size_t{ 1 }, code.GetSegments().size());
    EXPECT_NOTHROW(code.GetSegment(DefaultSegID));
    EXPECT_NOTHROW(code.GetSegment(DefaultSegName));
    EXPECT_THROW(code.GetSegment(SegID), std::invalid_argument);
    EXPECT_THROW(code.GetSegment(SegName), std::invalid_argument);

    CodeSegment<uint16_t> segment(SegID, SegName);
    segment.SetOffset(Offset);
    segment.SetData(Data);
    code.AddSegment(segment);
    EXPECT_EQ(SegID, segment.ID());
    EXPECT_EQ(SegName, segment.Name());
    EXPECT_EQ(Offset, segment.Offset());
    EXPECT_EQ(Data.size(), segment.Size());
    EXPECT_TRUE(Core::Util::Compare(Data, segment.Data()));

    EXPECT_EQ(ModuleName, code.ModuleName());
    EXPECT_TRUE(code.HaveSegment(DefaultSegID));
    EXPECT_TRUE(code.HaveSegment(DefaultSegName));
    EXPECT_TRUE(code.HaveSegment(SegID));
    EXPECT_TRUE(code.HaveSegment(SegName));
    EXPECT_EQ(size_t{ 2 }, code.GetSegments().size());
    EXPECT_NOTHROW(code.GetSegment(DefaultSegID));
    EXPECT_NOTHROW(code.GetSegment(DefaultSegName));
    EXPECT_NOTHROW(code.GetSegment(SegID));
    EXPECT_NOTHROW(code.GetSegment(SegName));
    EXPECT_EQ(SegID, code.GetSegment(SegID).ID());
    EXPECT_EQ(SegName, code.GetSegment(SegID).Name());
    EXPECT_EQ(Offset, code.GetSegment(SegID).Offset());
    EXPECT_EQ(Data.size(), code.GetSegment(SegID).Size());
    EXPECT_TRUE(Core::Util::Compare(Data, code.GetSegment(SegID).Data()));
}

TEST_FIXTURE(ObjectCodeTest, AddSegmentMove)
{
    ObjectCode code(ModuleName);
    EXPECT_EQ(ModuleName, code.ModuleName());
    EXPECT_TRUE(code.HaveSegment(DefaultSegID));
    EXPECT_TRUE(code.HaveSegment(DefaultSegName));
    EXPECT_FALSE(code.HaveSegment(SegID));
    EXPECT_FALSE(code.HaveSegment(SegName));
    EXPECT_EQ(size_t{ 1 }, code.GetSegments().size());
    EXPECT_NOTHROW(code.GetSegment(DefaultSegID));
    EXPECT_NOTHROW(code.GetSegment(DefaultSegName));
    EXPECT_THROW(code.GetSegment(SegID), std::invalid_argument);
    EXPECT_THROW(code.GetSegment(SegName), std::invalid_argument);

    CodeSegment<uint16_t> segment(SegID, SegName);
    segment.SetOffset(Offset);
    segment.SetData(Data);
    code.AddSegment(std::move(segment));
    EXPECT_EQ(SegID, segment.ID());
    EXPECT_EQ(SegName, segment.Name());
    EXPECT_EQ(Offset, segment.Offset());
    EXPECT_EQ(Data.size(), segment.Size());
    EXPECT_TRUE(Core::Util::Compare(Data, segment.Data()));

    EXPECT_EQ(ModuleName, code.ModuleName());
    EXPECT_TRUE(code.HaveSegment(DefaultSegID));
    EXPECT_TRUE(code.HaveSegment(DefaultSegName));
    EXPECT_TRUE(code.HaveSegment(SegID));
    EXPECT_TRUE(code.HaveSegment(SegName));
    EXPECT_EQ(size_t{ 2 }, code.GetSegments().size());
    EXPECT_NOTHROW(code.GetSegment(DefaultSegID));
    EXPECT_NOTHROW(code.GetSegment(DefaultSegName));
    EXPECT_NOTHROW(code.GetSegment(SegID));
    EXPECT_NOTHROW(code.GetSegment(SegName));
    EXPECT_EQ(SegID, code.GetSegment(SegID).ID());
    EXPECT_EQ(SegName, code.GetSegment(SegID).Name());
    EXPECT_EQ(Offset, code.GetSegment(SegID).Offset());
    EXPECT_EQ(Data.size(), code.GetSegment(SegID).Size());
    EXPECT_TRUE(Core::Util::Compare(Data, code.GetSegment(SegID).Data()));
}

} // namespace Test

} // namespace ObjectFile
