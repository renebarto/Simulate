#include "unit-test-c++/UnitTestC++.h"
#include "core/DateTime.h"
#include "core/Exception.h"
#include "core/TimeSpan.h"

using namespace std;

namespace Core
{

namespace Test
{

class DateTimeTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void DateTimeTest::SetUp()
{
}

void DateTimeTest::TearDown()
{
}

TEST_FIXTURE(DateTimeTest, Construction)
{
    DateTime dateTime;
    EXPECT_EQ(1970, dateTime.Year());
    EXPECT_EQ(1, dateTime.Month());
    EXPECT_EQ(1, dateTime.MonthDay());
    EXPECT_EQ(MonthType::January, dateTime.MonthName());
    EXPECT_EQ(WeekDayType::Thursday, dateTime.WeekDay());
    EXPECT_EQ(1, dateTime.YearDay());
    EXPECT_EQ(0, dateTime.Hour());
    EXPECT_EQ(0, dateTime.Minute());
    EXPECT_EQ(0, dateTime.Second());
    EXPECT_EQ(0, dateTime.MicroSeconds());
    EXPECT_EQ(1970, dateTime.WeekNumberingYear());
    EXPECT_EQ(1, dateTime.WeekOfYear());
}

TEST_FIXTURE(DateTimeTest, ConstructionCopy)
{
    int year = 2014;
    int month = 2;
    int day = 26;
    int yearDay = 57;
    MonthType monthName = MonthType::February;
    int hour = 1;
    int minute = 2;
    int second = 3;
    int microSeconds = 4;
    int weekNumberingYear = 2014;
    int weekOfYear = 9;
    DateTime dateTime(year, month, day, hour, minute, second, microSeconds);
    DateTime dateTime2(dateTime);
    EXPECT_EQ(year, dateTime2.Year());
    EXPECT_EQ(month, dateTime2.Month());
    EXPECT_EQ(day, dateTime2.MonthDay());
    EXPECT_EQ(monthName, dateTime2.MonthName());
    EXPECT_EQ(WeekDayType::Wednesday, dateTime2.WeekDay());
    EXPECT_EQ(yearDay, dateTime2.YearDay());
    EXPECT_EQ(hour, dateTime2.Hour());
    EXPECT_EQ(minute, dateTime2.Minute());
    EXPECT_EQ(second, dateTime2.Second());
    EXPECT_EQ(microSeconds, dateTime2.MicroSeconds());
    EXPECT_EQ(weekNumberingYear, dateTime2.WeekNumberingYear());
    EXPECT_EQ(weekOfYear, dateTime2.WeekOfYear());
}

TEST_FIXTURE(DateTimeTest, ConstructionYearMonthDayHourMinuteSecondMicroSecond)
{
    int year = 2014;
    int month = 2;
    int day = 26;
    int yearDay = 57;
    MonthType monthName = MonthType::February;
    WeekDayType weekDay = WeekDayType::Wednesday;
    int hour = 1;
    int minute = 2;
    int second = 3;
    int microSeconds = 4;
    int weekNumberingYear = 2014;
    int weekOfYear = 9;
    DateTime dateTime(year, month, day, hour, minute, second, microSeconds);
    EXPECT_EQ(year, dateTime.Year());
    EXPECT_EQ(month, dateTime.Month());
    EXPECT_EQ(day, dateTime.MonthDay());
    EXPECT_EQ(monthName, dateTime.MonthName());
    EXPECT_EQ(weekDay, dateTime.WeekDay());
    EXPECT_EQ(yearDay, dateTime.YearDay());
    EXPECT_EQ(hour, dateTime.Hour());
    EXPECT_EQ(minute, dateTime.Minute());
    EXPECT_EQ(second, dateTime.Second());
    EXPECT_EQ(microSeconds, dateTime.MicroSeconds());
    EXPECT_EQ(weekNumberingYear, dateTime.WeekNumberingYear());
    EXPECT_EQ(weekOfYear, dateTime.WeekOfYear());
}

TEST_FIXTURE(DateTimeTest, ConstructionYearMonthDayHourMinuteSecondMicroSecondEndOfYear)
{
    int year = 2013;
    int month = 12;
    int day = 31;
    int yearDay = 365;
    MonthType monthName = MonthType::December;
    WeekDayType weekDay = WeekDayType::Tuesday;
    int hour = 1;
    int minute = 2;
    int second = 3;
    int microSeconds = 4;
    int weekNumberingYear = 2014;
    int weekOfYear = 1;
    DateTime dateTime(year, month, day, hour, minute, second, microSeconds);
    EXPECT_EQ(year, dateTime.Year());
    EXPECT_EQ(month, dateTime.Month());
    EXPECT_EQ(day, dateTime.MonthDay());
    EXPECT_EQ(monthName, dateTime.MonthName());
    EXPECT_EQ(weekDay, dateTime.WeekDay());
    EXPECT_EQ(yearDay, dateTime.YearDay());
    EXPECT_EQ(hour, dateTime.Hour());
    EXPECT_EQ(minute, dateTime.Minute());
    EXPECT_EQ(second, dateTime.Second());
    EXPECT_EQ(microSeconds, dateTime.MicroSeconds());
    EXPECT_EQ(weekNumberingYear, dateTime.WeekNumberingYear());
    EXPECT_EQ(weekOfYear, dateTime.WeekOfYear());
}

TEST_FIXTURE(DateTimeTest, ConstructionYearMonthDayHourMinuteSecond)
{
    int year = 2014;
    int month = 2;
    int day = 26;
    int yearDay = 57;
    MonthType monthName = MonthType::February;
    WeekDayType weekDay = WeekDayType::Wednesday;
    int hour = 1;
    int minute = 2;
    int second = 3;
    int microSeconds = 0;
    DateTime dateTime(year, month, day, hour, minute, second);
    EXPECT_EQ(year, dateTime.Year());
    EXPECT_EQ(month, dateTime.Month());
    EXPECT_EQ(day, dateTime.MonthDay());
    EXPECT_EQ(monthName, dateTime.MonthName());
    EXPECT_EQ(weekDay, dateTime.WeekDay());
    EXPECT_EQ(yearDay, dateTime.YearDay());
    EXPECT_EQ(hour, dateTime.Hour());
    EXPECT_EQ(minute, dateTime.Minute());
    EXPECT_EQ(second, dateTime.Second());
    EXPECT_EQ(microSeconds, dateTime.MicroSeconds());
}

TEST_FIXTURE(DateTimeTest, ConstructionYearMonthNameDayHourMinuteSecondMicroSecond)
{
    int year = 2014;
    int month = 2;
    int day = 26;
    int yearDay = 57;
    MonthType monthName = MonthType::February;
    WeekDayType weekDay = WeekDayType::Wednesday;
    int hour = 1;
    int minute = 2;
    int second = 3;
    int microSeconds = 4;
    DateTime dateTime(year, monthName, day, hour, minute, second, microSeconds);
    EXPECT_EQ(year, dateTime.Year());
    EXPECT_EQ(month, dateTime.Month());
    EXPECT_EQ(day, dateTime.MonthDay());
    EXPECT_EQ(monthName, dateTime.MonthName());
    EXPECT_EQ(weekDay, dateTime.WeekDay());
    EXPECT_EQ(yearDay, dateTime.YearDay());
    EXPECT_EQ(hour, dateTime.Hour());
    EXPECT_EQ(minute, dateTime.Minute());
    EXPECT_EQ(second, dateTime.Second());
    EXPECT_EQ(microSeconds, dateTime.MicroSeconds());
}

TEST_FIXTURE(DateTimeTest, ConstructionYearMonthNameDayHourMinuteSecond)
{
    int year = 2014;
    int month = 2;
    int day = 26;
    int yearDay = 57;
    MonthType monthName = MonthType::February;
    WeekDayType weekDay = WeekDayType::Wednesday;
    int hour = 1;
    int minute = 2;
    int second = 3;
    int microSeconds = 0;
    DateTime dateTime(year, monthName, day, hour, minute, second);
    EXPECT_EQ(year, dateTime.Year());
    EXPECT_EQ(month, dateTime.Month());
    EXPECT_EQ(day, dateTime.MonthDay());
    EXPECT_EQ(monthName, dateTime.MonthName());
    EXPECT_EQ(weekDay, dateTime.WeekDay());
    EXPECT_EQ(yearDay, dateTime.YearDay());
    EXPECT_EQ(hour, dateTime.Hour());
    EXPECT_EQ(minute, dateTime.Minute());
    EXPECT_EQ(second, dateTime.Second());
    EXPECT_EQ(microSeconds, dateTime.MicroSeconds());
}

TEST_FIXTURE(DateTimeTest, ConstructionEpoch)
{
    int year = 2014;
    int month = 2;
    int day = 26;
    int yearDay = 57;
    int hour = 0; // UTC
    int minute = 2;
    int second = 3;
    int microSeconds = 0;
    MonthType monthName = MonthType::February;
    WeekDayType weekDay = WeekDayType::Wednesday;
    time_t epochTime = 1393372923;
    DateTime dateTime(epochTime);
    EXPECT_EQ(year, dateTime.Year());
    EXPECT_EQ(month, dateTime.Month());
    EXPECT_EQ(day, dateTime.MonthDay());
    EXPECT_EQ(monthName, dateTime.MonthName());
    EXPECT_EQ(weekDay, dateTime.WeekDay());
    EXPECT_EQ(yearDay, dateTime.YearDay());
    EXPECT_EQ(hour, dateTime.Hour());
    EXPECT_EQ(minute, dateTime.Minute());
    EXPECT_EQ(second, dateTime.Second());
    EXPECT_EQ(microSeconds, dateTime.MicroSeconds());
}

TEST_FIXTURE(DateTimeTest, ConstructionTimeSpec)
{
    int year = 2014;
    int month = 2;
    int day = 26;
    int yearDay = 57;
    int hour = 0; // UTC
    int minute = 2;
    int second = 3;
    int microSeconds = 4;
    MonthType monthName = MonthType::February;
    WeekDayType weekDay = WeekDayType::Wednesday;
    timespec timeSpec = { 1393372923, 4000 };
    DateTime dateTime(timeSpec);
    EXPECT_EQ(year, dateTime.Year());
    EXPECT_EQ(month, dateTime.Month());
    EXPECT_EQ(day, dateTime.MonthDay());
    EXPECT_EQ(monthName, dateTime.MonthName());
    EXPECT_EQ(weekDay, dateTime.WeekDay());
    EXPECT_EQ(yearDay, dateTime.YearDay());
    EXPECT_EQ(hour, dateTime.Hour());
    EXPECT_EQ(minute, dateTime.Minute());
    EXPECT_EQ(second, dateTime.Second());
    EXPECT_EQ(microSeconds, dateTime.MicroSeconds());
}

TEST_FIXTURE(DateTimeTest, ConstructionTimeVal)
{
    int year = 2014;
    int month = 2;
    int day = 26;
    int yearDay = 57;
    int hour = 0; // UTC
    int minute = 2;
    int second = 3;
    int microSeconds = 4;
    MonthType monthName = MonthType::February;
    WeekDayType weekDay = WeekDayType::Wednesday;
    timeval timeVal = { 1393372923, 4 };
    DateTime dateTime(timeVal);
    EXPECT_EQ(year, dateTime.Year());
    EXPECT_EQ(month, dateTime.Month());
    EXPECT_EQ(day, dateTime.MonthDay());
    EXPECT_EQ(monthName, dateTime.MonthName());
    EXPECT_EQ(weekDay, dateTime.WeekDay());
    EXPECT_EQ(yearDay, dateTime.YearDay());
    EXPECT_EQ(hour, dateTime.Hour());
    EXPECT_EQ(minute, dateTime.Minute());
    EXPECT_EQ(second, dateTime.Second());
    EXPECT_EQ(microSeconds, dateTime.MicroSeconds());
}

TEST_FIXTURE(DateTimeTest, Assignment)
{
    int year = 2014;
    int month = 2;
    int day = 26;
    int hour = 1;
    int minute = 2;
    int second = 3;
    int microSeconds = 4;
    DateTime dateTime(year, month, day, hour, minute, second, microSeconds);
    DateTime dateTime2;
    dateTime2 = dateTime;
    EXPECT_EQ(year, dateTime2.Year());
    EXPECT_EQ(month, dateTime2.Month());
    EXPECT_EQ(day, dateTime2.MonthDay());
    EXPECT_EQ(hour, dateTime2.Hour());
    EXPECT_EQ(minute, dateTime2.Minute());
    EXPECT_EQ(second, dateTime2.Second());
    EXPECT_EQ(microSeconds, dateTime2.MicroSeconds());
}

TEST_FIXTURE(DateTimeTest, AssignmentEpoch)
{
    int yearExpected = 2014;
    int monthExpected = 2;
    int dayExpected = 26;
    int hourExpected = 0; // UTC
    int minuteExpected = 2;
    int secondExpected = 3;
    int microSecondsExpected = 0;
    time_t epochTime = 1393372923;
    DateTime dateTime;
    dateTime = epochTime;
    EXPECT_EQ(yearExpected, dateTime.Year());
    EXPECT_EQ(monthExpected, dateTime.Month());
    EXPECT_EQ(dayExpected, dateTime.MonthDay());
    EXPECT_EQ(hourExpected, dateTime.Hour());
    EXPECT_EQ(minuteExpected, dateTime.Minute());
    EXPECT_EQ(secondExpected, dateTime.Second());
    EXPECT_EQ(microSecondsExpected, dateTime.MicroSeconds());
}

TEST_FIXTURE(DateTimeTest, AssignmentTimeSpec)
{
    int yearExpected = 2014;
    int monthExpected = 2;
    int dayExpected = 26;
    int hourExpected = 0; // UTC
    int minuteExpected = 2;
    int secondExpected = 3;
    int microSecondsExpected = 4;
    timespec timeSpec = { 1393372923, 4000 };
    DateTime dateTime;
    dateTime = timeSpec;
    EXPECT_EQ(yearExpected, dateTime.Year());
    EXPECT_EQ(monthExpected, dateTime.Month());
    EXPECT_EQ(dayExpected, dateTime.MonthDay());
    EXPECT_EQ(hourExpected, dateTime.Hour());
    EXPECT_EQ(minuteExpected, dateTime.Minute());
    EXPECT_EQ(secondExpected, dateTime.Second());
    EXPECT_EQ(microSecondsExpected, dateTime.MicroSeconds());
}

TEST_FIXTURE(DateTimeTest, AssignmentTimeVal)
{
    int yearExpected = 2014;
    int monthExpected = 2;
    int dayExpected = 26;
    int hourExpected = 0; // UTC
    int minuteExpected = 2;
    int secondExpected = 3;
    int microSecondsExpected = 4;
    timeval timeVal = { 1393372923, 4 };
    DateTime dateTime;
    dateTime = timeVal;
    EXPECT_EQ(yearExpected, dateTime.Year());
    EXPECT_EQ(monthExpected, dateTime.Month());
    EXPECT_EQ(dayExpected, dateTime.MonthDay());
    EXPECT_EQ(hourExpected, dateTime.Hour());
    EXPECT_EQ(minuteExpected, dateTime.Minute());
    EXPECT_EQ(secondExpected, dateTime.Second());
    EXPECT_EQ(microSecondsExpected, dateTime.MicroSeconds());
}

TEST_FIXTURE(DateTimeTest, CastOperatorEpochMicroSecondsFromLocalTime)
{
    int year = 2014;
    int month = 2;
    int day = 26;
    int hour = 1;
    int minute = 2;
    int second = 3;
    int microSeconds = 4;
    DateTime dateTime(year, month, day, hour, minute, second, microSeconds);
    time_t epochExpected = 1393372923;
    EXPECT_EQ(epochExpected, time_t(dateTime));
}

TEST_FIXTURE(DateTimeTest, CastOperatorEpochMicroSecondsFromUTCTime)
{
    int year = 2014;
    int month = 2;
    int day = 26;
    int hour = 0;
    int minute = 2;
    int second = 3;
    int microSeconds = 4;
    DateTime dateTime = DateTime::CreateUTC(year, month, day, hour, minute, second, microSeconds);
    time_t epochExpected = 1393372923;
    EXPECT_EQ(epochExpected, time_t(dateTime));
}

TEST_FIXTURE(DateTimeTest, CastOperatorTimeSpec)
{
    int year = 2014;
    int month = 2;
    int day = 26;
    int hour = 1;
    int minute = 2;
    int second = 3;
    int microSeconds = 4;
    DateTime dateTime(year, month, day, hour, minute, second, microSeconds);
    timespec timeSpecExpected = { 1393372923, 4000 };
    timespec timeSpecActual = timespec(dateTime);
    EXPECT_EQ(timeSpecExpected.tv_sec, timeSpecActual.tv_sec);
    EXPECT_EQ(timeSpecExpected.tv_nsec, timeSpecActual.tv_nsec);
}

TEST_FIXTURE(DateTimeTest, CastOperatorTimeVal)
{
    int year = 2014;
    int month = 2;
    int day = 26;
    int hour = 1;
    int minute = 2;
    int second = 3;
    int microSeconds = 4;
    DateTime dateTime(year, month, day, hour, minute, second, microSeconds);
    timeval timeValExpected = { 1393372923, 4 };
    timeval timeValActual = timeval(dateTime);
    EXPECT_EQ(timeValExpected.tv_sec, timeValActual.tv_sec);
    EXPECT_EQ(timeValExpected.tv_usec, timeValActual.tv_usec);
}

TEST_FIXTURE(DateTimeTest, AddAssignment)
{
    int year = 2014;
    int month = 2;
    int day = 26;
    int hour = 1;
    int minute = 2;
    int second = 3;
    int microSeconds = 4;
    DateTime dateTime(year, month, day, hour, minute, second, microSeconds);
    TimeSpan add(4000000123450000);
    dateTime += add;
    int yearExpected = 2014;
    int monthExpected = 4;
    int dayExpected = 13;
    int hourExpected = 9;
    int minuteExpected = 8;
    int secondExpected = 43;
    int microSecondsExpected = 123454;
    EXPECT_EQ(yearExpected, dateTime.Year());
    EXPECT_EQ(monthExpected, dateTime.Month());
    EXPECT_EQ(dayExpected, dateTime.MonthDay());
    EXPECT_EQ(hourExpected, dateTime.Hour());
    EXPECT_EQ(minuteExpected, dateTime.Minute());
    EXPECT_EQ(secondExpected, dateTime.Second());
    EXPECT_EQ(microSecondsExpected, dateTime.MicroSeconds());
}

TEST_FIXTURE(DateTimeTest, SubtractAssignment)
{
    int year = 2014;
    int month = 2;
    int day = 26;
    int hour = 1;
    int minute = 2;
    int second = 3;
    int microSeconds = 4;
    DateTime dateTime(year, month, day, hour, minute, second, microSeconds);
    TimeSpan subtract(4000000123450000);
    dateTime -= subtract;
    int yearExpected = 2014;
    int monthExpected = 1;
    int dayExpected = 10;
    int hourExpected = 17;
    int minuteExpected = 55;
    int secondExpected = 22;
    int microSecondsExpected = 876554;
    EXPECT_EQ(yearExpected, dateTime.Year());
    EXPECT_EQ(monthExpected, dateTime.Month());
    EXPECT_EQ(dayExpected, dateTime.MonthDay());
    EXPECT_EQ(hourExpected, dateTime.Hour());
    EXPECT_EQ(minuteExpected, dateTime.Minute());
    EXPECT_EQ(secondExpected, dateTime.Second());
    EXPECT_EQ(microSecondsExpected, dateTime.MicroSeconds());
}

TEST_FIXTURE(DateTimeTest, Equals)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    DateTime dateTime1(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    DateTime dateTime2(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    DateTime dateTime3(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    DateTime dateTime4(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    DateTime dateTime5(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    DateTime dateTime6(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    DateTime dateTime7(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    DateTime dateTime8(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_TRUE(dateTime.Equals(dateTime));
    EXPECT_FALSE(dateTime.Equals(dateTime1));
    EXPECT_FALSE(dateTime.Equals(dateTime2));
    EXPECT_FALSE(dateTime.Equals(dateTime3));
    EXPECT_FALSE(dateTime.Equals(dateTime4));
    EXPECT_FALSE(dateTime.Equals(dateTime5));
    EXPECT_FALSE(dateTime.Equals(dateTime6));
    EXPECT_FALSE(dateTime.Equals(dateTime7));
    EXPECT_TRUE(dateTime.Equals(dateTime8));
    EXPECT_TRUE(dateTime8.Equals(dateTime));
    EXPECT_FALSE(dateTime1.Equals(dateTime));
}

TEST_FIXTURE(DateTimeTest, LessThan)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    DateTime dateTime1(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    DateTime dateTime2(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    DateTime dateTime3(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    DateTime dateTime4(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    DateTime dateTime5(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    DateTime dateTime6(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    DateTime dateTime7(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    DateTime dateTime8(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_FALSE(dateTime.LessThan(dateTime));
    EXPECT_TRUE(dateTime.LessThan(dateTime1));
    EXPECT_TRUE(dateTime.LessThan(dateTime2));
    EXPECT_TRUE(dateTime.LessThan(dateTime3));
    EXPECT_TRUE(dateTime.LessThan(dateTime4));
    EXPECT_TRUE(dateTime.LessThan(dateTime5));
    EXPECT_TRUE(dateTime.LessThan(dateTime6));
    EXPECT_TRUE(dateTime.LessThan(dateTime7));
    EXPECT_FALSE(dateTime.LessThan(dateTime8));
    EXPECT_FALSE(dateTime1.LessThan(dateTime));
    EXPECT_FALSE(dateTime2.LessThan(dateTime));
    EXPECT_FALSE(dateTime3.LessThan(dateTime));
    EXPECT_FALSE(dateTime4.LessThan(dateTime));
    EXPECT_FALSE(dateTime5.LessThan(dateTime));
    EXPECT_FALSE(dateTime6.LessThan(dateTime));
    EXPECT_FALSE(dateTime7.LessThan(dateTime));
    EXPECT_FALSE(dateTime8.LessThan(dateTime));
}

TEST_FIXTURE(DateTimeTest, NowLocal)
{
    DateTime currentDateTime = DateTime::NowLocal();
    time_t timeVal = time(0);
#if defined(_MSC_VER)
#pragma warning (disable: 4996)
#endif
	tm timeLocal = *localtime(&timeVal);
#if defined(_MSC_VER)
#pragma warning (default: 4996)
#endif
	EXPECT_EQ(timeLocal.tm_year + 1900, currentDateTime.Year());
    EXPECT_EQ(timeLocal.tm_mon + 1, currentDateTime.Month());
    EXPECT_EQ(timeLocal.tm_mday, currentDateTime.MonthDay());
    EXPECT_EQ(timeLocal.tm_hour, currentDateTime.Hour());
    EXPECT_EQ(timeLocal.tm_min, currentDateTime.Minute());
    EXPECT_EQ(timeLocal.tm_sec, currentDateTime.Second());
}

TEST_FIXTURE(DateTimeTest, NowUTC)
{
    DateTime currentDateTime = DateTime::NowUTC();
    time_t timeVal = time(0);
#if defined(_MSC_VER)
#pragma warning (disable: 4996)
#endif
	tm timeLocal = *gmtime(&timeVal);
#if defined(_MSC_VER)
#pragma warning (default: 4996)
#endif
	EXPECT_EQ(timeLocal.tm_year + 1900, currentDateTime.Year());
    EXPECT_EQ(timeLocal.tm_mon + 1, currentDateTime.Month());
    EXPECT_EQ(timeLocal.tm_mday, currentDateTime.MonthDay());
    EXPECT_EQ(timeLocal.tm_hour, currentDateTime.Hour());
    EXPECT_EQ(timeLocal.tm_min, currentDateTime.Minute());
    EXPECT_EQ(timeLocal.tm_sec, currentDateTime.Second());
}

TEST_FIXTURE(DateTimeTest, CreateUTCYearMonthDayHourMinuteSecond)
{
    int year = 2014;
    int month = 2;
    int day = 26;
    int hour = 1;
    int minute = 2;
    int second = 3;
    int microSeconds = 0;
    DateTime dateTime = DateTime::CreateUTC(year, month, day, hour, minute, second);
    EXPECT_EQ(year, dateTime.Year());
    EXPECT_EQ(month, dateTime.Month());
    EXPECT_EQ(day, dateTime.MonthDay());
    EXPECT_EQ(hour, dateTime.Hour());
    EXPECT_EQ(minute, dateTime.Minute());
    EXPECT_EQ(second, dateTime.Second());
    EXPECT_EQ(microSeconds, dateTime.MicroSeconds());
}

TEST_FIXTURE(DateTimeTest, CreateUTCYearMonthDayHourMinuteSecondMicroSecond)
{
    int year = 2014;
    int month = 2;
    int day = 26;
    int hour = 1;
    int minute = 2;
    int second = 3;
    int microSeconds = 4;
    DateTime dateTime = DateTime::CreateUTC(year, month, day, hour, minute, second, microSeconds);
    EXPECT_EQ(year, dateTime.Year());
    EXPECT_EQ(month, dateTime.Month());
    EXPECT_EQ(day, dateTime.MonthDay());
    EXPECT_EQ(hour, dateTime.Hour());
    EXPECT_EQ(minute, dateTime.Minute());
    EXPECT_EQ(second, dateTime.Second());
    EXPECT_EQ(microSeconds, dateTime.MicroSeconds());
}

TEST_FIXTURE(DateTimeTest, CreateUTCYearMonthNameDayHourMinuteSecond)
{
    int year = 2014;
    int month = 2;
    MonthType monthName = MonthType::February;
    int day = 26;
    int hour = 1;
    int minute = 2;
    int second = 3;
    int microSeconds = 0;
    DateTime dateTime = DateTime::CreateUTC(year, monthName, day, hour, minute, second);
    EXPECT_EQ(year, dateTime.Year());
    EXPECT_EQ(month, dateTime.Month());
    EXPECT_EQ(day, dateTime.MonthDay());
    EXPECT_EQ(hour, dateTime.Hour());
    EXPECT_EQ(minute, dateTime.Minute());
    EXPECT_EQ(second, dateTime.Second());
    EXPECT_EQ(microSeconds, dateTime.MicroSeconds());
}

TEST_FIXTURE(DateTimeTest, CreateUTCYearMonthNameDayHourMinuteSecondMicroSecond)
{
    int year = 2014;
    int month = 2;
    MonthType monthName = MonthType::February;
    int day = 26;
    int hour = 1;
    int minute = 2;
    int second = 3;
    int microSeconds = 4;
    DateTime dateTime = DateTime::CreateUTC(year, monthName, day, hour, minute, second, microSeconds);
    EXPECT_EQ(year, dateTime.Year());
    EXPECT_EQ(month, dateTime.Month());
    EXPECT_EQ(day, dateTime.MonthDay());
    EXPECT_EQ(hour, dateTime.Hour());
    EXPECT_EQ(minute, dateTime.Minute());
    EXPECT_EQ(second, dateTime.Second());
    EXPECT_EQ(microSeconds, dateTime.MicroSeconds());
}

TEST_FIXTURE(DateTimeTest, CreateLocalYearMonthDayHourMinuteSecond)
{
    int year = 2014;
    int month = 2;
    int day = 26;
    int hour = 1;
    int minute = 2;
    int second = 3;
    int microSeconds = 0;
    DateTime dateTime = DateTime::CreateLocal(year, month, day, hour, minute, second);
    EXPECT_EQ(year, dateTime.Year());
    EXPECT_EQ(month, dateTime.Month());
    EXPECT_EQ(day, dateTime.MonthDay());
    EXPECT_EQ(hour, dateTime.Hour());
    EXPECT_EQ(minute, dateTime.Minute());
    EXPECT_EQ(second, dateTime.Second());
    EXPECT_EQ(microSeconds, dateTime.MicroSeconds());
}

TEST_FIXTURE(DateTimeTest, CreateLocalYearMonthDayHourMinuteSecondMicroSecond)
{
    int year = 2014;
    int month = 2;
    int day = 26;
    int hour = 1;
    int minute = 2;
    int second = 3;
    int microSeconds = 4;
    DateTime dateTime = DateTime::CreateLocal(year, month, day, hour, minute, second, microSeconds);
    EXPECT_EQ(year, dateTime.Year());
    EXPECT_EQ(month, dateTime.Month());
    EXPECT_EQ(day, dateTime.MonthDay());
    EXPECT_EQ(hour, dateTime.Hour());
    EXPECT_EQ(minute, dateTime.Minute());
    EXPECT_EQ(second, dateTime.Second());
    EXPECT_EQ(microSeconds, dateTime.MicroSeconds());
}

TEST_FIXTURE(DateTimeTest, CreateLocalYearMonthNameDayHourMinuteSecond)
{
    int year = 2014;
    int month = 2;
    MonthType monthName = MonthType::February;
    int day = 26;
    int hour = 1;
    int minute = 2;
    int second = 3;
    int microSeconds = 0;
    DateTime dateTime = DateTime::CreateLocal(year, monthName, day, hour, minute, second);
    EXPECT_EQ(year, dateTime.Year());
    EXPECT_EQ(month, dateTime.Month());
    EXPECT_EQ(day, dateTime.MonthDay());
    EXPECT_EQ(hour, dateTime.Hour());
    EXPECT_EQ(minute, dateTime.Minute());
    EXPECT_EQ(second, dateTime.Second());
    EXPECT_EQ(microSeconds, dateTime.MicroSeconds());
}

TEST_FIXTURE(DateTimeTest, CreateLocalYearMonthNameDayHourMinuteSecondMicroSecond)
{
    int year = 2014;
    int month = 2;
    MonthType monthName = MonthType::February;
    int day = 26;
    int hour = 1;
    int minute = 2;
    int second = 3;
    int microSeconds = 4;
    DateTime dateTime = DateTime::CreateLocal(year, monthName, day, hour, minute, second, microSeconds);
    EXPECT_EQ(year, dateTime.Year());
    EXPECT_EQ(month, dateTime.Month());
    EXPECT_EQ(day, dateTime.MonthDay());
    EXPECT_EQ(hour, dateTime.Hour());
    EXPECT_EQ(minute, dateTime.Minute());
    EXPECT_EQ(second, dateTime.Second());
    EXPECT_EQ(microSeconds, dateTime.MicroSeconds());
}

#if defined(__GNUC__)
TEST_FIXTURE(DateTimeTest, OffsetFromUTC)
{
    DateTime dateTime = DateTime::CreateLocal(2014, 02, 26, 1, 2, 3, 567891);
    time_t time = dateTime;
    tm localTime = *localtime(&time);
	int64_t expected = localTime.tm_gmtoff * DateTime::MicroSecondsPerSecond;
    int64_t actual = dateTime.OffsetFromUTC().MicroSeconds();
    EXPECT_EQ(expected, actual);
}
#endif

#if defined(__GNUC__)
TEST_FIXTURE(DateTimeTest, OffsetFromUTC_SummerTime)
{
    DateTime dateTime = DateTime::CreateLocal(2014, 06, 26, 1, 2, 3, 567891);
    time_t time = dateTime;
    tm localTime = *localtime(&time);
    int64_t expected = localTime.tm_gmtoff * DateTime::MicroSecondsPerSecond;
    int64_t actual = dateTime.OffsetFromUTC().MicroSeconds();
    EXPECT_EQ(expected, actual);
}
#endif

#if defined(__GNUC__)
TEST_FIXTURE(DateTimeTest, TimeZoneNameLocal)
{
    DateTime dateTime(2014, 02, 26, 1, 2, 3, 567891);
    time_t time = dateTime;
    tm localTime = *localtime(&time);
    string nameExpected = localTime.tm_zone;
    string nameActual = dateTime.TimeZoneName();
    EXPECT_EQ(nameExpected, nameActual);
}
#endif

#if defined(__GNUC__)
TEST_FIXTURE(DateTimeTest, TimeZoneNameLocalSummerTime)
{
    DateTime dateTime(2014, 06, 26, 1, 2, 3, 567891);
    time_t time = dateTime;
    tm localTime = *localtime(&time);
    string nameExpected = localTime.tm_zone;
    string nameActual = dateTime.TimeZoneName();
    EXPECT_EQ(nameExpected, nameActual);
}
#endif

#if defined(__GNUC__)
TEST_FIXTURE(DateTimeTest, TimeZoneNameUTC)
{
    DateTime dateTime = DateTime::CreateUTC(2014, 02, 26, 1, 2, 3, 567891);
    time_t time = dateTime;
    tm localTime = *gmtime(&time);
    string nameExpected = localTime.tm_zone;
    string nameActual = dateTime.TimeZoneName();
    EXPECT_EQ(nameExpected, nameActual);
}
#endif

TEST_FIXTURE(DateTimeTest, IsDayLightSavings)
{
    DateTime dateTime = DateTime::CreateLocal(2014, 02, 26, 1, 2, 3, 567891);
    EXPECT_FALSE(dateTime.IsDaylightSavings());
    dateTime = DateTime::CreateLocal(2014, 06, 26, 1, 2, 3, 567891);
    EXPECT_TRUE(dateTime.IsDaylightSavings());
}

TEST_FIXTURE(DateTimeTest, ConvertToLocalTime)
{
    DateTime dateTimeUTC = DateTime::CreateUTC(2014, 02, 26, 1, 2, 3, 567891);
    DateTime dateTimeLocal = dateTimeUTC.ConvertToLocalTime();
    int year = 2014;
    int month = 2;
    int day = 26;
    int hour = 2;
    int minute = 2;
    int second = 3;
    int microSeconds = 567891;
    EXPECT_EQ(year, dateTimeLocal.Year());
    EXPECT_EQ(month, dateTimeLocal.Month());
    EXPECT_EQ(day, dateTimeLocal.MonthDay());
    EXPECT_EQ(hour, dateTimeLocal.Hour());
    EXPECT_EQ(minute, dateTimeLocal.Minute());
    EXPECT_EQ(second, dateTimeLocal.Second());
    EXPECT_EQ(microSeconds, dateTimeLocal.MicroSeconds());
}

TEST_FIXTURE(DateTimeTest, ConvertToUTCTime)
{
    DateTime dateTimeLocal = DateTime::CreateLocal(2014, 02, 26, 1, 2, 3, 567891);
    DateTime dateTimeUTC = dateTimeLocal.ConvertToUTCTime();
    int year = 2014;
    int month = 2;
    int day = 26;
    int hour = 0;
    int minute = 2;
    int second = 3;
    int microSeconds = 567891;
    EXPECT_EQ(year, dateTimeUTC.Year());
    EXPECT_EQ(month, dateTimeUTC.Month());
    EXPECT_EQ(day, dateTimeUTC.MonthDay());
    EXPECT_EQ(hour, dateTimeUTC.Hour());
    EXPECT_EQ(minute, dateTimeUTC.Minute());
    EXPECT_EQ(second, dateTimeUTC.Second());
    EXPECT_EQ(microSeconds, dateTimeUTC.MicroSeconds());
}

TEST_FIXTURE(DateTimeTest, PrintTo)
{
    DateTime dateTime(2014, 02, 26, 1, 2, 3, 567891);
    ostringstream stream;
    PrintTo(dateTime, stream);
    EXPECT_EQ("2014-02-26 01:02:03.567891", stream.str());
}

TEST_FIXTURE(DateTimeTest, PrintToFormatted)
{
    DateTime dateTime = DateTime::CreateUTC(2014, 02, 26, 1, 2, 3);
    ostringstream stream;
    dateTime.PrintTo(stream, "%F %T");
    EXPECT_EQ("2014-02-26 01:02:03", stream.str());
#if defined(__GNUC__)
	stream.str("");
    dateTime.PrintTo(stream, "%F %T %Z");
    EXPECT_EQ("2014-02-26 01:02:03 GMT", stream.str());
#endif
}

TEST_FIXTURE(DateTimeTest, PrintToFormattedInvalidFormatString)
{
    DateTime dateTime = DateTime::CreateUTC(2014, 02, 26, 1, 2, 3);
    ostringstream stream;
    EXPECT_THROW(dateTime.PrintTo(stream, ""), Exception);
}

TEST_FIXTURE(DateTimeTest, OperatorAddTimeSpanDateTime)
{
    DateTime dateTime(2014, 2, 26, 1, 2, 3, 567891);

    TimeSpan timeSpan;
    DateTime actual;
    DateTime expected;

    timeSpan = 1000; // One microsecond
    actual = timeSpan + dateTime;
    expected = DateTime::CreateLocal(2014, 2, 26, 1, 2, 3, 567892);
    EXPECT_EQ(expected, actual);

    timeSpan = 1000000000; // One second
    actual = timeSpan + dateTime;
    expected = DateTime::CreateLocal(2014, 2, 26, 1, 2, 4, 567891);
    EXPECT_EQ(expected, actual);

    timeSpan = 60000000000ll; // One minute
    actual = timeSpan + dateTime;
    expected = DateTime::CreateLocal(2014, 2, 26, 1, 3, 3, 567891);
    EXPECT_EQ(expected, actual);

    timeSpan = 3600000000000ll; // One hour
    actual = timeSpan + dateTime;
    expected = DateTime::CreateLocal(2014, 2, 26, 2, 2, 3, 567891);
    EXPECT_EQ(expected, actual);

    timeSpan = 86400000000000ll; // One day
    actual = timeSpan + dateTime;
    expected = DateTime::CreateLocal(2014, 2, 27, 1, 2, 3, 567891);
    EXPECT_EQ(expected, actual);

    timeSpan = (28 + 31 + 30) * 86400000000000ll; // Three months (28 + 31 + 30 days)
    // Adds one extra hour due to summer time (CET->CEST)
    actual = timeSpan + dateTime;
    expected = DateTime::CreateLocal(2014, 5, 26, 2, 2, 3, 567891);
    EXPECT_EQ(expected, actual);

    timeSpan = 365 * 86400000000000ll; // One year (365 days)
    actual = timeSpan + dateTime;
    expected = DateTime::CreateLocal(2015, 2, 26, 1, 2, 3, 567891);
    EXPECT_EQ(expected, actual);

    timeSpan = 40000000500000000ll;
    // Adds one extra hour due to summer time (CET->CEST)
    actual = timeSpan + dateTime;
    expected = DateTime::CreateLocal(2015, 6, 4, 1, 8, 44, 67891);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(DateTimeTest, OperatorAddDateTimeTimeSpan)
{
    DateTime dateTime(2014, 2, 26, 1, 2, 3, 567891);

    TimeSpan timeSpan;
    DateTime actual;
    DateTime expected;

    timeSpan = 1000; // One microsecond
    actual = dateTime + timeSpan;
    expected = DateTime::CreateLocal(2014, 2, 26, 1, 2, 3, 567892);
    EXPECT_EQ(expected, actual);

    timeSpan = 1000000000; // One second
    actual = dateTime + timeSpan;
    expected = DateTime::CreateLocal(2014, 2, 26, 1, 2, 4, 567891);
    EXPECT_EQ(expected, actual);

    timeSpan = 60000000000ll; // One minute
    actual = dateTime + timeSpan;
    expected = DateTime::CreateLocal(2014, 2, 26, 1, 3, 3, 567891);
    EXPECT_EQ(expected, actual);

    timeSpan = 3600000000000ll; // One hour
    actual = dateTime + timeSpan;
    expected = DateTime::CreateLocal(2014, 2, 26, 2, 2, 3, 567891);
    EXPECT_EQ(expected, actual);

    timeSpan = 86400000000000ll; // One day
    actual = dateTime + timeSpan;
    expected = DateTime::CreateLocal(2014, 2, 27, 1, 2, 3, 567891);
    EXPECT_EQ(expected, actual);

    timeSpan = (28 + 31 + 30) * 86400000000000ll; // Three months (28 + 31 + 30 days)
    // Adds one extra hour due to summer time (CET->CEST)
    actual = dateTime + timeSpan;
    expected = DateTime::CreateLocal(2014, 5, 26, 2, 2, 3, 567891);
    EXPECT_EQ(expected, actual);

    timeSpan = 365 * 86400000000000ll; // One year (365 days)
    actual = dateTime + timeSpan;
    expected = DateTime::CreateLocal(2015, 2, 26, 1, 2, 3, 567891);
    EXPECT_EQ(expected, actual);

    timeSpan = 40000000500000000ll;
    // Adds one extra hour due to summer time (CET->CEST)
    actual = dateTime + timeSpan;
    expected = DateTime::CreateLocal(2015, 6, 4, 1, 8, 44, 67891);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(DateTimeTest, OperatorSubtractDateTimeTimeSpan)
{
    DateTime dateTime(2014, 2, 26, 1, 2, 3, 100000);

    TimeSpan timeSpan;
    DateTime actual;
    DateTime expected;

    timeSpan = 1000; // One microsecond
    actual = dateTime - timeSpan;
    expected = DateTime::CreateLocal(2014, 2, 26, 1, 2, 3, 99999);
    EXPECT_EQ(expected, actual);

    timeSpan = 1000000000; // One second
    actual = dateTime - timeSpan;
    expected = DateTime::CreateLocal(2014, 2, 26, 1, 2, 2, 100000);
    EXPECT_EQ(expected, actual);

    timeSpan = 60000000000ll; // One minute
    actual = dateTime - timeSpan;
    expected = DateTime::CreateLocal(2014, 2, 26, 1, 1, 3, 100000);
    EXPECT_EQ(expected, actual);

    timeSpan = 3600000000000ll; // One hour
    actual = dateTime - timeSpan;
    expected = DateTime::CreateLocal(2014, 2, 26, 0, 2, 3, 100000);
    EXPECT_EQ(expected, actual);

    timeSpan = 86400000000000ll; // One day
    actual = dateTime - timeSpan;
    expected = DateTime::CreateLocal(2014, 2, 25, 1, 2, 3, 100000);
    EXPECT_EQ(expected, actual);

    timeSpan = (31 + 31 + 30 + 31 + 30 + 31) * 86400000000000ll; // Six months (31 + 31 + 30 + 31 + 30 + 31 days)
    // Adds one extra hour due to summer time (CET->CEST)
    actual = dateTime - timeSpan;
    expected = DateTime::CreateLocal(2013, 8, 26, 2, 2, 3, 100000);
    EXPECT_EQ(expected, actual);

    timeSpan = 365 * 86400000000000ll; // One year (365 days)
    actual = dateTime - timeSpan;
    expected = DateTime::CreateLocal(2013, 2, 26, 1, 2, 3, 100000);
    EXPECT_EQ(expected, actual);

    timeSpan = 40000000123450000ll;
    // Adds one extra hour due to summer time (CET->CEST)
    actual = dateTime - timeSpan;
    expected = DateTime::CreateLocal(2012, 11, 20, 1, 55, 22, 976550);
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(DateTimeTest, OperatorSubtractDateTimeDateTime)
{
    DateTime dateTime1(2014, 2, 26, 1, 2, 3, 100000);

    DateTime dateTime2;
    TimeSpan actual;
    TimeSpan expected;

    dateTime2 = DateTime::CreateLocal(2014, 2, 26, 1, 2, 3, 99999);
    actual = dateTime1 - dateTime2;
    expected = 1000; // One microsecond
    EXPECT_EQ(expected, actual);

    dateTime2 = DateTime::CreateLocal(2014, 2, 26, 1, 2, 2, 100000);
    actual = dateTime1 - dateTime2;
    expected = 1000000000; // One second
    EXPECT_EQ(expected, actual);

    dateTime2 = DateTime::CreateLocal(2014, 2, 26, 1, 1, 3, 100000);
    actual = dateTime1 - dateTime2;
    expected = 60000000000ll; // One minute
    EXPECT_EQ(expected, actual);

    dateTime2 = DateTime::CreateLocal(2014, 2, 26, 0, 2, 3, 100000);
    actual = dateTime1 - dateTime2;
    expected = 3600000000000ll; // One hour
    EXPECT_EQ(expected, actual);

    dateTime2 = DateTime::CreateLocal(2014, 2, 25, 1, 2, 3, 100000);
    actual = dateTime1 - dateTime2;
    expected = 86400000000000ll; // One day
    EXPECT_EQ(expected, actual);

    // Add one extra hour due to summer time (CET->CEST)
    dateTime2 = DateTime::CreateLocal(2013, 8, 26, 2, 2, 3, 100000);
    actual = dateTime1 - dateTime2;
    expected = (31 + 31 + 30 + 31 + 30 + 31) * 86400000000000ll; // Six months (31 + 31 + 30 + 31 + 30 + 31 days)
    EXPECT_EQ(expected, actual);

    dateTime2 = DateTime::CreateLocal(2013, 2, 26, 1, 2, 3, 100000);
    actual = dateTime1 - dateTime2;
    expected = 365 * 86400000000000ll; // One year (365 days)
    EXPECT_EQ(expected, actual);

    // Add one extra hour due to summer time (CET->CEST)
    dateTime2 = DateTime::CreateLocal(2012, 11, 20, 1, 55, 22, 976550);
    actual = dateTime1 - dateTime2;
    expected = 40000000123450000ll;
    EXPECT_EQ(expected, actual);
}

TEST_FIXTURE(DateTimeTest, OperatorEqualsDateTimeDateTime)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    DateTime dateTime1(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    DateTime dateTime2(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    DateTime dateTime3(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    DateTime dateTime4(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    DateTime dateTime5(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    DateTime dateTime6(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    DateTime dateTime7(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    DateTime dateTime8(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_FALSE(dateTime == dateTime1);
    EXPECT_FALSE(dateTime == dateTime2);
    EXPECT_FALSE(dateTime == dateTime3);
    EXPECT_FALSE(dateTime == dateTime4);
    EXPECT_FALSE(dateTime == dateTime5);
    EXPECT_FALSE(dateTime == dateTime6);
    EXPECT_FALSE(dateTime == dateTime7);
    EXPECT_TRUE(dateTime == dateTime8);
}

TEST_FIXTURE(DateTimeTest, OperatorEqualsTimeSpecDataTime)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec1 = (timespec)DateTime(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec2 = (timespec)DateTime(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec3 = (timespec)DateTime(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    timespec timeSpec4 = (timespec)DateTime(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    timespec timeSpec5 = (timespec)DateTime(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    timespec timeSpec6 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    timespec timeSpec7 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    timespec timeSpec8 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_FALSE(timeSpec1 == dateTime);
    EXPECT_FALSE(timeSpec2 == dateTime);
    EXPECT_FALSE(timeSpec3 == dateTime);
    EXPECT_FALSE(timeSpec4 == dateTime);
    EXPECT_FALSE(timeSpec5 == dateTime);
    EXPECT_FALSE(timeSpec6 == dateTime);
    EXPECT_FALSE(timeSpec7 == dateTime);
    EXPECT_TRUE(timeSpec8 == dateTime);
}

TEST_FIXTURE(DateTimeTest, OperatorEqualsTimeValDataTime)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal1 = (timeval)DateTime(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal2 = (timeval)DateTime(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal3 = (timeval)DateTime(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    timeval timeVal4 = (timeval)DateTime(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    timeval timeVal5 = (timeval)DateTime(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    timeval timeVal6 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    timeval timeVal7 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    timeval timeVal8 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_FALSE(timeVal1 == dateTime);
    EXPECT_FALSE(timeVal2 == dateTime);
    EXPECT_FALSE(timeVal3 == dateTime);
    EXPECT_FALSE(timeVal4 == dateTime);
    EXPECT_FALSE(timeVal5 == dateTime);
    EXPECT_FALSE(timeVal6 == dateTime);
    EXPECT_FALSE(timeVal7 == dateTime);
    EXPECT_TRUE(timeVal8 == dateTime);
}

TEST_FIXTURE(DateTimeTest, OperatorEqualsDateTimeTimeSpec)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec1 = (timespec)DateTime(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec2 = (timespec)DateTime(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec3 = (timespec)DateTime(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    timespec timeSpec4 = (timespec)DateTime(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    timespec timeSpec5 = (timespec)DateTime(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    timespec timeSpec6 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    timespec timeSpec7 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    timespec timeSpec8 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_FALSE(dateTime == timeSpec1);
    EXPECT_FALSE(dateTime == timeSpec2);
    EXPECT_FALSE(dateTime == timeSpec3);
    EXPECT_FALSE(dateTime == timeSpec4);
    EXPECT_FALSE(dateTime == timeSpec5);
    EXPECT_FALSE(dateTime == timeSpec6);
    EXPECT_FALSE(dateTime == timeSpec7);
    EXPECT_TRUE(dateTime == timeSpec8);
}

TEST_FIXTURE(DateTimeTest, OperatorEqualsDateTimeTimeVal)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal1 = (timeval)DateTime(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal2 = (timeval)DateTime(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal3 = (timeval)DateTime(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    timeval timeVal4 = (timeval)DateTime(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    timeval timeVal5 = (timeval)DateTime(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    timeval timeVal6 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    timeval timeVal7 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    timeval timeVal8 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_FALSE(dateTime == timeVal1);
    EXPECT_FALSE(dateTime == timeVal2);
    EXPECT_FALSE(dateTime == timeVal3);
    EXPECT_FALSE(dateTime == timeVal4);
    EXPECT_FALSE(dateTime == timeVal5);
    EXPECT_FALSE(dateTime == timeVal6);
    EXPECT_FALSE(dateTime == timeVal7);
    EXPECT_TRUE(dateTime == timeVal8);
}

TEST_FIXTURE(DateTimeTest, OperatorNotEqualsDateTimeDateTime)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    DateTime dateTime1(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    DateTime dateTime2(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    DateTime dateTime3(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    DateTime dateTime4(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    DateTime dateTime5(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    DateTime dateTime6(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    DateTime dateTime7(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    DateTime dateTime8(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_TRUE(dateTime != dateTime1);
    EXPECT_TRUE(dateTime != dateTime2);
    EXPECT_TRUE(dateTime != dateTime3);
    EXPECT_TRUE(dateTime != dateTime4);
    EXPECT_TRUE(dateTime != dateTime5);
    EXPECT_TRUE(dateTime != dateTime6);
    EXPECT_TRUE(dateTime != dateTime7);
    EXPECT_FALSE(dateTime != dateTime8);
}

TEST_FIXTURE(DateTimeTest, OperatorNotEqualsTimeSpecDateTime)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec1 = (timespec)DateTime(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec2 = (timespec)DateTime(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec3 = (timespec)DateTime(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    timespec timeSpec4 = (timespec)DateTime(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    timespec timeSpec5 = (timespec)DateTime(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    timespec timeSpec6 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    timespec timeSpec7 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    timespec timeSpec8 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_TRUE(timeSpec1 != dateTime);
    EXPECT_TRUE(timeSpec2 != dateTime);
    EXPECT_TRUE(timeSpec3 != dateTime);
    EXPECT_TRUE(timeSpec4 != dateTime);
    EXPECT_TRUE(timeSpec5 != dateTime);
    EXPECT_TRUE(timeSpec6 != dateTime);
    EXPECT_TRUE(timeSpec7 != dateTime);
    EXPECT_FALSE(timeSpec8 != dateTime);
}

TEST_FIXTURE(DateTimeTest, OperatorNotEqualsTimeValDateTime)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal1 = (timeval)DateTime(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal2 = (timeval)DateTime(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal3 = (timeval)DateTime(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    timeval timeVal4 = (timeval)DateTime(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    timeval timeVal5 = (timeval)DateTime(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    timeval timeVal6 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    timeval timeVal7 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    timeval timeVal8 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_TRUE(timeVal1 != dateTime);
    EXPECT_TRUE(timeVal2 != dateTime);
    EXPECT_TRUE(timeVal3 != dateTime);
    EXPECT_TRUE(timeVal4 != dateTime);
    EXPECT_TRUE(timeVal5 != dateTime);
    EXPECT_TRUE(timeVal6 != dateTime);
    EXPECT_TRUE(timeVal7 != dateTime);
    EXPECT_FALSE(timeVal8 != dateTime);
}

TEST_FIXTURE(DateTimeTest, OperatorNotEqualsDateTimeTimeSpec)
{
     int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec1 = (timespec)DateTime(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec2 = (timespec)DateTime(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec3 = (timespec)DateTime(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    timespec timeSpec4 = (timespec)DateTime(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    timespec timeSpec5 = (timespec)DateTime(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    timespec timeSpec6 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    timespec timeSpec7 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    timespec timeSpec8 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_TRUE(dateTime != timeSpec1);
    EXPECT_TRUE(dateTime != timeSpec2);
    EXPECT_TRUE(dateTime != timeSpec3);
    EXPECT_TRUE(dateTime != timeSpec4);
    EXPECT_TRUE(dateTime != timeSpec5);
    EXPECT_TRUE(dateTime != timeSpec6);
    EXPECT_TRUE(dateTime != timeSpec7);
    EXPECT_FALSE(dateTime != timeSpec8);
}

TEST_FIXTURE(DateTimeTest, OperatorNotEqualsDateTimeTimeVal)
{
     int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal1 = (timeval)DateTime(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal2 = (timeval)DateTime(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal3 = (timeval)DateTime(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    timeval timeVal4 = (timeval)DateTime(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    timeval timeVal5 = (timeval)DateTime(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    timeval timeVal6 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    timeval timeVal7 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    timeval timeVal8 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_TRUE(dateTime != timeVal1);
    EXPECT_TRUE(dateTime != timeVal2);
    EXPECT_TRUE(dateTime != timeVal3);
    EXPECT_TRUE(dateTime != timeVal4);
    EXPECT_TRUE(dateTime != timeVal5);
    EXPECT_TRUE(dateTime != timeVal6);
    EXPECT_TRUE(dateTime != timeVal7);
    EXPECT_FALSE(dateTime != timeVal8);
}

TEST_FIXTURE(DateTimeTest, OperatorLessDateTimeDateTime)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    DateTime dateTime1(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    DateTime dateTime2(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    DateTime dateTime3(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    DateTime dateTime4(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    DateTime dateTime5(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    DateTime dateTime6(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    DateTime dateTime7(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    DateTime dateTime8(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_TRUE(dateTime < dateTime1);
    EXPECT_TRUE(dateTime < dateTime2);
    EXPECT_TRUE(dateTime < dateTime3);
    EXPECT_TRUE(dateTime < dateTime4);
    EXPECT_TRUE(dateTime < dateTime5);
    EXPECT_TRUE(dateTime < dateTime6);
    EXPECT_TRUE(dateTime < dateTime7);
    EXPECT_FALSE(dateTime < dateTime8);
}

TEST_FIXTURE(DateTimeTest, OperatorLessTimeSpecDateTime)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec1 = (timespec)DateTime(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec2 = (timespec)DateTime(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec3 = (timespec)DateTime(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    timespec timeSpec4 = (timespec)DateTime(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    timespec timeSpec5 = (timespec)DateTime(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    timespec timeSpec6 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    timespec timeSpec7 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    timespec timeSpec8 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_FALSE(timeSpec1 < dateTime);
    EXPECT_FALSE(timeSpec2 < dateTime);
    EXPECT_FALSE(timeSpec3 < dateTime);
    EXPECT_FALSE(timeSpec4 < dateTime);
    EXPECT_FALSE(timeSpec5 < dateTime);
    EXPECT_FALSE(timeSpec6 < dateTime);
    EXPECT_FALSE(timeSpec7 < dateTime);
    EXPECT_FALSE(timeSpec8 < dateTime);
}

TEST_FIXTURE(DateTimeTest, OperatorLessTimeValDateTime)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal1 = (timeval)DateTime(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal2 = (timeval)DateTime(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal3 = (timeval)DateTime(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    timeval timeVal4 = (timeval)DateTime(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    timeval timeVal5 = (timeval)DateTime(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    timeval timeVal6 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    timeval timeVal7 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    timeval timeVal8 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_FALSE(timeVal1 < dateTime);
    EXPECT_FALSE(timeVal2 < dateTime);
    EXPECT_FALSE(timeVal3 < dateTime);
    EXPECT_FALSE(timeVal4 < dateTime);
    EXPECT_FALSE(timeVal5 < dateTime);
    EXPECT_FALSE(timeVal6 < dateTime);
    EXPECT_FALSE(timeVal7 < dateTime);
    EXPECT_FALSE(timeVal8 < dateTime);
}

TEST_FIXTURE(DateTimeTest, OperatorLessDateTimeTimeSpec)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec1 = (timespec)DateTime(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec2 = (timespec)DateTime(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec3 = (timespec)DateTime(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    timespec timeSpec4 = (timespec)DateTime(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    timespec timeSpec5 = (timespec)DateTime(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    timespec timeSpec6 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    timespec timeSpec7 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    timespec timeSpec8 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_TRUE(dateTime < timeSpec1);
    EXPECT_TRUE(dateTime < timeSpec2);
    EXPECT_TRUE(dateTime < timeSpec3);
    EXPECT_TRUE(dateTime < timeSpec4);
    EXPECT_TRUE(dateTime < timeSpec5);
    EXPECT_TRUE(dateTime < timeSpec6);
    EXPECT_TRUE(dateTime < timeSpec7);
    EXPECT_FALSE(dateTime < timeSpec8);
}

TEST_FIXTURE(DateTimeTest, OperatorLessDateTimeTimeVal)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal1 = (timeval)DateTime(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal2 = (timeval)DateTime(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal3 = (timeval)DateTime(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    timeval timeVal4 = (timeval)DateTime(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    timeval timeVal5 = (timeval)DateTime(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    timeval timeVal6 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    timeval timeVal7 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    timeval timeVal8 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_TRUE(dateTime < timeVal1);
    EXPECT_TRUE(dateTime < timeVal2);
    EXPECT_TRUE(dateTime < timeVal3);
    EXPECT_TRUE(dateTime < timeVal4);
    EXPECT_TRUE(dateTime < timeVal5);
    EXPECT_TRUE(dateTime < timeVal6);
    EXPECT_TRUE(dateTime < timeVal7);
    EXPECT_FALSE(dateTime < timeVal8);
}

TEST_FIXTURE(DateTimeTest, OperatorGreaterDateTimeDateTime)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    DateTime dateTime1(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    DateTime dateTime2(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    DateTime dateTime3(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    DateTime dateTime4(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    DateTime dateTime5(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    DateTime dateTime6(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    DateTime dateTime7(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    DateTime dateTime8(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_FALSE(dateTime > dateTime1);
    EXPECT_FALSE(dateTime > dateTime2);
    EXPECT_FALSE(dateTime > dateTime3);
    EXPECT_FALSE(dateTime > dateTime4);
    EXPECT_FALSE(dateTime > dateTime5);
    EXPECT_FALSE(dateTime > dateTime6);
    EXPECT_FALSE(dateTime > dateTime7);
    EXPECT_FALSE(dateTime > dateTime8);
}

TEST_FIXTURE(DateTimeTest, OperatorGreaterTimeSpecDateTime)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec1 = (timespec)DateTime(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec2 = (timespec)DateTime(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec3 = (timespec)DateTime(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    timespec timeSpec4 = (timespec)DateTime(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    timespec timeSpec5 = (timespec)DateTime(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    timespec timeSpec6 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    timespec timeSpec7 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    timespec timeSpec8 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_TRUE(timeSpec1 > dateTime);
    EXPECT_TRUE(timeSpec2 > dateTime);
    EXPECT_TRUE(timeSpec3 > dateTime);
    EXPECT_TRUE(timeSpec4 > dateTime);
    EXPECT_TRUE(timeSpec5 > dateTime);
    EXPECT_TRUE(timeSpec6 > dateTime);
    EXPECT_TRUE(timeSpec7 > dateTime);
    EXPECT_FALSE(timeSpec8 > dateTime);
}

TEST_FIXTURE(DateTimeTest, OperatorGreaterTimeValDateTime)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal1 = (timeval)DateTime(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal2 = (timeval)DateTime(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal3 = (timeval)DateTime(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    timeval timeVal4 = (timeval)DateTime(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    timeval timeVal5 = (timeval)DateTime(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    timeval timeVal6 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    timeval timeVal7 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    timeval timeVal8 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_TRUE(timeVal1 > dateTime);
    EXPECT_TRUE(timeVal2 > dateTime);
    EXPECT_TRUE(timeVal3 > dateTime);
    EXPECT_TRUE(timeVal4 > dateTime);
    EXPECT_TRUE(timeVal5 > dateTime);
    EXPECT_TRUE(timeVal6 > dateTime);
    EXPECT_TRUE(timeVal7 > dateTime);
    EXPECT_FALSE(timeVal8 > dateTime);
}

TEST_FIXTURE(DateTimeTest, OperatorGreaterDateTimeTimeSpec)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec1 = (timespec)DateTime(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec2 = (timespec)DateTime(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec3 = (timespec)DateTime(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    timespec timeSpec4 = (timespec)DateTime(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    timespec timeSpec5 = (timespec)DateTime(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    timespec timeSpec6 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    timespec timeSpec7 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    timespec timeSpec8 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_FALSE(dateTime > timeSpec1);
    EXPECT_FALSE(dateTime > timeSpec2);
    EXPECT_FALSE(dateTime > timeSpec3);
    EXPECT_FALSE(dateTime > timeSpec4);
    EXPECT_FALSE(dateTime > timeSpec5);
    EXPECT_FALSE(dateTime > timeSpec6);
    EXPECT_FALSE(dateTime > timeSpec7);
    EXPECT_FALSE(dateTime > timeSpec8);
}

TEST_FIXTURE(DateTimeTest, OperatorGreaterDateTimeTimeVal)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal1 = (timeval)DateTime(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal2 = (timeval)DateTime(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal3 = (timeval)DateTime(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    timeval timeVal4 = (timeval)DateTime(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    timeval timeVal5 = (timeval)DateTime(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    timeval timeVal6 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    timeval timeVal7 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    timeval timeVal8 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_FALSE(dateTime > timeVal1);
    EXPECT_FALSE(dateTime > timeVal2);
    EXPECT_FALSE(dateTime > timeVal3);
    EXPECT_FALSE(dateTime > timeVal4);
    EXPECT_FALSE(dateTime > timeVal5);
    EXPECT_FALSE(dateTime > timeVal6);
    EXPECT_FALSE(dateTime > timeVal7);
    EXPECT_FALSE(dateTime > timeVal8);
}

TEST_FIXTURE(DateTimeTest, OperatorLessEqualDateTimeDateTime)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    DateTime dateTime1(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    DateTime dateTime2(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    DateTime dateTime3(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    DateTime dateTime4(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    DateTime dateTime5(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    DateTime dateTime6(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    DateTime dateTime7(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    DateTime dateTime8(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_TRUE(dateTime <= dateTime1);
    EXPECT_TRUE(dateTime <= dateTime2);
    EXPECT_TRUE(dateTime <= dateTime3);
    EXPECT_TRUE(dateTime <= dateTime4);
    EXPECT_TRUE(dateTime <= dateTime5);
    EXPECT_TRUE(dateTime <= dateTime6);
    EXPECT_TRUE(dateTime <= dateTime7);
    EXPECT_TRUE(dateTime <= dateTime8);
}

TEST_FIXTURE(DateTimeTest, OperatorLessEqualTimeSpecDateTime)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec1 = (timespec)DateTime(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec2 = (timespec)DateTime(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec3 = (timespec)DateTime(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    timespec timeSpec4 = (timespec)DateTime(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    timespec timeSpec5 = (timespec)DateTime(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    timespec timeSpec6 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    timespec timeSpec7 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    timespec timeSpec8 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_FALSE(timeSpec1 <= dateTime);
    EXPECT_FALSE(timeSpec2 <= dateTime);
    EXPECT_FALSE(timeSpec3 <= dateTime);
    EXPECT_FALSE(timeSpec4 <= dateTime);
    EXPECT_FALSE(timeSpec5 <= dateTime);
    EXPECT_FALSE(timeSpec6 <= dateTime);
    EXPECT_FALSE(timeSpec7 <= dateTime);
    EXPECT_TRUE(timeSpec8 <= dateTime);
}

TEST_FIXTURE(DateTimeTest, OperatorLessEqualTimeValDateTime)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal1 = (timeval)DateTime(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal2 = (timeval)DateTime(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal3 = (timeval)DateTime(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    timeval timeVal4 = (timeval)DateTime(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    timeval timeVal5 = (timeval)DateTime(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    timeval timeVal6 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    timeval timeVal7 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    timeval timeVal8 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_FALSE(timeVal1 <= dateTime);
    EXPECT_FALSE(timeVal2 <= dateTime);
    EXPECT_FALSE(timeVal3 <= dateTime);
    EXPECT_FALSE(timeVal4 <= dateTime);
    EXPECT_FALSE(timeVal5 <= dateTime);
    EXPECT_FALSE(timeVal6 <= dateTime);
    EXPECT_FALSE(timeVal7 <= dateTime);
    EXPECT_TRUE(timeVal8 <= dateTime);
}

TEST_FIXTURE(DateTimeTest, OperatorLessEqualDateTimeTimeSpec)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec1 = (timespec)DateTime(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec2 = (timespec)DateTime(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec3 = (timespec)DateTime(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    timespec timeSpec4 = (timespec)DateTime(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    timespec timeSpec5 = (timespec)DateTime(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    timespec timeSpec6 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    timespec timeSpec7 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    timespec timeSpec8 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_TRUE(dateTime <= timeSpec1);
    EXPECT_TRUE(dateTime <= timeSpec2);
    EXPECT_TRUE(dateTime <= timeSpec3);
    EXPECT_TRUE(dateTime <= timeSpec4);
    EXPECT_TRUE(dateTime <= timeSpec5);
    EXPECT_TRUE(dateTime <= timeSpec6);
    EXPECT_TRUE(dateTime <= timeSpec7);
    EXPECT_TRUE(dateTime <= timeSpec8);
}

TEST_FIXTURE(DateTimeTest, OperatorLessEqualDateTimeTimeVal)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal1 = (timeval)DateTime(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal2 = (timeval)DateTime(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal3 = (timeval)DateTime(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    timeval timeVal4 = (timeval)DateTime(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    timeval timeVal5 = (timeval)DateTime(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    timeval timeVal6 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    timeval timeVal7 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    timeval timeVal8 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_TRUE(dateTime <= timeVal1);
    EXPECT_TRUE(dateTime <= timeVal2);
    EXPECT_TRUE(dateTime <= timeVal3);
    EXPECT_TRUE(dateTime <= timeVal4);
    EXPECT_TRUE(dateTime <= timeVal5);
    EXPECT_TRUE(dateTime <= timeVal6);
    EXPECT_TRUE(dateTime <= timeVal7);
    EXPECT_TRUE(dateTime <= timeVal8);
}

TEST_FIXTURE(DateTimeTest, OperatorGreaterEqualDateTimeDateTime)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    DateTime dateTime1(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    DateTime dateTime2(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    DateTime dateTime3(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    DateTime dateTime4(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    DateTime dateTime5(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    DateTime dateTime6(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    DateTime dateTime7(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    DateTime dateTime8(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_FALSE(dateTime >= dateTime1);
    EXPECT_FALSE(dateTime >= dateTime2);
    EXPECT_FALSE(dateTime >= dateTime3);
    EXPECT_FALSE(dateTime >= dateTime4);
    EXPECT_FALSE(dateTime >= dateTime5);
    EXPECT_FALSE(dateTime >= dateTime6);
    EXPECT_FALSE(dateTime >= dateTime7);
    EXPECT_TRUE(dateTime >= dateTime8);
}

TEST_FIXTURE(DateTimeTest, OperatorGreaterEqualTimeSpecDateTime)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec1 = (timespec)DateTime(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec2 = (timespec)DateTime(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec3 = (timespec)DateTime(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    timespec timeSpec4 = (timespec)DateTime(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    timespec timeSpec5 = (timespec)DateTime(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    timespec timeSpec6 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    timespec timeSpec7 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    timespec timeSpec8 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_TRUE(timeSpec1 >= dateTime);
    EXPECT_TRUE(timeSpec2 >= dateTime);
    EXPECT_TRUE(timeSpec3 >= dateTime);
    EXPECT_TRUE(timeSpec4 >= dateTime);
    EXPECT_TRUE(timeSpec5 >= dateTime);
    EXPECT_TRUE(timeSpec6 >= dateTime);
    EXPECT_TRUE(timeSpec7 >= dateTime);
    EXPECT_TRUE(timeSpec8 >= dateTime);
}

TEST_FIXTURE(DateTimeTest, OperatorGreaterEqualTimeValDateTime)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal1 = (timeval)DateTime(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal2 = (timeval)DateTime(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal3 = (timeval)DateTime(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    timeval timeVal4 = (timeval)DateTime(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    timeval timeVal5 = (timeval)DateTime(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    timeval timeVal6 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    timeval timeVal7 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    timeval timeVal8 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_TRUE(timeVal1 >= dateTime);
    EXPECT_TRUE(timeVal2 >= dateTime);
    EXPECT_TRUE(timeVal3 >= dateTime);
    EXPECT_TRUE(timeVal4 >= dateTime);
    EXPECT_TRUE(timeVal5 >= dateTime);
    EXPECT_TRUE(timeVal6 >= dateTime);
    EXPECT_TRUE(timeVal7 >= dateTime);
    EXPECT_TRUE(timeVal8 >= dateTime);
}

TEST_FIXTURE(DateTimeTest, OperatorGreaterEqualDateTimeTimeSpec)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec1 = (timespec)DateTime(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec2 = (timespec)DateTime(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    timespec timeSpec3 = (timespec)DateTime(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    timespec timeSpec4 = (timespec)DateTime(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    timespec timeSpec5 = (timespec)DateTime(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    timespec timeSpec6 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    timespec timeSpec7 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    timespec timeSpec8 = (timespec)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_FALSE(dateTime >= timeSpec1);
    EXPECT_FALSE(dateTime >= timeSpec2);
    EXPECT_FALSE(dateTime >= timeSpec3);
    EXPECT_FALSE(dateTime >= timeSpec4);
    EXPECT_FALSE(dateTime >= timeSpec5);
    EXPECT_FALSE(dateTime >= timeSpec6);
    EXPECT_FALSE(dateTime >= timeSpec7);
    EXPECT_TRUE(dateTime >= timeSpec8);
}

TEST_FIXTURE(DateTimeTest, OperatorGreaterEqualDateTimeTimeVal)
{
    int year1 = 2014;
    int month1 = 3;
    int day1 = 26;
    int hour1 = 12;
    int minute1 = 34;
    int second1 = 56;
    int microSeconds1 = 789;
    int year2 = 2016;
    int month2 = 4;
    int day2 = 30;
    int hour2 = 13;
    int minute2 = 35;
    int second2 = 57;
    int microSeconds2 = 789123;
    DateTime dateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal1 = (timeval)DateTime(year2, month1, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal2 = (timeval)DateTime(year1, month2, day1, hour1, minute1, second1, microSeconds1);
    timeval timeVal3 = (timeval)DateTime(year1, month1, day2, hour1, minute1, second1, microSeconds1);
    timeval timeVal4 = (timeval)DateTime(year1, month1, day1, hour2, minute1, second1, microSeconds1);
    timeval timeVal5 = (timeval)DateTime(year1, month1, day1, hour1, minute2, second1, microSeconds1);
    timeval timeVal6 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second2, microSeconds1);
    timeval timeVal7 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds2);
    timeval timeVal8 = (timeval)DateTime(year1, month1, day1, hour1, minute1, second1, microSeconds1);
    EXPECT_FALSE(dateTime >= timeVal1);
    EXPECT_FALSE(dateTime >= timeVal2);
    EXPECT_FALSE(dateTime >= timeVal3);
    EXPECT_FALSE(dateTime >= timeVal4);
    EXPECT_FALSE(dateTime >= timeVal5);
    EXPECT_FALSE(dateTime >= timeVal6);
    EXPECT_FALSE(dateTime >= timeVal7);
    EXPECT_TRUE(dateTime >= timeVal8);
}

} // namespace Test

} // namespace Core
