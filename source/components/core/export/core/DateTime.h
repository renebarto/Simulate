#pragma once

#include <ctime>
#include "core/TimeSpan.h"

namespace Core
{

enum class MonthType
{
    January = 1,
    February = 2,
    March = 3,
    April = 4,
    May = 5,
    June = 6,
    July = 7,
    August = 8,
    September = 9,
    October = 10,
    November = 11,
    December = 12,
};

enum class WeekDayType
{
    Monday = 1,
    Tuesday = 2,
    Wednesday = 3,
    Thursday = 4,
    Friday = 5,
    Saturday = 6,
    Sunday = 7,
};

class DateTime
{
public:
    static const int64_t MicroSecondsPerSecond;
    static const int64_t NanoSecondsPerMicroSecond;
    static const int64_t NanoSecondsPerSecond;

    DateTime();
    DateTime(const DateTime & other);
    DateTime(int year, int month, int day, int hour, int minute, double second);
    DateTime(int year, int month, int day, int hour, int minute, int second,
             int microSeconds = 0);
    DateTime(int year, MonthType month, int day, int hour, int minute, double second);
    DateTime(int year, MonthType month, int day, int hour, int minute, int second,
             int microSeconds = 0);
    DateTime(time_t epochTime, bool localTime = false);
    DateTime(timespec timeSpec, bool localTime = false);
    DateTime(timeval timeVal, bool localTime = false);
    virtual ~DateTime();

    DateTime & operator = (const DateTime & other);
    DateTime & operator = (time_t epochTime);
    DateTime & operator = (timespec timeSpec);
    DateTime & operator = (timeval timeVal);

    operator time_t() const;
    operator timespec() const;
    operator timeval() const;

    DateTime & operator += (const TimeSpan & timeSpan);
    DateTime & operator -= (const TimeSpan & timeSpan);

    bool Equals(const DateTime & other) const;
    bool LessThan(const DateTime & other) const;

    static DateTime NowUTC();
    static DateTime NowLocal();
    static DateTime CreateUTC(int year, int month, int day, int hour, int minute, double second);
    static DateTime CreateUTC(int year, int month, int day, int hour, int minute, int second,
                              int microSeconds = 0);
    static DateTime CreateUTC(int year, MonthType month, int day, int hour, int minute,
                              double second);
    static DateTime CreateUTC(int year, MonthType month, int day, int hour, int minute, int second,
                              int microSeconds = 0);
    static DateTime CreateLocal(int year, int month, int day, int hour, int minute, double second);
    static DateTime CreateLocal(int year, int month, int day, int hour, int minute, int second,
                                int microSeconds = 0);
    static DateTime CreateLocal(int year, MonthType month, int day, int hour, int minute,
                                double second);
    static DateTime CreateLocal(int year, MonthType month, int day, int hour, int minute,
                                int second, int microSeconds = 0);

    int Hour() const;
    int Minute() const;
    int Second() const;
    int MicroSeconds() const;

    int Year() const;
    int Month() const;
    MonthType MonthName() const;
    int MonthDay() const;
    int YearDay() const;
    WeekDayType WeekDay() const;
    int WeekNumberingYear() const;
    int WeekOfYear() const;

    TimeSpan OffsetFromUTC() const;
    std::string TimeZoneName() const;
    bool IsDaylightSavings() const;
    DateTime ConvertToLocalTime() const;
    DateTime ConvertToUTCTime() const;

    void PrintTo(std::ostream & stream) const;
    void PrintTo(std::ostream & stream, std::string const & formatString) const;

    friend DateTime operator + (const TimeSpan & lhs, const DateTime & rhs);
    friend DateTime operator + (const DateTime & lhs, const TimeSpan & rhs);
    friend DateTime operator - (const DateTime & lhs, const TimeSpan & rhs);
    friend TimeSpan operator - (const DateTime & lhs, const DateTime & rhs);

    friend bool operator == (const DateTime & lhs, const DateTime & rhs);
    friend bool operator == (timespec lhs, const DateTime & rhs);
    friend bool operator == (timeval lhs, const DateTime & rhs);
    friend bool operator == (const DateTime & lhs, timespec rhs);
    friend bool operator == (const DateTime & lhs, timeval rhs);
    friend bool operator != (const DateTime & lhs, const DateTime & rhs);
    friend bool operator != (timespec lhs, const DateTime & rhs);
    friend bool operator != (timeval lhs, const DateTime & rhs);
    friend bool operator != (const DateTime & lhs, timespec rhs);
    friend bool operator != (const DateTime & lhs, timeval rhs);
    friend bool operator < (const DateTime & lhs, const DateTime & rhs);
    friend bool operator < (timespec lhs, const DateTime & rhs);
    friend bool operator < (timeval lhs, const DateTime & rhs);
    friend bool operator < (const DateTime & lhs, timespec rhs);
    friend bool operator < (const DateTime & lhs, timeval rhs);
    friend bool operator > (const DateTime & lhs, const DateTime & rhs);
    friend bool operator > (timespec lhs, const DateTime & rhs);
    friend bool operator > (timeval lhs, const DateTime & rhs);
    friend bool operator > (const DateTime & lhs, timespec rhs);
    friend bool operator > (const DateTime & lhs, timeval rhs);
    friend bool operator <= (const DateTime & lhs, const DateTime & rhs);
    friend bool operator <= (timespec lhs, const DateTime & rhs);
    friend bool operator <= (timeval lhs, const DateTime & rhs);
    friend bool operator <= (const DateTime & lhs, timespec rhs);
    friend bool operator <= (const DateTime & lhs, timeval rhs);
    friend bool operator >= (const DateTime & lhs, const DateTime & rhs);
    friend bool operator >= (timespec lhs, const DateTime & rhs);
    friend bool operator >= (timeval lhs, const DateTime & rhs);
    friend bool operator >= (const DateTime & lhs, timespec rhs);
    friend bool operator >= (const DateTime & lhs, timeval rhs);

private:
    timespec time;
    tm dateTime;
	bool isLocalTime;
    void Assign(timespec value, bool localTime);
    void Assign(timeval value, bool localTime);
    void Assign(time_t value, bool localTime);
    void Assign(int year, int month, int day, int hour, int minute, double second, bool localTime);
    void Assign(const tm & value);
	int DiffFromUTC() const;
};

inline void PrintTo(DateTime const & dateTime, std::ostream & stream)
{
    dateTime.PrintTo(stream);
}

inline std::ostream & operator << (std::ostream & stream, DateTime const & dateTime)
{
    dateTime.PrintTo(stream);
    return stream;
}

} // namespace Core
