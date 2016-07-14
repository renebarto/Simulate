#include "unit-test-c++/UnitTestC++.h"
#include "core/TimeSpan.h"
#include "core/Util.h"

using namespace std;

namespace Core
{

namespace Test
{

class TimeSpanTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void TimeSpanTest::SetUp()
{
}

void TimeSpanTest::TearDown()
{
}

TEST_FIXTURE(TimeSpanTest, Construction)
{
    TimeSpan timeSpan;

    EXPECT_EQ(0, timeSpan.NanoSeconds());
    EXPECT_EQ(0, timeSpan.MicroSeconds());
    EXPECT_EQ(0, timeSpan.MilliSeconds());
    EXPECT_EQ(0, timeSpan.Seconds());
    EXPECT_EQ(0, timeSpan.Minutes());
    EXPECT_EQ(0, timeSpan.Hours());
    EXPECT_EQ(0, timeSpan.Days());
    EXPECT_EQ(0, (int64_t)timeSpan);
}

TEST_FIXTURE(TimeSpanTest, ConstructionInterval)
{
    int64_t interval = 111234567890123;
    TimeSpan timeSpan(interval);

    EXPECT_EQ(interval, timeSpan.NanoSeconds());
    EXPECT_EQ(interval / 1000, timeSpan.MicroSeconds());
    EXPECT_EQ(interval / 1000000, timeSpan.MilliSeconds());
    EXPECT_EQ((double)interval / 1000000000, timeSpan.Seconds());
    EXPECT_EQ(interval / 60000000000ll, timeSpan.Minutes());
    EXPECT_EQ(interval / 3600000000000ll, timeSpan.Hours());
    EXPECT_EQ(interval / 86400000000000ll, timeSpan.Days());
    EXPECT_EQ(interval, (int64_t)timeSpan);
}

TEST_FIXTURE(TimeSpanTest, ConstructionCopy)
{
    int64_t interval = 111234567890123;
    TimeSpan timeSpan(interval);
    TimeSpan timeSpan2(timeSpan);

    EXPECT_EQ(interval, timeSpan2.NanoSeconds());
    EXPECT_EQ(interval / 1000, timeSpan2.MicroSeconds());
    EXPECT_EQ(interval / 1000000, timeSpan2.MilliSeconds());
    EXPECT_EQ((double)interval / 1000000000, timeSpan2.Seconds());
    EXPECT_EQ(interval / 60000000000ll, timeSpan2.Minutes());
    EXPECT_EQ(interval / 3600000000000ll, timeSpan2.Hours());
    EXPECT_EQ(interval / 86400000000000ll, timeSpan2.Days());
    EXPECT_EQ(interval, (int64_t)timeSpan2);
}

TEST_FIXTURE(TimeSpanTest, AssignInterval)
{
    int64_t interval = 111234567890123;
    TimeSpan timeSpan;

    timeSpan = interval;

    EXPECT_EQ(interval, timeSpan.NanoSeconds());
    EXPECT_EQ(interval / 1000, timeSpan.MicroSeconds());
    EXPECT_EQ(interval / 1000000, timeSpan.MilliSeconds());
    EXPECT_EQ((double)interval / 1000000000, timeSpan.Seconds());
    EXPECT_EQ(interval / 60000000000ll, timeSpan.Minutes());
    EXPECT_EQ(interval / 3600000000000ll, timeSpan.Hours());
    EXPECT_EQ(interval / 86400000000000ll, timeSpan.Days());
    EXPECT_EQ(interval, (int64_t)timeSpan);
}

TEST_FIXTURE(TimeSpanTest, AssignTimeSpan)
{
    int64_t interval = 111234567890123;
    TimeSpan timeSpan(interval);
    TimeSpan timeSpan2;

    timeSpan2 = timeSpan;

    EXPECT_EQ(interval, timeSpan2.NanoSeconds());
    EXPECT_EQ(interval / 1000, timeSpan2.MicroSeconds());
    EXPECT_EQ(interval / 1000000, timeSpan2.MilliSeconds());
    EXPECT_EQ((double)interval / 1000000000, timeSpan2.Seconds());
    EXPECT_EQ(interval / 60000000000ll, timeSpan2.Minutes());
    EXPECT_EQ(interval / 3600000000000ll, timeSpan2.Hours());
    EXPECT_EQ(interval / 86400000000000ll, timeSpan2.Days());
    EXPECT_EQ(interval, (int64_t)timeSpan);
}

TEST_FIXTURE(TimeSpanTest, OperatorEquals)
{
    int64_t interval1 = 1112345678901234ll;
    int64_t interval2 = 1112345678910123ll;
    int64_t interval3 = 1234567890123456ll;
    TimeSpan timeSpan1(interval1);
    TimeSpan timeSpan2(interval2);
    TimeSpan timeSpan3(interval3);
    TimeSpan timeSpan4(interval1);

    EXPECT_TRUE(timeSpan1 == timeSpan4);
    EXPECT_TRUE(timeSpan4 == timeSpan1);
    EXPECT_FALSE(timeSpan1 == timeSpan2);
    EXPECT_FALSE(timeSpan1 == timeSpan3);

    EXPECT_TRUE(timeSpan1 == interval1);
    EXPECT_FALSE(timeSpan1 == interval2);
    EXPECT_FALSE(timeSpan1 == interval3);
    EXPECT_TRUE(timeSpan3 == interval3);

    EXPECT_TRUE(interval1 == timeSpan1);
    EXPECT_FALSE(interval2 == timeSpan1);
    EXPECT_FALSE(interval3 == timeSpan1);
    EXPECT_TRUE(interval3 == timeSpan3);

    EXPECT_EQ(timeSpan1, timeSpan4);
    EXPECT_EQ(timeSpan4, timeSpan1);
    EXPECT_NE(timeSpan1, timeSpan2);
    EXPECT_NE(timeSpan1, timeSpan3);
    EXPECT_EQ(timeSpan1, timeSpan1);

    EXPECT_EQ(timeSpan1, interval1);
    EXPECT_NE(timeSpan1, interval2);
    EXPECT_NE(timeSpan1, interval3);
    EXPECT_EQ(timeSpan3, interval3);

    EXPECT_EQ(interval1, timeSpan1);
    EXPECT_NE(interval2, timeSpan1);
    EXPECT_NE(interval3, timeSpan1);
    EXPECT_EQ(interval3, timeSpan3);
}

TEST_FIXTURE(TimeSpanTest, OperatorNotEquals)
{
    int64_t interval1 = 1112345678901234ll;
    int64_t interval2 = 1112345678910123ll;
    int64_t interval3 = 1234567890123456ll;
    TimeSpan timeSpan1(interval1);
    TimeSpan timeSpan2(interval2);
    TimeSpan timeSpan3(interval3);
    TimeSpan timeSpan4(interval1);

    EXPECT_FALSE(timeSpan1 != timeSpan4);
    EXPECT_FALSE(timeSpan4 != timeSpan1);
    EXPECT_TRUE(timeSpan1 != timeSpan2);
    EXPECT_TRUE(timeSpan1 != timeSpan3);

    EXPECT_FALSE(timeSpan1 != interval1);
    EXPECT_TRUE(timeSpan1 != interval2);
    EXPECT_TRUE(timeSpan1 != interval3);
    EXPECT_FALSE(timeSpan3 != interval3);

    EXPECT_FALSE(interval1 != timeSpan1);
    EXPECT_TRUE(interval2 != timeSpan1);
    EXPECT_TRUE(interval3 != timeSpan1);
    EXPECT_FALSE(interval3 != timeSpan3);
}

TEST_FIXTURE(TimeSpanTest, PrintTo)
{
    int64_t interval = 111234567890123;
    TimeSpan timeSpan(interval);
    int days = timeSpan.Days();
    int hours = timeSpan.Hours() % 24;
    int minutes = timeSpan.Minutes() % 60;
    int seconds = (int)timeSpan.Seconds() % 60;
    int microSeconds = timeSpan.MicroSeconds() % 1000000;
    ostringstream streamExpected;
    streamExpected << days << " days, " << hours << " hours, "
                   << minutes << " minutes, "
                   << seconds << "." << setfill('0') << setw(6) << microSeconds << " seconds";
    string expected = streamExpected.str();
    ostringstream streamActual;
    PrintTo(timeSpan, streamActual);
    string actual = streamActual.str();
    EXPECT_EQ(expected, actual);
}

} // namespace Test

} // namespace Core
