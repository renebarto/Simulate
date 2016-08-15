#include "unit-test-c++/UnitTestC++.h"

#include "CharSet.h"

using namespace std;

namespace Coco
{

namespace Test
{

class CharSetTest : public UnitTestCpp::TestFixture
{
public:
	virtual void SetUp();
	virtual void TearDown();
};

void CharSetTest::SetUp()
{
}

void CharSetTest::TearDown()
{
}

TEST_FIXTURE(CharSetTest, ConstructDefault)
{
    CharSet set;
    EXPECT_NULL(set.FirstRange());
}

TEST_FIXTURE(CharSetTest, Clone)
{
    CharSet set;
    set.Set(0);
    EXPECT_NOT_NULL(set.FirstRange());

    CharSet & other = set.Clone();
    EXPECT_NOT_NULL(other.FirstRange());
    EXPECT_TRUE(other.Get(0));
}

TEST_FIXTURE(CharSetTest, GetSet)
{
    //CharSet set(Size, false);
    //for (size_t i = 0; i < Size; ++i)
    //    EXPECT_EQ(false, set.Get(i));
    //for (size_t i = 0; i < Size; ++i)
    //{
    //    set.Set(i, true);
    //    for (size_t j = 0; j < Size; ++j)
    //        EXPECT_EQ(j <= i, set.Get(j));
    //}
    //for (size_t i = 0; i < Size; ++i)
    //    EXPECT_EQ(true, set.Get(i));
    //for (size_t i = 0; i < Size; ++i)
    //{
    //    set.Set(i, false);
    //    for (size_t j = 0; j < Size; ++j)
    //        EXPECT_EQ(j > i, set.Get(j));
    //}
    //for (size_t i = 0; i < Size; ++i)
    //    EXPECT_EQ(false, set.Get(i));
}

TEST_FIXTURE(CharSetTest, Equal)
{
    //CharSet set(Size, false);
    //CharSet ref1(Size, false);
    //CharSet ref2(Size, true);
    //CharSet ref3(Size, false);
    //ref3.Set(0, true);
    //
    //EXPECT_TRUE(set.Equal(set));
    //EXPECT_TRUE(set.Equal(ref1));
    //EXPECT_TRUE(ref1.Equal(set));
    //EXPECT_FALSE(set.Equal(ref2));
    //EXPECT_FALSE(ref2.Equal(set));
    //EXPECT_FALSE(set.Equal(ref3));
    //EXPECT_FALSE(ref3.Equal(set));
    //EXPECT_FALSE(ref2.Equal(ref1));
    //EXPECT_FALSE(ref2.Equal(ref3));
    //set.Set(0, true);
    //EXPECT_TRUE(set.Equal(set));
    //EXPECT_FALSE(set.Equal(ref1));
    //EXPECT_FALSE(ref1.Equal(set));
    //EXPECT_FALSE(set.Equal(ref2));
    //EXPECT_FALSE(ref2.Equal(set));
    //EXPECT_TRUE(set.Equal(ref3));
    //EXPECT_TRUE(ref3.Equal(set));
    //set.SetAll(true);
    //EXPECT_TRUE(set.Equal(set));
    //EXPECT_FALSE(set.Equal(ref1));
    //EXPECT_FALSE(ref1.Equal(set));
    //EXPECT_TRUE(set.Equal(ref2));
    //EXPECT_TRUE(ref2.Equal(set));
    //EXPECT_FALSE(set.Equal(ref3));
    //EXPECT_FALSE(ref3.Equal(set));
}

TEST_FIXTURE(CharSetTest, OperatorEqual)
{
    //CharSet set(Size, false);
    //CharSet ref1(Size, false);
    //CharSet ref2(Size, true);
    //CharSet ref3(Size, false);
    //ref3.Set(0, true);
    //
    //EXPECT_TRUE(set == set);
    //EXPECT_TRUE(set == ref1);
    //EXPECT_TRUE(ref1 == set);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_FALSE(ref2 == set);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_FALSE(ref3 == set);
    //EXPECT_FALSE(ref2 == ref1);
    //EXPECT_FALSE(ref2 == ref3);
    //set.Set(0, true);
    //EXPECT_TRUE(set == set);
    //EXPECT_FALSE(set == ref1);
    //EXPECT_FALSE(ref1 == set);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_FALSE(ref2 == set);
    //EXPECT_TRUE(set == ref3);
    //EXPECT_TRUE(ref3 == set);
    //set.SetAll(true);
    //EXPECT_TRUE(set == set);
    //EXPECT_FALSE(set == ref1);
    //EXPECT_FALSE(ref1 == set);
    //EXPECT_TRUE(set == ref2);
    //EXPECT_TRUE(ref2 == set);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_FALSE(ref3 == set);
}

TEST_FIXTURE(CharSetTest, OperatorInequal)
{
    //CharSet set(Size, false);
    //CharSet ref1(Size, false);
    //CharSet ref2(Size, true);
    //CharSet ref3(Size, false);
    //ref3.Set(0, true);
    //
    //EXPECT_FALSE(set != set);
    //EXPECT_FALSE(set != ref1);
    //EXPECT_FALSE(ref1 != set);
    //EXPECT_TRUE(set != ref2);
    //EXPECT_TRUE(ref2 != set);
    //EXPECT_TRUE(set != ref3);
    //EXPECT_TRUE(ref3 != set);
    //EXPECT_TRUE(ref2 != ref1);
    //EXPECT_TRUE(ref2 != ref3);
    //set.Set(0, true);
    //EXPECT_FALSE(set != set);
    //EXPECT_TRUE(set != ref1);
    //EXPECT_TRUE(ref1 != set);
    //EXPECT_TRUE(set != ref2);
    //EXPECT_TRUE(ref2 != set);
    //EXPECT_FALSE(set != ref3);
    //EXPECT_FALSE(ref3 != set);
    //set.SetAll(true);
    //EXPECT_FALSE(set != set);
    //EXPECT_TRUE(set != ref1);
    //EXPECT_TRUE(ref1 != set);
    //EXPECT_FALSE(set != ref2);
    //EXPECT_FALSE(ref2 != set);
    //EXPECT_TRUE(set != ref3);
    //EXPECT_TRUE(ref3 != set);
}

TEST_FIXTURE(CharSetTest, Overlaps)
{
    //CharSet set(Size, false);
    //CharSet ref1(Size, false);
    //CharSet ref2(Size, true);
    //CharSet ref3(Size, false);
    //ref3.Set(0, true);
    //CharSet ref4(Size, false);
    //ref4.Set(1, true);

    //EXPECT_FALSE(set.Overlaps(ref1));
    //EXPECT_FALSE(ref1.Overlaps(set));
    //EXPECT_FALSE(set.Overlaps(ref2));
    //EXPECT_FALSE(ref2.Overlaps(set));
    //EXPECT_FALSE(set.Overlaps(ref3));
    //EXPECT_FALSE(ref3.Overlaps(set));
    //EXPECT_TRUE(ref2.Overlaps(ref3));
    //EXPECT_TRUE(ref3.Overlaps(ref2));
    //EXPECT_TRUE(ref2.Overlaps(ref4));
    //EXPECT_TRUE(ref4.Overlaps(ref2));
    //set.Set(0, true);
    //EXPECT_FALSE(set.Overlaps(ref1));
    //EXPECT_FALSE(ref1.Overlaps(set));
    //EXPECT_TRUE(set.Overlaps(ref2));
    //EXPECT_TRUE(ref2.Overlaps(set));
    //EXPECT_TRUE(set.Overlaps(ref3));
    //EXPECT_TRUE(ref3.Overlaps(set));
    //EXPECT_FALSE(set.Overlaps(ref4));
    //EXPECT_FALSE(ref4.Overlaps(set));
    //set.SetAll(true);
    //EXPECT_FALSE(set.Overlaps(ref1));
    //EXPECT_FALSE(ref1.Overlaps(set));
    //EXPECT_TRUE(set.Overlaps(ref2));
    //EXPECT_TRUE(ref2.Overlaps(set));
    //EXPECT_TRUE(set.Overlaps(ref3));
    //EXPECT_TRUE(ref3.Overlaps(set));
    //EXPECT_TRUE(set.Overlaps(ref4));
    //EXPECT_TRUE(ref4.Overlaps(set));
}

TEST_FIXTURE(CharSetTest, Not)
{
    //CharSet set(Size, false);
    //CharSet ref1(Size, false);
    //CharSet ref2(Size, true);
    //CharSet ref3(Size, false);
    //ref3.Set(0, true);
    //CharSet ref4(Size, true);
    //ref4.Set(0, false);

    //EXPECT_TRUE(set == ref1);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_FALSE(set == ref4);
    //set.Not();
    //EXPECT_FALSE(set == ref1);
    //EXPECT_TRUE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_FALSE(set == ref4);
    //set.Set(0, false);
    //EXPECT_FALSE(set == ref1);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_TRUE(set == ref4);
    //set.Not();
    //EXPECT_FALSE(set == ref1);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_TRUE(set == ref3);
    //EXPECT_FALSE(set == ref4);
}

TEST_FIXTURE(CharSetTest, OperatorNot)
{
    //CharSet set(Size, false);
    //CharSet ref1(Size, false);
    //CharSet ref2(Size, true);
    //CharSet ref3(Size, false);
    //ref3.Set(0, true);
    //CharSet ref4(Size, true);
    //ref4.Set(0, false);

    //EXPECT_TRUE(set == ref1);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_FALSE(set == ref4);

    //EXPECT_FALSE(~set == ref1);
    //EXPECT_TRUE(~set == ref2);
    //EXPECT_FALSE(~set == ref3);
    //EXPECT_FALSE(~set == ref4);

    //set = ref4;
    //EXPECT_FALSE(~set == ref1);
    //EXPECT_FALSE(~set == ref2);
    //EXPECT_TRUE(~set == ref3);
    //EXPECT_FALSE(~set == ref4);
}

TEST_FIXTURE(CharSetTest, And)
{
    //CharSet set(Size, false);
    //CharSet ref1(Size, false);
    //CharSet ref2(Size, true);
    //CharSet ref3(Size, false);
    //ref3.Set(0, true);
    //CharSet ref4(Size, true);
    //ref4.Set(0, false);

    //EXPECT_TRUE(set == ref1);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_FALSE(set == ref4);
    //set.And(ref2);
    //EXPECT_TRUE(set == ref1);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_FALSE(set == ref4);
    //set.SetAll(true);
    //set.And(ref2);
    //EXPECT_FALSE(set == ref1);
    //EXPECT_TRUE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_FALSE(set == ref4);
    //set.And(ref4);
    //EXPECT_FALSE(set == ref1);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_TRUE(set == ref4);
    //set = ref3;
    //set.And(ref2);
    //EXPECT_FALSE(set == ref1);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_TRUE(set == ref3);
    //EXPECT_FALSE(set == ref4);
    //set.And(ref4);
    //EXPECT_TRUE(set == ref1);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_FALSE(set == ref4);
}

TEST_FIXTURE(CharSetTest, OperatorAndEquals)
{
    //CharSet set(Size, false);
    //CharSet ref1(Size, false);
    //CharSet ref2(Size, true);
    //CharSet ref3(Size, false);
    //ref3.Set(0, true);
    //CharSet ref4(Size, true);
    //ref4.Set(0, false);

    //EXPECT_TRUE(set == ref1);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_FALSE(set == ref4);
    //set &= ref2;
    //EXPECT_TRUE(set == ref1);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_FALSE(set == ref4);
    //set.SetAll(true);
    //set &= ref2;
    //EXPECT_FALSE(set == ref1);
    //EXPECT_TRUE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_FALSE(set == ref4);
    //set &= ref4;
    //EXPECT_FALSE(set == ref1);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_TRUE(set == ref4);
    //set = ref3;
    //set &= ref2;
    //EXPECT_FALSE(set == ref1);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_TRUE(set == ref3);
    //EXPECT_FALSE(set == ref4);
    //set &= ref4;
    //EXPECT_TRUE(set == ref1);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_FALSE(set == ref4);
}

TEST_FIXTURE(CharSetTest, OperatorAnd)
{
    //CharSet set(Size, false);
    //CharSet ref1(Size, false);
    //CharSet ref2(Size, true);
    //CharSet ref3(Size, false);
    //ref3.Set(0, true);
    //CharSet ref4(Size, true);
    //ref4.Set(0, false);

    //EXPECT_TRUE((set & ref2) == ref1);
    //EXPECT_FALSE((set & ref2) == ref2);
    //EXPECT_FALSE((set & ref2) == ref3);
    //EXPECT_FALSE((set & ref2) == ref4);
    //EXPECT_TRUE((set & ref4) == ref1);
    //EXPECT_FALSE((set & ref4) == ref2);
    //EXPECT_FALSE((set & ref4) == ref3);
    //EXPECT_FALSE((set & ref4) == ref4);
    //set.SetAll(true);
    //EXPECT_FALSE((set & ref2) == ref1);
    //EXPECT_TRUE((set & ref2) == ref2);
    //EXPECT_FALSE((set & ref2) == ref3);
    //EXPECT_FALSE((set & ref2) == ref4);
    //EXPECT_FALSE((set & ref4) == ref1);
    //EXPECT_FALSE((set & ref4) == ref2);
    //EXPECT_FALSE((set & ref4) == ref3);
    //EXPECT_TRUE((set & ref4) == ref4);
    //set = ref3;
    //EXPECT_FALSE((set & ref2) == ref1);
    //EXPECT_FALSE((set & ref2) == ref2);
    //EXPECT_TRUE((set & ref2) == ref3);
    //EXPECT_FALSE((set & ref2) == ref4);
    //EXPECT_TRUE((set & ref4) == ref1);
    //EXPECT_FALSE((set & ref4) == ref2);
    //EXPECT_FALSE((set & ref4) == ref3);
    //EXPECT_FALSE((set & ref4) == ref4);
}

TEST_FIXTURE(CharSetTest, Or)
{
    //CharSet set(Size, false);
    //CharSet ref1(Size, false);
    //CharSet ref2(Size, true);
    //CharSet ref3(Size, false);
    //ref3.Set(0, true);
    //CharSet ref4(Size, true);
    //ref4.Set(0, false);

    //EXPECT_TRUE(set == ref1);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_FALSE(set == ref4);
    //set.Or(ref2);
    //EXPECT_FALSE(set == ref1);
    //EXPECT_TRUE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_FALSE(set == ref4);
    //set = ref1;
    //set.Or(ref3);
    //EXPECT_FALSE(set == ref1);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_TRUE(set == ref3);
    //EXPECT_FALSE(set == ref4);
    //set.Or(ref4);
    //EXPECT_FALSE(set == ref1);
    //EXPECT_TRUE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_FALSE(set == ref4);
    //set = ref4;
    //set.Or(ref3);
    //EXPECT_FALSE(set == ref1);
    //EXPECT_TRUE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_FALSE(set == ref4);
}

TEST_FIXTURE(CharSetTest, OperatorOrEquals)
{
    //CharSet set(Size, false);
    //CharSet ref1(Size, false);
    //CharSet ref2(Size, true);
    //CharSet ref3(Size, false);
    //ref3.Set(0, true);
    //CharSet ref4(Size, true);
    //ref4.Set(0, false);

    //EXPECT_TRUE(set == ref1);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_FALSE(set == ref4);
    //set |= ref2;
    //EXPECT_FALSE(set == ref1);
    //EXPECT_TRUE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_FALSE(set == ref4);
    //set = ref1;
    //set |= ref3;
    //EXPECT_FALSE(set == ref1);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_TRUE(set == ref3);
    //EXPECT_FALSE(set == ref4);
    //set |= ref4;
    //EXPECT_FALSE(set == ref1);
    //EXPECT_TRUE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_FALSE(set == ref4);
    //set = ref4;
    //set |= ref3;
    //EXPECT_FALSE(set == ref1);
    //EXPECT_TRUE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_FALSE(set == ref4);
}

TEST_FIXTURE(CharSetTest, OperatorOr)
{
    //CharSet set(Size, false);
    //CharSet ref1(Size, false);
    //CharSet ref2(Size, true);
    //CharSet ref3(Size, false);
    //ref3.Set(0, true);
    //CharSet ref4(Size, true);
    //ref4.Set(0, false);

    //EXPECT_FALSE((set | ref2) == ref1);
    //EXPECT_TRUE((set | ref2) == ref2);
    //EXPECT_FALSE((set | ref2) == ref3);
    //EXPECT_FALSE((set | ref2) == ref4);
    //EXPECT_FALSE((set | ref4) == ref1);
    //EXPECT_FALSE((set | ref4) == ref2);
    //EXPECT_FALSE((set | ref4) == ref3);
    //EXPECT_TRUE((set | ref4) == ref4);
    //set = ref1;
    //EXPECT_FALSE((set | ref3) == ref1);
    //EXPECT_FALSE((set | ref3) == ref2);
    //EXPECT_TRUE((set | ref3) == ref3);
    //EXPECT_FALSE((set | ref3) == ref4);
    //set = ref3;
    //EXPECT_FALSE((set | ref4) == ref1);
    //EXPECT_TRUE((set | ref4) == ref2);
    //EXPECT_FALSE((set | ref4) == ref3);
    //EXPECT_FALSE((set | ref4) == ref4);
    //set = ref4;
    //EXPECT_FALSE((set | ref3) == ref1);
    //EXPECT_TRUE((set | ref3) == ref2);
    //EXPECT_FALSE((set | ref3) == ref3);
    //EXPECT_FALSE((set | ref3) == ref4);
}

TEST_FIXTURE(CharSetTest, Xor)
{
    //CharSet set(Size, false);
    //CharSet ref1(Size, false);
    //CharSet ref2(Size, true);
    //CharSet ref3(Size, false);
    //ref3.Set(0, true);
    //CharSet ref4(Size, true);
    //ref4.Set(0, false);

    //EXPECT_TRUE(set == ref1);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_FALSE(set == ref4);
    //set.Xor(ref2);
    //EXPECT_FALSE(set == ref1);
    //EXPECT_TRUE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_FALSE(set == ref4);
    //set.Xor(ref3);
    //EXPECT_FALSE(set == ref1);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_TRUE(set == ref4);
    //set.Xor(ref2);
    //EXPECT_FALSE(set == ref1);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_TRUE(set == ref3);
    //EXPECT_FALSE(set == ref4);
    //set.Xor(ref3);
    //EXPECT_TRUE(set == ref1);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_FALSE(set == ref4);
}

TEST_FIXTURE(CharSetTest, OperatorXorEquals)
{
    //CharSet set(Size, false);
    //CharSet ref1(Size, false);
    //CharSet ref2(Size, true);
    //CharSet ref3(Size, false);
    //ref3.Set(0, true);
    //CharSet ref4(Size, true);
    //ref4.Set(0, false);

    //EXPECT_TRUE(set == ref1);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_FALSE(set == ref4);
    //set ^= ref2;
    //EXPECT_FALSE(set == ref1);
    //EXPECT_TRUE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_FALSE(set == ref4);
    //set ^= ref3;
    //EXPECT_FALSE(set == ref1);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_TRUE(set == ref4);
    //set ^= ref2;
    //EXPECT_FALSE(set == ref1);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_TRUE(set == ref3);
    //EXPECT_FALSE(set == ref4);
    //set ^= ref3;
    //EXPECT_TRUE(set == ref1);
    //EXPECT_FALSE(set == ref2);
    //EXPECT_FALSE(set == ref3);
    //EXPECT_FALSE(set == ref4);
}

TEST_FIXTURE(CharSetTest, OperatorXor)
{
    //CharSet set(Size, false);
    //CharSet ref1(Size, false);
    //CharSet ref2(Size, true);
    //CharSet ref3(Size, false);
    //ref3.Set(0, true);
    //CharSet ref4(Size, true);
    //ref4.Set(0, false);

    //EXPECT_FALSE((set ^ ref2) == ref1);
    //EXPECT_TRUE((set ^ ref2) == ref2);
    //EXPECT_FALSE((set ^ ref2) == ref3);
    //EXPECT_FALSE((set ^ ref2) == ref4);
    //EXPECT_FALSE((set ^ ref4) == ref1);
    //EXPECT_FALSE((set ^ ref4) == ref2);
    //EXPECT_FALSE((set ^ ref4) == ref3);
    //EXPECT_TRUE((set ^ ref4) == ref4);
    //set = ref2;
    //EXPECT_FALSE((set ^ ref3) == ref1);
    //EXPECT_FALSE((set ^ ref3) == ref2);
    //EXPECT_FALSE((set ^ ref3) == ref3);
    //EXPECT_TRUE((set ^ ref3) == ref4);
    //set = ref4;
    //EXPECT_FALSE((set ^ ref2) == ref1);
    //EXPECT_FALSE((set ^ ref2) == ref2);
    //EXPECT_TRUE((set ^ ref2) == ref3);
    //EXPECT_FALSE((set ^ ref2) == ref4);
    //set = ref3;
    //EXPECT_TRUE((set ^ ref3) == ref1);
    //EXPECT_FALSE((set ^ ref3) == ref2);
    //EXPECT_FALSE((set ^ ref3) == ref3);
    //EXPECT_FALSE((set ^ ref3) == ref4);
}

} // namespace Test

} // namespace Simulate
