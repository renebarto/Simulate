#include "unit-test-c++/UnitTestC++.h"

#include "BitArray.h"

using namespace std;

namespace Coco
{

namespace Test
{

class BitSetTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void BitSetTest::SetUp()
{
}

void BitSetTest::TearDown()
{
}

static const int Size = 7;

TEST_FIXTURE(BitSetTest, ConstructDefault)
{
    BitArray set;
    EXPECT_EQ(0, set.getCount());
    // There is no range check in the original code
    //EXPECT_THROW(set.Get(0), std::invalid_argument);
}

TEST_FIXTURE(BitSetTest, ConstructSize)
{
    BitArray set(Size);
    EXPECT_EQ(Size, set.getCount());
    for (int i = 0; i < Size; ++i)
        EXPECT_EQ(false, set.Get(i));
    // There is no range check in the original code
    //EXPECT_THROW(set.Get(Size + 1), std::invalid_argument);
}

TEST_FIXTURE(BitSetTest, ConstructSizeDefault)
{
    BitArray set(Size, true);
    EXPECT_EQ(Size, set.getCount());
    for (int i = 0; i < Size; ++i)
        EXPECT_EQ(true, set.Get(i));
    // There is no range check in the original code
    //EXPECT_THROW(set.Get(Size + 1), std::invalid_argument);
}

TEST_FIXTURE(BitSetTest, ConstructCopy)
{
    BitArray set(Size, true);
    BitArray other(set);
    EXPECT_EQ(Size, set.getCount());
    for (int i = 0; i < Size; ++i)
        EXPECT_EQ(true, set.Get(i));
    EXPECT_EQ(Size, other.getCount());
    for (int i = 0; i < Size; ++i)
        EXPECT_EQ(true, other.Get(i));
}

TEST_FIXTURE(BitSetTest, Assign)
{
    BitArray set(Size, true);
    BitArray other;
    other = set;
    EXPECT_EQ(Size, set.getCount());
    for (int i = 0; i < Size; ++i)
        EXPECT_EQ(true, set.Get(i));
    EXPECT_EQ(Size, other.getCount());
    for (int i = 0; i < Size; ++i)
        EXPECT_EQ(true, other.Get(i));
}

TEST_FIXTURE(BitSetTest, GetSet)
{
    BitArray set(Size, false);
    for (int i = 0; i < Size; ++i)
        EXPECT_EQ(false, set.Get(i));
    for (int i = 0; i < Size; ++i)
    {
        set.Set(i, true);
        for (int j = 0; j < Size; ++j)
            EXPECT_EQ(j <= i, set.Get(j));
    }
    for (int i = 0; i < Size; ++i)
        EXPECT_EQ(true, set.Get(i));
    for (int i = 0; i < Size; ++i)
    {
        set.Set(i, false);
        for (int j = 0; j < Size; ++j)
            EXPECT_EQ(j > i, set.Get(j));
    }
    for (int i = 0; i < Size; ++i)
        EXPECT_EQ(false, set.Get(i));
}

TEST_FIXTURE(BitSetTest, OperatorIndex)
{
    BitArray set(Size, false);
    for (int i = 0; i < Size; ++i)
        EXPECT_EQ(false, set[i]);
    for (int i = 0; i < Size; ++i)
    {
        set.Set(i, true);
        for (int j = 0; j < Size; ++j)
            EXPECT_EQ(j <= i, set[j]);
    }
    for (int i = 0; i < Size; ++i)
    {
        set.Set(i, false);
        for (int j = 0; j < Size; ++j)
            EXPECT_EQ(j > i, set[j]);
    }
}

TEST_FIXTURE(BitSetTest, SetAll)
{
    BitArray set(Size, false);
    for (int i = 0; i < Size; ++i)
        EXPECT_EQ(false, set[i]);
    set.SetAll(true);
    for (int i = 0; i < Size; ++i)
        EXPECT_EQ(true, set[i]);
    set.SetAll(false);
    for (int i = 0; i < Size; ++i)
        EXPECT_EQ(false, set[i]);
}

TEST_FIXTURE(BitSetTest, Equal)
{
    BitArray set(Size, false);
    BitArray ref1(Size, false);
    BitArray ref2(Size, true);
    BitArray ref3(Size, false);
    ref3.Set(0, true);
    
    EXPECT_TRUE(set.Equal(&set));
    EXPECT_TRUE(set.Equal(&ref1));
    EXPECT_TRUE(ref1.Equal(&set));
    EXPECT_FALSE(set.Equal(&ref2));
    EXPECT_FALSE(ref2.Equal(&set));
    EXPECT_FALSE(set.Equal(&ref3));
    EXPECT_FALSE(ref3.Equal(&set));
    set.Set(0, true);
    EXPECT_TRUE(set.Equal(&set));
    EXPECT_FALSE(set.Equal(&ref1));
    EXPECT_FALSE(ref1.Equal(&set));
    EXPECT_FALSE(set.Equal(&ref2));
    EXPECT_FALSE(ref2.Equal(&set));
    EXPECT_TRUE(set.Equal(&ref3));
    EXPECT_TRUE(ref3.Equal(&set));
    set.SetAll(true);
    EXPECT_TRUE(set.Equal(&set));
    EXPECT_FALSE(set.Equal(&ref1));
    EXPECT_FALSE(ref1.Equal(&set));
    EXPECT_TRUE(set.Equal(&ref2));
    EXPECT_TRUE(ref2.Equal(&set));
    EXPECT_FALSE(set.Equal(&ref3));
    EXPECT_FALSE(ref3.Equal(&set));
}

TEST_FIXTURE(BitSetTest, Overlaps)
{
    BitArray set(Size, false);
    BitArray ref1(Size, false);
    BitArray ref2(Size, true);
    BitArray ref3(Size, false);
    ref3.Set(0, true);
    BitArray ref4(Size, false);
    ref4.Set(1, true);

    EXPECT_FALSE(set.Overlaps(&ref1));
    EXPECT_FALSE(ref1.Overlaps(&set));
    EXPECT_FALSE(set.Overlaps(&ref2));
    EXPECT_FALSE(ref2.Overlaps(&set));
    EXPECT_FALSE(set.Overlaps(&ref3));
    EXPECT_FALSE(ref3.Overlaps(&set));
    EXPECT_TRUE(ref2.Overlaps(&ref3));
    EXPECT_TRUE(ref3.Overlaps(&ref2));
    EXPECT_TRUE(ref2.Overlaps(&ref4));
    EXPECT_TRUE(ref4.Overlaps(&ref2));
    set.Set(0, true);
    EXPECT_FALSE(set.Overlaps(&ref1));
    EXPECT_FALSE(ref1.Overlaps(&set));
    EXPECT_TRUE(set.Overlaps(&ref2));
    EXPECT_TRUE(ref2.Overlaps(&set));
    EXPECT_TRUE(set.Overlaps(&ref3));
    EXPECT_TRUE(ref3.Overlaps(&set));
    EXPECT_FALSE(set.Overlaps(&ref4));
    EXPECT_FALSE(ref4.Overlaps(&set));
    set.SetAll(true);
    EXPECT_FALSE(set.Overlaps(&ref1));
    EXPECT_FALSE(ref1.Overlaps(&set));
    EXPECT_TRUE(set.Overlaps(&ref2));
    EXPECT_TRUE(ref2.Overlaps(&set));
    EXPECT_TRUE(set.Overlaps(&ref3));
    EXPECT_TRUE(ref3.Overlaps(&set));
    EXPECT_TRUE(set.Overlaps(&ref4));
    EXPECT_TRUE(ref4.Overlaps(&set));
}

TEST_FIXTURE(BitSetTest, Not)
{
    BitArray set(Size, false);
    BitArray ref1(Size, false);
    BitArray ref2(Size, true);
    BitArray ref3(Size, false);
    ref3.Set(0, true);
    BitArray ref4(Size, true);
    ref4.Set(0, false);

    EXPECT_TRUE(set.Equal(&ref1));
    EXPECT_FALSE(set.Equal(&ref2));
    EXPECT_FALSE(set.Equal(&ref3));
    EXPECT_FALSE(set.Equal(&ref4));
    set.Not();
    EXPECT_FALSE(set.Equal(&ref1));
    EXPECT_TRUE(set.Equal(&ref2));
    EXPECT_FALSE(set.Equal(&ref3));
    EXPECT_FALSE(set.Equal(&ref4));
    set.Set(0, false);
    EXPECT_FALSE(set.Equal(&ref1));
    EXPECT_FALSE(set.Equal(&ref2));
    EXPECT_FALSE(set.Equal(&ref3));
    EXPECT_TRUE(set.Equal(&ref4));
    set.Not();
    EXPECT_FALSE(set.Equal(&ref1));
    EXPECT_FALSE(set.Equal(&ref2));
    EXPECT_TRUE(set.Equal(&ref3));
    EXPECT_FALSE(set.Equal(&ref4));
}

TEST_FIXTURE(BitSetTest, And)
{
    BitArray set(Size, false);
    BitArray ref1(Size, false);
    BitArray ref2(Size, true);
    BitArray ref3(Size, false);
    ref3.Set(0, true);
    BitArray ref4(Size, true);
    ref4.Set(0, false);

    EXPECT_TRUE(set.Equal(&ref1));
    EXPECT_FALSE(set.Equal(&ref2));
    EXPECT_FALSE(set.Equal(&ref3));
    EXPECT_FALSE(set.Equal(&ref4));
    set.And(&ref2);
    EXPECT_TRUE(set.Equal(&ref1));
    EXPECT_FALSE(set.Equal(&ref2));
    EXPECT_FALSE(set.Equal(&ref3));
    EXPECT_FALSE(set.Equal(&ref4));
    set.SetAll(true);
    set.And(&ref2);
    EXPECT_FALSE(set.Equal(&ref1));
    EXPECT_TRUE(set.Equal(&ref2));
    EXPECT_FALSE(set.Equal(&ref3));
    EXPECT_FALSE(set.Equal(&ref4));
    set.And(&ref4);
    EXPECT_FALSE(set.Equal(&ref1));
    EXPECT_FALSE(set.Equal(&ref2));
    EXPECT_FALSE(set.Equal(&ref3));
    EXPECT_TRUE(set.Equal(&ref4));
    set = ref3;
    set.And(&ref2);
    EXPECT_FALSE(set.Equal(&ref1));
    EXPECT_FALSE(set.Equal(&ref2));
    EXPECT_TRUE(set.Equal(&ref3));
    EXPECT_FALSE(set.Equal(&ref4));
    set.And(&ref4);
    EXPECT_TRUE(set.Equal(&ref1));
    EXPECT_FALSE(set.Equal(&ref2));
    EXPECT_FALSE(set.Equal(&ref3));
    EXPECT_FALSE(set.Equal(&ref4));
}

TEST_FIXTURE(BitSetTest, Or)
{
    BitArray set(Size, false);
    BitArray ref1(Size, false);
    BitArray ref2(Size, true);
    BitArray ref3(Size, false);
    ref3.Set(0, true);
    BitArray ref4(Size, true);
    ref4.Set(0, false);

    EXPECT_TRUE(set.Equal(&ref1));
    EXPECT_FALSE(set.Equal(&ref2));
    EXPECT_FALSE(set.Equal(&ref3));
    EXPECT_FALSE(set.Equal(&ref4));
    set.Or(&ref2);
    EXPECT_FALSE(set.Equal(&ref1));
    EXPECT_TRUE(set.Equal(&ref2));
    EXPECT_FALSE(set.Equal(&ref3));
    EXPECT_FALSE(set.Equal(&ref4));
    set = ref1;
    set.Or(&ref3);
    EXPECT_FALSE(set.Equal(&ref1));
    EXPECT_FALSE(set.Equal(&ref2));
    EXPECT_TRUE(set.Equal(&ref3));
    EXPECT_FALSE(set.Equal(&ref4));
    set.Or(&ref4);
    EXPECT_FALSE(set.Equal(&ref1));
    EXPECT_TRUE(set.Equal(&ref2));
    EXPECT_FALSE(set.Equal(&ref3));
    EXPECT_FALSE(set.Equal(&ref4));
    set = ref4;
    set.Or(&ref3);
    EXPECT_FALSE(set.Equal(&ref1));
    EXPECT_TRUE(set.Equal(&ref2));
    EXPECT_FALSE(set.Equal(&ref3));
    EXPECT_FALSE(set.Equal(&ref4));
}

TEST_FIXTURE(BitSetTest, Xor)
{
    BitArray set(Size, false);
    BitArray ref1(Size, false);
    BitArray ref2(Size, true);
    BitArray ref3(Size, false);
    ref3.Set(0, true);
    BitArray ref4(Size, true);
    ref4.Set(0, false);

    EXPECT_TRUE(set.Equal(&ref1));
    EXPECT_FALSE(set.Equal(&ref2));
    EXPECT_FALSE(set.Equal(&ref3));
    EXPECT_FALSE(set.Equal(&ref4));
    set.Xor(&ref2);
    EXPECT_FALSE(set.Equal(&ref1));
    EXPECT_TRUE(set.Equal(&ref2));
    EXPECT_FALSE(set.Equal(&ref3));
    EXPECT_FALSE(set.Equal(&ref4));
    set.Xor(&ref3);
    EXPECT_FALSE(set.Equal(&ref1));
    EXPECT_FALSE(set.Equal(&ref2));
    EXPECT_FALSE(set.Equal(&ref3));
    EXPECT_TRUE(set.Equal(&ref4));
    set.Xor(&ref2);
    EXPECT_FALSE(set.Equal(&ref1));
    EXPECT_FALSE(set.Equal(&ref2));
    EXPECT_TRUE(set.Equal(&ref3));
    EXPECT_FALSE(set.Equal(&ref4));
    set.Xor(&ref3);
    EXPECT_TRUE(set.Equal(&ref1));
    EXPECT_FALSE(set.Equal(&ref2));
    EXPECT_FALSE(set.Equal(&ref3));
    EXPECT_FALSE(set.Equal(&ref4));
}

} // namespace Test

} // namespace Simulate
