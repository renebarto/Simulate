#include "unit-test-c++/UnitTestC++.h"

#include "BitSet.h"

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

static const size_t Size = 7;

TEST_FIXTURE(BitSetTest, ConstructDefault)
{
    BitSet set;
    EXPECT_EQ(0, set.Count());
    EXPECT_THROW(set.Get(0), std::invalid_argument);
}

TEST_FIXTURE(BitSetTest, ConstructSize)
{
    BitSet set(Size);
    EXPECT_EQ(Size, set.Count());
    for (size_t i = 0; i < Size; ++i)
        EXPECT_EQ(false, set.Get(i));
    EXPECT_THROW(set.Get(Size + 1), std::invalid_argument);
}

TEST_FIXTURE(BitSetTest, ConstructSizeDefault)
{
    BitSet set(Size, true);
    EXPECT_EQ(Size, set.Count());
    for (size_t i = 0; i < Size; ++i)
        EXPECT_EQ(true, set.Get(i));
    EXPECT_THROW(set.Get(Size + 1), std::invalid_argument);
}

TEST_FIXTURE(BitSetTest, ConstructCopy)
{
    BitSet set(Size, true);
    BitSet other(set);
    EXPECT_EQ(Size, set.Count());
    for (size_t i = 0; i < Size; ++i)
        EXPECT_EQ(true, set.Get(i));
    EXPECT_EQ(Size, other.Count());
    for (size_t i = 0; i < Size; ++i)
        EXPECT_EQ(true, other.Get(i));
}

TEST_FIXTURE(BitSetTest, Clone)
{
    BitSet set(Size, true);
    BitSet other(set.Clone());
    EXPECT_EQ(Size, set.Count());
    for (size_t i = 0; i < Size; ++i)
        EXPECT_EQ(true, set.Get(i));
    EXPECT_EQ(Size, other.Count());
    for (size_t i = 0; i < Size; ++i)
        EXPECT_EQ(true, other.Get(i));
}

TEST_FIXTURE(BitSetTest, Assign)
{
    BitSet set(Size, true);
    BitSet other;
    other = set;
    EXPECT_EQ(Size, set.Count());
    for (size_t i = 0; i < Size; ++i)
        EXPECT_EQ(true, set.Get(i));
    EXPECT_EQ(Size, other.Count());
    for (size_t i = 0; i < Size; ++i)
        EXPECT_EQ(true, other.Get(i));
}

TEST_FIXTURE(BitSetTest, GetSet)
{
    BitSet set(Size, false);
    for (size_t i = 0; i < Size; ++i)
        EXPECT_EQ(false, set.Get(i));
    for (size_t i = 0; i < Size; ++i)
    {
        set.Set(i, true);
        for (size_t j = 0; j < Size; ++j)
            EXPECT_EQ(j <= i, set.Get(j));
    }
    for (size_t i = 0; i < Size; ++i)
        EXPECT_EQ(true, set.Get(i));
    for (size_t i = 0; i < Size; ++i)
    {
        set.Set(i, false);
        for (size_t j = 0; j < Size; ++j)
            EXPECT_EQ(j > i, set.Get(j));
    }
    for (size_t i = 0; i < Size; ++i)
        EXPECT_EQ(false, set.Get(i));
}

TEST_FIXTURE(BitSetTest, OperatorIndex)
{
    BitSet set(Size, false);
    for (size_t i = 0; i < Size; ++i)
        EXPECT_EQ(false, set[i]);
    for (size_t i = 0; i < Size; ++i)
    {
        set.Set(i, true);
        for (size_t j = 0; j < Size; ++j)
            EXPECT_EQ(j <= i, set[j]);
    }
    for (size_t i = 0; i < Size; ++i)
    {
        set.Set(i, false);
        for (size_t j = 0; j < Size; ++j)
            EXPECT_EQ(j > i, set[j]);
    }
}

TEST_FIXTURE(BitSetTest, SetAll)
{
    BitSet set(Size, false);
    for (size_t i = 0; i < Size; ++i)
        EXPECT_EQ(false, set[i]);
    set.SetAll(true);
    for (size_t i = 0; i < Size; ++i)
        EXPECT_EQ(true, set[i]);
    set.SetAll(false);
    for (size_t i = 0; i < Size; ++i)
        EXPECT_EQ(false, set[i]);
}

TEST_FIXTURE(BitSetTest, OperatorEqual)
{
    BitSet set(Size, false);
    BitSet ref1(Size, false);
    BitSet ref2(Size, true);
    BitSet ref3(Size, false);
    ref3.Set(0, true);
    
    EXPECT_TRUE(set == set);
    EXPECT_TRUE(set == ref1);
    EXPECT_TRUE(ref1 == set);
    EXPECT_FALSE(set == ref2);
    EXPECT_FALSE(ref2 == set);
    EXPECT_FALSE(set == ref3);
    EXPECT_FALSE(ref3 == set);
    EXPECT_FALSE(ref2 == ref1);
    EXPECT_FALSE(ref2 == ref3);
    set.Set(0, true);
    EXPECT_TRUE(set == set);
    EXPECT_FALSE(set == ref1);
    EXPECT_FALSE(ref1 == set);
    EXPECT_FALSE(set == ref2);
    EXPECT_FALSE(ref2 == set);
    EXPECT_TRUE(set == ref3);
    EXPECT_TRUE(ref3 == set);
    set.SetAll(true);
    EXPECT_TRUE(set == set);
    EXPECT_FALSE(set == ref1);
    EXPECT_FALSE(ref1 == set);
    EXPECT_TRUE(set == ref2);
    EXPECT_TRUE(ref2 == set);
    EXPECT_FALSE(set == ref3);
    EXPECT_FALSE(ref3 == set);
}

TEST_FIXTURE(BitSetTest, OperatorInequal)
{
    BitSet set(Size, false);
    BitSet ref1(Size, false);
    BitSet ref2(Size, true);
    BitSet ref3(Size, false);
    ref3.Set(0, true);
    
    EXPECT_FALSE(set != set);
    EXPECT_FALSE(set != ref1);
    EXPECT_FALSE(ref1 != set);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(ref2 != set);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(ref3 != set);
    EXPECT_TRUE(ref2 != ref1);
    EXPECT_TRUE(ref2 != ref3);
    set.Set(0, true);
    EXPECT_FALSE(set != set);
    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(ref1 != set);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(ref2 != set);
    EXPECT_FALSE(set != ref3);
    EXPECT_FALSE(ref3 != set);
    set.SetAll(true);
    EXPECT_FALSE(set != set);
    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(ref1 != set);
    EXPECT_FALSE(set != ref2);
    EXPECT_FALSE(ref2 != set);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(ref3 != set);
}

TEST_FIXTURE(BitSetTest, Overlaps)
{
    BitSet set(Size, false);
    BitSet ref1(Size, false);
    BitSet ref2(Size, true);
    BitSet ref3(Size, false);
    ref3.Set(0, true);
    BitSet ref4(Size, false);
    ref4.Set(1, true);

    EXPECT_FALSE(set.Overlaps(ref1));
    EXPECT_FALSE(ref1.Overlaps(set));
    EXPECT_FALSE(set.Overlaps(ref2));
    EXPECT_FALSE(ref2.Overlaps(set));
    EXPECT_FALSE(set.Overlaps(ref3));
    EXPECT_FALSE(ref3.Overlaps(set));
    EXPECT_TRUE(ref2.Overlaps(ref3));
    EXPECT_TRUE(ref3.Overlaps(ref2));
    EXPECT_TRUE(ref2.Overlaps(ref4));
    EXPECT_TRUE(ref4.Overlaps(ref2));
    set.Set(0, true);
    EXPECT_FALSE(set.Overlaps(ref1));
    EXPECT_FALSE(ref1.Overlaps(set));
    EXPECT_TRUE(set.Overlaps(ref2));
    EXPECT_TRUE(ref2.Overlaps(set));
    EXPECT_TRUE(set.Overlaps(ref3));
    EXPECT_TRUE(ref3.Overlaps(set));
    EXPECT_FALSE(set.Overlaps(ref4));
    EXPECT_FALSE(ref4.Overlaps(set));
    set.SetAll(true);
    EXPECT_FALSE(set.Overlaps(ref1));
    EXPECT_FALSE(ref1.Overlaps(set));
    EXPECT_TRUE(set.Overlaps(ref2));
    EXPECT_TRUE(ref2.Overlaps(set));
    EXPECT_TRUE(set.Overlaps(ref3));
    EXPECT_TRUE(ref3.Overlaps(set));
    EXPECT_TRUE(set.Overlaps(ref4));
    EXPECT_TRUE(ref4.Overlaps(set));
}

TEST_FIXTURE(BitSetTest, Includes)
{
    BitSet set(Size, false);
    BitSet ref1(Size, false);
    BitSet ref2(Size, true);
    BitSet ref3(Size, false);
    ref3.Set(0, true);
    BitSet ref4(Size, false);
    ref4.Set(1, true);

    EXPECT_TRUE(set.Includes(ref1));
    EXPECT_TRUE(ref1.Includes(set));
    EXPECT_FALSE(set.Includes(ref2));
    EXPECT_TRUE(ref2.Includes(set));
    EXPECT_FALSE(set.Includes(ref3));
    EXPECT_TRUE(ref3.Includes(set));
    EXPECT_TRUE(ref2.Includes(ref3));
    EXPECT_FALSE(ref3.Includes(ref2));
    EXPECT_TRUE(ref2.Includes(ref4));
    EXPECT_FALSE(ref4.Includes(ref2));
    set.Set(0, true);
    EXPECT_TRUE(set.Includes(ref1));
    EXPECT_FALSE(ref1.Includes(set));
    EXPECT_FALSE(set.Includes(ref2));
    EXPECT_TRUE(ref2.Includes(set));
    EXPECT_TRUE(set.Includes(ref3));
    EXPECT_TRUE(ref3.Includes(set));
    EXPECT_FALSE(set.Includes(ref4));
    EXPECT_FALSE(ref4.Includes(set));
    set.SetAll(true);
    EXPECT_TRUE(set.Includes(ref1));
    EXPECT_FALSE(ref1.Includes(set));
    EXPECT_TRUE(set.Includes(ref2));
    EXPECT_TRUE(ref2.Includes(set));
    EXPECT_TRUE(set.Includes(ref3));
    EXPECT_FALSE(ref3.Includes(set));
    EXPECT_TRUE(set.Includes(ref4));
    EXPECT_FALSE(ref4.Includes(set));
}

TEST_FIXTURE(BitSetTest, First)
{
    BitSet ref1(Size, false);
    BitSet ref2(Size, true);
    BitSet ref3(Size, false);
    ref3.Set(3, true);
    BitSet ref4(Size, true);
    ref4.Set(0, false);

    EXPECT_EQ(size_t(-1), ref1.First());
    EXPECT_EQ(size_t{ 0 }, ref2.First());
    EXPECT_EQ(size_t{ 3 }, ref3.First());
    EXPECT_EQ(size_t{ 1 }, ref4.First());
}

TEST_FIXTURE(BitSetTest, Elements)
{
    BitSet ref1(Size, false);
    BitSet ref2(Size, true);
    BitSet ref3(Size, false);
    ref3.Set(0, true);
    BitSet ref4(Size, true);
    ref4.Set(0, false);

    EXPECT_EQ(size_t{ 0 }, ref1.Elements());
    EXPECT_EQ(size_t{ Size }, ref2.Elements());
    EXPECT_EQ(size_t{ 1 }, ref3.Elements());
    EXPECT_EQ(size_t{ Size - 1 }, ref4.Elements());
}

TEST_FIXTURE(BitSetTest, OperatorNot)
{
    BitSet set(Size, false);
    BitSet ref1(Size, false);
    BitSet ref2(Size, true);
    BitSet ref3(Size, false);
    ref3.Set(0, true);
    BitSet ref4(Size, true);
    ref4.Set(0, false);

    EXPECT_TRUE(set == ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set != ref4);

    EXPECT_TRUE(~set != ref1);
    EXPECT_TRUE(~set == ref2);
    EXPECT_TRUE(~set != ref3);
    EXPECT_TRUE(~set != ref4);

    set = ref4;
    EXPECT_TRUE(~set != ref1);
    EXPECT_TRUE(~set != ref2);
    EXPECT_TRUE(~set == ref3);
    EXPECT_TRUE(~set != ref4);
}

TEST_FIXTURE(BitSetTest, OperatorAndEquals)
{
    BitSet set(Size, false);
    BitSet ref1(Size, false);
    BitSet ref2(Size, true);
    BitSet ref3(Size, false);
    ref3.Set(0, true);
    BitSet ref4(Size, true);
    ref4.Set(0, false);

    EXPECT_TRUE(set == ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set != ref4);
    set &= ref2;
    EXPECT_TRUE(set == ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set != ref4);
    set.SetAll(true);
    set &= ref2;
    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set == ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set != ref4);
    set &= ref4;
    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set == ref4);
    set = ref3;
    set &= ref2;
    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set == ref3);
    EXPECT_TRUE(set != ref4);
    set &= ref4;
    EXPECT_TRUE(set == ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set != ref4);
}

TEST_FIXTURE(BitSetTest, OperatorAnd)
{
    BitSet set(Size, false);
    BitSet ref1(Size, false);
    BitSet ref2(Size, true);
    BitSet ref3(Size, false);
    ref3.Set(0, true);
    BitSet ref4(Size, true);
    ref4.Set(0, false);

    EXPECT_TRUE((set & ref2) == ref1);
    EXPECT_TRUE((set & ref2) != ref2);
    EXPECT_TRUE((set & ref2) != ref3);
    EXPECT_TRUE((set & ref2) != ref4);
    EXPECT_TRUE((set & ref4) == ref1);
    EXPECT_TRUE((set & ref4) != ref2);
    EXPECT_TRUE((set & ref4) != ref3);
    EXPECT_TRUE((set & ref4) != ref4);
    set.SetAll(true);
    EXPECT_TRUE((set & ref2) != ref1);
    EXPECT_TRUE((set & ref2) == ref2);
    EXPECT_TRUE((set & ref2) != ref3);
    EXPECT_TRUE((set & ref2) != ref4);
    EXPECT_TRUE((set & ref4) != ref1);
    EXPECT_TRUE((set & ref4) != ref2);
    EXPECT_TRUE((set & ref4) != ref3);
    EXPECT_TRUE((set & ref4) == ref4);
    set = ref3;
    EXPECT_TRUE((set & ref2) != ref1);
    EXPECT_TRUE((set & ref2) != ref2);
    EXPECT_TRUE((set & ref2) == ref3);
    EXPECT_TRUE((set & ref2) != ref4);
    EXPECT_TRUE((set & ref4) == ref1);
    EXPECT_TRUE((set & ref4) != ref2);
    EXPECT_TRUE((set & ref4) != ref3);
    EXPECT_TRUE((set & ref4) != ref4);
}

TEST_FIXTURE(BitSetTest, OperatorOrEquals)
{
    BitSet set(Size, false);
    BitSet ref1(Size, false);
    BitSet ref2(Size, true);
    BitSet ref3(Size, false);
    ref3.Set(0, true);
    BitSet ref4(Size, true);
    ref4.Set(0, false);

    EXPECT_TRUE(set == ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set != ref4);
    set |= ref2;
    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set == ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set != ref4);
    set = ref1;
    set |= ref3;
    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set == ref3);
    EXPECT_TRUE(set != ref4);
    set |= ref4;
    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set == ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set != ref4);
    set = ref4;
    set |= ref3;
    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set == ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set != ref4);
}

TEST_FIXTURE(BitSetTest, OperatorOr)
{
    BitSet set(Size, false);
    BitSet ref1(Size, false);
    BitSet ref2(Size, true);
    BitSet ref3(Size, false);
    ref3.Set(0, true);
    BitSet ref4(Size, true);
    ref4.Set(0, false);

    EXPECT_TRUE((set | ref2) != ref1);
    EXPECT_TRUE((set | ref2) == ref2);
    EXPECT_TRUE((set | ref2) != ref3);
    EXPECT_TRUE((set | ref2) != ref4);
    EXPECT_TRUE((set | ref4) != ref1);
    EXPECT_TRUE((set | ref4) != ref2);
    EXPECT_TRUE((set | ref4) != ref3);
    EXPECT_TRUE((set | ref4) == ref4);
    set = ref1;
    EXPECT_TRUE((set | ref3) != ref1);
    EXPECT_TRUE((set | ref3) != ref2);
    EXPECT_TRUE((set | ref3) == ref3);
    EXPECT_TRUE((set | ref3) != ref4);
    set = ref3;
    EXPECT_TRUE((set | ref4) != ref1);
    EXPECT_TRUE((set | ref4) == ref2);
    EXPECT_TRUE((set | ref4) != ref3);
    EXPECT_TRUE((set | ref4) != ref4);
    set = ref4;
    EXPECT_TRUE((set | ref3) != ref1);
    EXPECT_TRUE((set | ref3) == ref2);
    EXPECT_TRUE((set | ref3) != ref3);
    EXPECT_TRUE((set | ref3) != ref4);
}

TEST_FIXTURE(BitSetTest, OperatorXorEquals)
{
    BitSet set(Size, false);
    BitSet ref1(Size, false);
    BitSet ref2(Size, true);
    BitSet ref3(Size, false);
    ref3.Set(0, true);
    BitSet ref4(Size, true);
    ref4.Set(0, false);

    EXPECT_TRUE(set == ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set != ref4);
    set ^= ref2;
    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set == ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set != ref4);
    set ^= ref3;
    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set == ref4);
    set ^= ref2;
    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set == ref3);
    EXPECT_TRUE(set != ref4);
    set ^= ref3;
    EXPECT_TRUE(set == ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set != ref4);
}

TEST_FIXTURE(BitSetTest, OperatorXor)
{
    BitSet set(Size, false);
    BitSet ref1(Size, false);
    BitSet ref2(Size, true);
    BitSet ref3(Size, false);
    ref3.Set(0, true);
    BitSet ref4(Size, true);
    ref4.Set(0, false);

    EXPECT_TRUE((set ^ ref2) != ref1);
    EXPECT_TRUE((set ^ ref2) == ref2);
    EXPECT_TRUE((set ^ ref2) != ref3);
    EXPECT_TRUE((set ^ ref2) != ref4);
    EXPECT_TRUE((set ^ ref4) != ref1);
    EXPECT_TRUE((set ^ ref4) != ref2);
    EXPECT_TRUE((set ^ ref4) != ref3);
    EXPECT_TRUE((set ^ ref4) == ref4);
    set = ref2;
    EXPECT_TRUE((set ^ ref3) != ref1);
    EXPECT_TRUE((set ^ ref3) != ref2);
    EXPECT_TRUE((set ^ ref3) != ref3);
    EXPECT_TRUE((set ^ ref3) == ref4);
    set = ref4;
    EXPECT_TRUE((set ^ ref2) != ref1);
    EXPECT_TRUE((set ^ ref2) != ref2);
    EXPECT_TRUE((set ^ ref2) == ref3);
    EXPECT_TRUE((set ^ ref2) != ref4);
    set = ref3;
    EXPECT_TRUE((set ^ ref3) == ref1);
    EXPECT_TRUE((set ^ ref3) != ref2);
    EXPECT_TRUE((set ^ ref3) != ref3);
    EXPECT_TRUE((set ^ ref3) != ref4);
}

TEST_FIXTURE(BitSetTest, OperatorMinusEquals)
{
    BitSet set(Size, false);
    BitSet ref1(Size, false);
    BitSet ref2(Size, true);
    BitSet ref3(Size, false);
    ref3.Set(0, true);
    BitSet ref4(Size, true);
    ref4.Set(0, false);

    EXPECT_TRUE(set == ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set != ref4);
    set -= ref2;
    EXPECT_TRUE(set == ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set != ref4);
    set = ref2;
    set -= ref3;
    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set == ref4);
    set -= ref2;
    EXPECT_TRUE(set == ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set != ref4);
    set = ref4;
    set -= ref1;
    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set == ref4);
    set -= ref3;
    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set == ref4);
}

TEST_FIXTURE(BitSetTest, OperatorMinus)
{
    BitSet set(Size, false);
    BitSet ref1(Size, false);
    BitSet ref2(Size, true);
    BitSet ref3(Size, false);
    ref3.Set(0, true);
    BitSet ref4(Size, true);
    ref4.Set(0, false);

    EXPECT_TRUE((set - ref2) == ref1);
    EXPECT_TRUE((set - ref2) != ref2);
    EXPECT_TRUE((set - ref2) != ref3);
    EXPECT_TRUE((set - ref2) != ref4);
    EXPECT_TRUE((set - ref4) == ref1);
    EXPECT_TRUE((set - ref4) != ref2);
    EXPECT_TRUE((set - ref4) != ref3);
    EXPECT_TRUE((set - ref4) != ref4);
    set = ref2;
    EXPECT_TRUE((set - ref3) != ref1);
    EXPECT_TRUE((set - ref3) != ref2);
    EXPECT_TRUE((set - ref3) != ref3);
    EXPECT_TRUE((set - ref3) == ref4);
    set = ref3;
    EXPECT_TRUE((set - ref1) != ref1);
    EXPECT_TRUE((set - ref1) != ref2);
    EXPECT_TRUE((set - ref1) == ref3);
    EXPECT_TRUE((set - ref1) != ref4);
    set = ref4;
    EXPECT_TRUE((set - ref3) != ref1);
    EXPECT_TRUE((set - ref3) != ref2);
    EXPECT_TRUE((set - ref3) != ref3);
    EXPECT_TRUE((set - ref3) == ref4);
}

} // namespace Test

} // namespace Simulate
