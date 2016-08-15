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

static std::vector<wchar_t> chars = { 0, 1, 2, 3, 4, 5 };

TEST_FIXTURE(CharSetTest, ConstructDefault)
{
    CharSet set;
    EXPECT_EQ(size_t{ 0 }, set.Count());
    EXPECT_TRUE(set.begin() == set.end());
}

TEST_FIXTURE(CharSetTest, ConstructCopy)
{
    CharSet set;
    for (auto ch : chars)
    {
        set.Set(ch);
    }
    CharSet other(set);
    EXPECT_EQ(chars.size(), set.Count());
    EXPECT_EQ(chars.size(), other.Count());
    for (auto ch : chars)
    {
        EXPECT_TRUE(set.Get(ch));
        EXPECT_TRUE(other.Get(ch));
    }
    EXPECT_FALSE(set.begin() == set.end());
    EXPECT_FALSE(other.begin() == other.end());
}

TEST_FIXTURE(CharSetTest, ConstructMove)
{
    CharSet set;
    for (auto ch : chars)
    {
        set.Set(ch);
    }
    CharSet other(std::move(set));
    EXPECT_EQ(0, set.Count());
    EXPECT_EQ(chars.size(), other.Count());
    for (auto ch : chars)
    {
        EXPECT_FALSE(set.Get(ch));
        EXPECT_TRUE(other.Get(ch));
    }
    EXPECT_TRUE(set.begin() == set.end());
    EXPECT_FALSE(other.begin() == other.end());
}

TEST_FIXTURE(CharSetTest, AssignCopy)
{
    CharSet set;
    for (auto ch : chars)
    {
        set.Set(ch);
    }
    CharSet other;
    other = set;
    EXPECT_EQ(chars.size(), set.Count());
    EXPECT_EQ(chars.size(), other.Count());
    for (auto ch : chars)
    {
        EXPECT_TRUE(set.Get(ch));
        EXPECT_TRUE(other.Get(ch));
    }
}

TEST_FIXTURE(CharSetTest, AssignMove)
{
    CharSet set;
    for (auto ch : chars)
    {
        set.Set(ch);
    }
    CharSet other;
    other = std::move(set);
    EXPECT_EQ(0, set.Count());
    EXPECT_EQ(chars.size(), other.Count());
    for (auto ch : chars)
    {
        EXPECT_FALSE(set.Get(ch));
        EXPECT_TRUE(other.Get(ch));
    }
}

TEST_FIXTURE(CharSetTest, Clone)
{
    CharSet set;
    for (auto ch : chars)
    {
        set.Set(ch);
    }
    CharSet other = set.Clone();
    EXPECT_EQ(chars.size(), set.Count());
    EXPECT_EQ(chars.size(), other.Count());
    for (auto ch : chars)
    {
        EXPECT_TRUE(set.Get(ch));
        EXPECT_TRUE(other.Get(ch));
    }
}

TEST_FIXTURE(CharSetTest, GetSet)
{

    CharSet set;
    for (auto ch : chars)
        EXPECT_EQ(false, set.Get(ch));
    for (size_t i = 0; i < chars.size(); ++i)
    {
        set.Set(chars[i]);
        for (size_t j = 0; j < chars.size(); ++j)
            EXPECT_EQ(j <= i, set.Get(chars[j]));
    }
    for (auto ch : chars)
        EXPECT_EQ(true, set.Get(ch));
}

TEST_FIXTURE(CharSetTest, OperatorEqual)
{
    CharSet set;
    CharSet ref1;
    CharSet ref2;
    for (size_t i = 0; i < chars.size(); ++i)
    {
        ref1.Set(chars[i]);
        if (i != 0)
        {
            set.Set(chars[i]);
            ref2.Set(chars[i]);
        }
    }

    EXPECT_TRUE(set == set);
    EXPECT_FALSE(set == ref1);
    EXPECT_FALSE(ref1 == set);
    EXPECT_TRUE(set == ref2);
    EXPECT_TRUE(ref2 == set);
    EXPECT_FALSE(ref1 == ref2);
    EXPECT_FALSE(ref2 == ref1);
    set.Set(0);
    EXPECT_TRUE(set == set);
    EXPECT_TRUE(set == ref1);
    EXPECT_TRUE(ref1 == set);
    EXPECT_FALSE(set == ref2);
    EXPECT_FALSE(ref2 == set);
    EXPECT_FALSE(ref1 == ref2);
    EXPECT_FALSE(ref2 == ref1);
}

TEST_FIXTURE(CharSetTest, OperatorInequal)
{
    CharSet set;
    CharSet ref1;
    CharSet ref2;
    for (size_t i = 0; i < chars.size(); ++i)
    {
        ref1.Set(chars[i]);
        if (i != 0)
        {
            set.Set(chars[i]);
            ref2.Set(chars[i]);
        }
    }

    EXPECT_FALSE(set != set);
    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(ref1 != set);
    EXPECT_FALSE(set != ref2);
    EXPECT_FALSE(ref2 != set);
    EXPECT_TRUE(ref1 != ref2);
    EXPECT_TRUE(ref2 != ref1);
    set.Set(0);
    EXPECT_FALSE(set != set);
    EXPECT_FALSE(set != ref1);
    EXPECT_FALSE(ref1 != set);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(ref2 != set);
    EXPECT_TRUE(ref1 != ref2);
    EXPECT_TRUE(ref2 != ref1);
}

TEST_FIXTURE(CharSetTest, Overlaps)
{
    CharSet set;
    CharSet ref1;
    CharSet ref2;
    set.Set(chars[0]);
    ref1.Set(chars[0]);
    ref1.Set(chars[1]);
    ref1.Set(chars[2]);
    ref2.Set(chars[1]);

    EXPECT_TRUE(set.Overlaps(set));
    EXPECT_TRUE(set.Overlaps(ref1));
    EXPECT_TRUE(ref1.Overlaps(set));
    EXPECT_FALSE(set.Overlaps(ref2));
    EXPECT_FALSE(ref2.Overlaps(set));
    EXPECT_TRUE(ref1.Overlaps(ref2));
    EXPECT_TRUE(ref2.Overlaps(ref1));
}

TEST_FIXTURE(CharSetTest, Includes)
{
    CharSet set;
    CharSet ref1;
    CharSet ref2;
    set.Set(chars[0]);
    ref1.Set(chars[0]);
    ref1.Set(chars[1]);
    ref1.Set(chars[2]);
    ref2.Set(chars[1]);

    EXPECT_TRUE(set.Includes(set));
    EXPECT_FALSE(set.Includes(ref1));
    EXPECT_TRUE(ref1.Includes(set));
    EXPECT_FALSE(set.Includes(ref2));
    EXPECT_FALSE(ref2.Includes(set));
    EXPECT_TRUE(ref1.Includes(ref2));
    EXPECT_FALSE(ref2.Includes(ref1));
}

TEST_FIXTURE(CharSetTest, OperatorAndEquals)
{
    CharSet set;
    CharSet ref1;
    CharSet ref2;
    CharSet ref3;
    CharSet ref4;
    set.Set(chars[0]);
    set.Set(chars[1]);
    set.Set(chars[2]);
    ref1.Set(chars[0]);
    ref2.Set(chars[1]);
    ref3 = set;

    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set == ref3);
    EXPECT_TRUE(set != ref4);
    set &= ref1;
    EXPECT_TRUE(set == ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set != ref4);
    set = ref3;
    set &= ref2;
    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set == ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set != ref4);
    set &= ref1;
    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set == ref4);
    set = ref3;
    set &= ref3;
    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set == ref3);
    EXPECT_TRUE(set != ref4);
    set &= ref4;
    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set == ref4);
}

TEST_FIXTURE(CharSetTest, OperatorAnd)
{
    CharSet set;
    CharSet ref1;
    CharSet ref2;
    CharSet ref3;
    CharSet ref4;
    set.Set(chars[0]);
    set.Set(chars[1]);
    set.Set(chars[2]);
    ref1.Set(chars[0]);
    ref2.Set(chars[1]);
    ref3 = set;

    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set == ref3);
    EXPECT_TRUE(set != ref4);
    EXPECT_TRUE((set & ref1) == ref1);
    EXPECT_TRUE((set & ref1) != ref2);
    EXPECT_TRUE((set & ref1) != ref3);
    EXPECT_TRUE((set & ref1) != ref4);
    EXPECT_TRUE((set & ref2) != ref1);
    EXPECT_TRUE((set & ref2) == ref2);
    EXPECT_TRUE((set & ref2) != ref3);
    EXPECT_TRUE((set & ref2) != ref4);
    set = ref2;
    EXPECT_TRUE((set & ref1) != ref1);
    EXPECT_TRUE((set & ref1) != ref2);
    EXPECT_TRUE((set & ref1) != ref3);
    EXPECT_TRUE((set & ref1) == ref4);
    set = ref3;
    EXPECT_TRUE((set & ref3) != ref1);
    EXPECT_TRUE((set & ref3) != ref2);
    EXPECT_TRUE((set & ref3) == ref3);
    EXPECT_TRUE((set & ref3) != ref4);
    set = ref3;
    EXPECT_TRUE((set & ref4) != ref1);
    EXPECT_TRUE((set & ref4) != ref2);
    EXPECT_TRUE((set & ref4) != ref3);
    EXPECT_TRUE((set & ref4) == ref4);
}

TEST_FIXTURE(CharSetTest, OperatorOrEquals)
{
    CharSet set;
    CharSet ref1;
    CharSet ref2;
    CharSet ref3;
    CharSet ref4;
    ref1.Set(chars[0]);
    ref2.Set(chars[1]);
    ref2.Set(chars[2]);
    ref3.Set(chars[0]);
    ref3.Set(chars[1]);
    ref3.Set(chars[2]);

    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set == ref4);
    set |= ref1;
    EXPECT_TRUE(set == ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set != ref4);
    set = ref4;
    set |= ref2;
    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set == ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set != ref4);
    set |= ref1;
    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set == ref3);
    EXPECT_TRUE(set != ref4);
    set = ref4;
    set |= ref3;
    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set == ref3);
    EXPECT_TRUE(set != ref4);
    set = ref4;
    set |= ref4;
    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set == ref4);
}

TEST_FIXTURE(CharSetTest, OperatorOr)
{
    CharSet set;
    CharSet ref1;
    CharSet ref2;
    CharSet ref3;
    CharSet ref4;
    ref1.Set(chars[0]);
    ref2.Set(chars[1]);
    ref2.Set(chars[2]);
    ref3.Set(chars[0]);
    ref3.Set(chars[1]);
    ref3.Set(chars[2]);

    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set == ref4);
    EXPECT_TRUE((set | ref1) == ref1);
    EXPECT_TRUE((set | ref1) != ref2);
    EXPECT_TRUE((set | ref1) != ref3);
    EXPECT_TRUE((set | ref1) != ref4);
    EXPECT_TRUE((set | ref2) != ref1);
    EXPECT_TRUE((set | ref2) == ref2);
    EXPECT_TRUE((set | ref2) != ref3);
    EXPECT_TRUE((set | ref2) != ref4);
    set = ref2;
    EXPECT_TRUE((set | ref1) != ref1);
    EXPECT_TRUE((set | ref1) != ref2);
    EXPECT_TRUE((set | ref1) == ref3);
    EXPECT_TRUE((set | ref1) != ref4);
    set = ref4;
    EXPECT_TRUE((set | ref3) != ref1);
    EXPECT_TRUE((set | ref3) != ref2);
    EXPECT_TRUE((set | ref3) == ref3);
    EXPECT_TRUE((set | ref3) != ref4);
    EXPECT_TRUE((set | ref4) != ref1);
    EXPECT_TRUE((set | ref4) != ref2);
    EXPECT_TRUE((set | ref4) != ref3);
    EXPECT_TRUE((set | ref4) == ref4);
}

TEST_FIXTURE(CharSetTest, Clear)
{
    CharSet set;
    for (auto ch : chars)
    {
        set.Set(ch);
    }
    EXPECT_EQ(chars.size(), set.Count());
    for (auto ch : chars)
    {
        EXPECT_TRUE(set.Get(ch));
    }
    set.Clear();
    EXPECT_EQ(size_t{ 0 }, set.Count());
    for (auto ch : chars)
    {
        EXPECT_FALSE(set.Get(ch));
    }
}

TEST_FIXTURE(CharSetTest, Fill)
{
    CharSet set;
    set.Fill();
    // We should officially test for all characters, but that will take until eternity
    for (auto ch : chars)
    {
        EXPECT_TRUE(set.Get(ch));
    }
    //for (wchar_t ch = 0; ch < 65536; ++ch)
    //{
    //    EXPECT_TRUE(set.Get(ch));
    //}
}

TEST_FIXTURE(CharSetTest, OperatorMinusEquals)
{
    CharSet set;
    CharSet ref1;
    CharSet ref2;
    CharSet ref3;
    CharSet ref4;
    ref1.Set(chars[0]);
    ref2.Set(chars[1]);
    ref2.Set(chars[2]);
    ref3.Set(chars[0]);
    ref3.Set(chars[1]);
    ref3.Set(chars[2]);

    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set == ref4);
    set = ref3;
    set -= ref1;
    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set == ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set != ref4);
    set -= ref2;
    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set == ref4);
    set = ref3;
    set -= ref2;
    EXPECT_TRUE(set == ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set != ref4);
    set = ref3;
    set -= ref4;
    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set == ref3);
    EXPECT_TRUE(set != ref4);
}

TEST_FIXTURE(CharSetTest, OperatorMinus)
{
    CharSet set;
    CharSet ref1;
    CharSet ref2;
    CharSet ref3;
    CharSet ref4;
    ref1.Set(chars[0]);
    ref2.Set(chars[1]);
    ref2.Set(chars[2]);
    ref3.Set(chars[0]);
    ref3.Set(chars[1]);
    ref3.Set(chars[2]);

    EXPECT_TRUE(set != ref1);
    EXPECT_TRUE(set != ref2);
    EXPECT_TRUE(set != ref3);
    EXPECT_TRUE(set == ref4);
    set = ref3;
    EXPECT_TRUE((set - ref1) != ref1);
    EXPECT_TRUE((set - ref1) == ref2);
    EXPECT_TRUE((set - ref1) != ref3);
    EXPECT_TRUE((set - ref1) != ref4);
    set = ref2;
    EXPECT_TRUE((set - ref2) != ref1);
    EXPECT_TRUE((set - ref2) != ref2);
    EXPECT_TRUE((set - ref2) != ref3);
    EXPECT_TRUE((set - ref2) == ref4);
    set = ref3;
    EXPECT_TRUE((set - ref2) == ref1);
    EXPECT_TRUE((set - ref2) != ref2);
    EXPECT_TRUE((set - ref2) != ref3);
    EXPECT_TRUE((set - ref2) != ref4);
    set = ref3;
    EXPECT_TRUE((set - ref4) != ref1);
    EXPECT_TRUE((set - ref4) != ref2);
    EXPECT_TRUE((set - ref4) == ref3);
    EXPECT_TRUE((set - ref4) != ref4);
}

TEST_FIXTURE(CharSetTest, BeginEnd)
{
    CharSet set;
    for (auto ch : chars)
    {
        set.Set(ch);
    }
    EXPECT_FALSE(set.begin() == set.end());
    CharSet::ConstIterator it = set.begin();
    EXPECT_TRUE(it == set.begin());
    EXPECT_FALSE(it == set.end());
    EXPECT_EQ(chars[0], it->from);
    EXPECT_EQ(chars[chars.size() - 1], it->to);
    ++it;
    EXPECT_FALSE(it == set.begin());
    EXPECT_TRUE(it == set.end());
}

} // namespace Test

} // namespace Simulate
