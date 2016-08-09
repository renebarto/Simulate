#include "unit-test-c++/UnitTestC++.h"

//#include <unistd.h>
#include "core/ByteArray.h"
#include "core/Util.h"
#include "core/TestData.h"

using namespace std;

namespace Core
{

namespace Test
{

class UtilTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void UtilTest::SetUp()
{
}

void UtilTest::TearDown()
{
}

TEST_FIXTURE(UtilTest, ConvertToTimeSpec)
{
    timespec expected = { 1000, 0 };
    timespec actual = Util::ConvertToTimeSpec(1000000);
    EXPECT_EQ(expected.tv_sec, actual.tv_sec);
    EXPECT_EQ(expected.tv_nsec, actual.tv_nsec);

    expected = timespec { 1, 0 };
    actual = Util::ConvertToTimeSpec(1000);
    EXPECT_EQ(expected.tv_sec, actual.tv_sec);
    EXPECT_EQ(expected.tv_nsec, actual.tv_nsec);

    expected = timespec { 0, 1000000 };
    actual = Util::ConvertToTimeSpec(1);
    EXPECT_EQ(expected.tv_sec, actual.tv_sec);
    EXPECT_EQ(expected.tv_nsec, actual.tv_nsec);
}

TEST_FIXTURE(UtilTest, ConvertDurationToTimeSpecMilliSeconds)
{
    timespec expected = { 1000, 0 };
    timespec actual = Util::ConvertDurationToTimeSpec(std::chrono::milliseconds(1000000));
    EXPECT_EQ(expected.tv_sec, actual.tv_sec);
    EXPECT_EQ(expected.tv_nsec, actual.tv_nsec);

    expected = timespec { 1, 0 };
    actual = Util::ConvertDurationToTimeSpec(std::chrono::milliseconds(1000));
    EXPECT_EQ(expected.tv_sec, actual.tv_sec);
    EXPECT_EQ(expected.tv_nsec, actual.tv_nsec);

    expected = timespec { 0, 1000000 };
    actual = Util::ConvertDurationToTimeSpec(std::chrono::milliseconds(1));
    EXPECT_EQ(expected.tv_sec, actual.tv_sec);
    EXPECT_EQ(expected.tv_nsec, actual.tv_nsec);
}

TEST_FIXTURE(UtilTest, ConvertDurationToTimeSpecMicroSeconds)
{
    timespec expected = { 1, 0 };
    timespec actual = Util::ConvertDurationToTimeSpec(std::chrono::microseconds(1000000));
    EXPECT_EQ(expected.tv_sec, actual.tv_sec);
    EXPECT_EQ(expected.tv_nsec, actual.tv_nsec);

    expected = timespec { 0, 1000000 };
    actual = Util::ConvertDurationToTimeSpec(std::chrono::microseconds(1000));
    EXPECT_EQ(expected.tv_sec, actual.tv_sec);
    EXPECT_EQ(expected.tv_nsec, actual.tv_nsec);

    expected = timespec { 0, 1000 };
    actual = Util::ConvertDurationToTimeSpec(std::chrono::microseconds(1));
    EXPECT_EQ(expected.tv_sec, actual.tv_sec);
    EXPECT_EQ(expected.tv_nsec, actual.tv_nsec);
}

TEST_FIXTURE(UtilTest, ConvertDurationToTimeSpecNanoSeconds)
{
    timespec expected = { 1, 0 };
    timespec actual = Util::ConvertDurationToTimeSpec(std::chrono::nanoseconds(1000000000));
    EXPECT_EQ(expected.tv_sec, actual.tv_sec);
    EXPECT_EQ(expected.tv_nsec, actual.tv_nsec);

    expected = timespec { 0, 1000000 };
    actual = Util::ConvertDurationToTimeSpec(std::chrono::nanoseconds(1000000));
    EXPECT_EQ(expected.tv_sec, actual.tv_sec);
    EXPECT_EQ(expected.tv_nsec, actual.tv_nsec);

    expected = timespec { 0, 1000 };
    actual = Util::ConvertDurationToTimeSpec(std::chrono::nanoseconds(1000));
    EXPECT_EQ(expected.tv_sec, actual.tv_sec);
    EXPECT_EQ(expected.tv_nsec, actual.tv_nsec);

    expected = timespec { 0, 1 };
    actual = Util::ConvertDurationToTimeSpec(std::chrono::nanoseconds(1));
    EXPECT_EQ(expected.tv_sec, actual.tv_sec);
    EXPECT_EQ(expected.tv_nsec, actual.tv_nsec);
}

TEST_FIXTURE(UtilTest, Clip)
{
    int min = 1;
    int max = 10;
    EXPECT_EQ(1, Util::Clip(0, min, max));
    EXPECT_EQ(1, Util::Clip(1, min, max));
    EXPECT_EQ(5, Util::Clip(5, min, max));
    EXPECT_EQ(10, Util::Clip(10, min, max));
    EXPECT_EQ(10, Util::Clip(11, min, max));
}

TEST_FIXTURE(UtilTest, Base64Encode)
{
    ByteArray data({0, 1, 2, 3, 4, 5, 6, 7});
    string expected = "AAECAwQFBgc=";
    EXPECT_EQ(expected, Util::Base64Encode(data));
}

TEST_FIXTURE(UtilTest, Base64Decode)
{
    ByteArray expected({0, 1, 2, 3, 4, 5, 6, 7});
    string base64 = Util::Base64Encode(expected);
    EXPECT_EQ(expected, Util::Base64Decode(base64));
}

TEST_FIXTURE(UtilTest, TryParseBool)
{
    bool expected = true;
    bool actual;
    EXPECT_TRUE(Util::TryParse("true", actual));
    EXPECT_EQ(expected, actual);

    expected = false;
    EXPECT_TRUE(Util::TryParse("FaLsE", actual));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Util::TryParse("abcd", actual));
    EXPECT_FALSE(Util::TryParse("1234", actual));
}

TEST_FIXTURE(UtilTest, TryParseUInt8)
{
    uint8_t expected = 254;
    uint8_t actual;
    EXPECT_TRUE(Util::TryParse("254", actual));
    EXPECT_EQ(expected, actual);

    expected = 0xab;
    EXPECT_TRUE(Util::TryParse("aB", actual, 16));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Util::TryParse("xyz", actual));
    EXPECT_FALSE(Util::TryParse("1234", actual));
}

TEST_FIXTURE(UtilTest, TryParseInt16)
{
    int16_t expected = 12345;
    int16_t actual;
    EXPECT_TRUE(Util::TryParse("12345", actual));
    EXPECT_EQ(expected, actual);

    expected = int16_t(0xabcd);
    EXPECT_TRUE(Util::TryParse("aBcD", actual, 16));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Util::TryParse("xyz", actual));
    EXPECT_FALSE(Util::TryParse("54321", actual));
}

TEST_FIXTURE(UtilTest, TryParseUInt16)
{
    uint16_t expected = 54321;
    uint16_t actual;
    EXPECT_TRUE(Util::TryParse("54321", actual));
    EXPECT_EQ(expected, actual);

    expected = 0xabcd;
    EXPECT_TRUE(Util::TryParse("aBcD", actual, 16));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Util::TryParse("xyz", actual));
    EXPECT_FALSE(Util::TryParse("654321", actual));
}

TEST_FIXTURE(UtilTest, TryParseInt32)
{
    int32_t expected = 12345678;
    int32_t actual;
    EXPECT_TRUE(Util::TryParse("12345678", actual));
    EXPECT_EQ(expected, actual);

    expected = (int32_t)0xabcdefef;
    EXPECT_TRUE(Util::TryParse("aBcDEfeF", actual, 16));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Util::TryParse("xyz", actual));
    EXPECT_FALSE(Util::TryParse("FFFFFFFFF", actual));
}

TEST_FIXTURE(UtilTest, TryParseUInt32)
{
    uint32_t expected = 4000000000u;
    uint32_t actual;
    EXPECT_TRUE(Util::TryParse("4000000000", actual));
    EXPECT_EQ(expected, actual);

    expected = 0xabcdefef;
    EXPECT_TRUE(Util::TryParse("aBcDeFeF", actual, 16));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Util::TryParse("xyz", actual));
    EXPECT_FALSE(Util::TryParse("FFFFFFFFF", actual));
}

TEST_FIXTURE(UtilTest, TryParseInt64)
{
    int64_t expected = 987654321;
    int64_t actual;
    EXPECT_TRUE(Util::TryParse("987654321", actual));
    EXPECT_EQ(expected, actual);

    expected = 0xfedcba9876543210;
    EXPECT_TRUE(Util::TryParse("fEdCbA9876543210", actual, 16));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Util::TryParse("xyz", actual));
    EXPECT_FALSE(Util::TryParse("FFFFFFFFFFFFFFFFF", actual));
}

TEST_FIXTURE(UtilTest, TryParseUInt64)
{
    uint64_t expected = 9223372036854775808ULL;
    uint64_t actual;
    EXPECT_TRUE(Util::TryParse("9223372036854775808", actual));
    EXPECT_EQ(expected, actual);

    expected = 0xfedcba9876543210;
    EXPECT_TRUE(Util::TryParse("fEdCbA9876543210", actual, 16));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Util::TryParse("xyz", actual));
    EXPECT_FALSE(Util::TryParse("FFFFFFFFFFFFFFFFF", actual));
}

TEST_FIXTURE(UtilTest, TryParseFloat)
{
    float expected = 1002;
    float actual;
    EXPECT_TRUE(Util::TryParse("1.002E+03", actual));
    EXPECT_EQ(expected, actual);

    expected = 5.0E-37F;
    EXPECT_TRUE(Util::TryParse("5e-37", actual));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Util::TryParse("xyz", actual));
}

TEST_FIXTURE(UtilTest, TryParseDouble)
{
    double expected = 1000002;
    double actual;
    EXPECT_TRUE(Util::TryParse("1.000002E+06", actual));
    EXPECT_EQ(expected, actual);

    expected = 1e-200;
    EXPECT_TRUE(Util::TryParse("1e-200", actual));
    EXPECT_EQ(expected, actual);

    EXPECT_FALSE(Util::TryParse("xyz", actual));
}

TEST_FIXTURE(UtilTest, ToStringBool)
{
    bool value = true;
    EXPECT_EQ("true", Util::ToString(value));
    value = false;
    EXPECT_EQ("false", Util::ToString(value));
}

TEST_FIXTURE(UtilTest, ToStringUInt8)
{
    uint8_t value = 0xAA;
    EXPECT_EQ("10101010", Util::ToString(value, 2));
    EXPECT_EQ("252", Util::ToString(value, 8));
    EXPECT_EQ("170", Util::ToString(value, 10));
    EXPECT_EQ("AA", Util::ToString(value, 16));
}

TEST_FIXTURE(UtilTest, ToStringInt16)
{
    int16_t value = 0x5555;
    EXPECT_EQ("0101010101010101", Util::ToString(value, 2));
    EXPECT_EQ("052525", Util::ToString(value, 8));
    EXPECT_EQ("21845", Util::ToString(value, 10));
    EXPECT_EQ("5555", Util::ToString(value, 16));
}

TEST_FIXTURE(UtilTest, ToStringUInt16)
{
    uint16_t value = 0xAAAA;
    EXPECT_EQ("1010101010101010", Util::ToString(value, 2));
    EXPECT_EQ("125252", Util::ToString(value, 8));
    EXPECT_EQ("43690", Util::ToString(value, 10));
    EXPECT_EQ("AAAA", Util::ToString(value, 16));
}

TEST_FIXTURE(UtilTest, ToStringInt32)
{
    int32_t value = 0x55555555;
    EXPECT_EQ("01010101010101010101010101010101", Util::ToString(value, 2));
    EXPECT_EQ("12525252525", Util::ToString(value, 8));
    EXPECT_EQ("1431655765", Util::ToString(value, 10));
    EXPECT_EQ("55555555", Util::ToString(value, 16));
}

TEST_FIXTURE(UtilTest, ToStringUInt32)
{
    uint32_t value = 0xAAAAAAAA;
    EXPECT_EQ("10101010101010101010101010101010", Util::ToString(value, 2));
    EXPECT_EQ("25252525252", Util::ToString(value, 8));
    EXPECT_EQ("2863311530", Util::ToString(value, 10));
    EXPECT_EQ("AAAAAAAA", Util::ToString(value, 16));
}

TEST_FIXTURE(UtilTest, ToStringInt64)
{
    int64_t value = 0x5555555555555555;
    EXPECT_EQ("0101010101010101010101010101010101010101010101010101010101010101", Util::ToString(value, 2));
    EXPECT_EQ("0525252525252525252525", Util::ToString(value, 8));
    EXPECT_EQ("6148914691236517205", Util::ToString(value, 10));
    EXPECT_EQ("5555555555555555", Util::ToString(value, 16));
}

TEST_FIXTURE(UtilTest, ToStringUInt64)
{
    uint64_t value = 0xAAAAAAAAAAAAAAAA;
    EXPECT_EQ("1010101010101010101010101010101010101010101010101010101010101010", Util::ToString(value, 2));
    EXPECT_EQ("1252525252525252525252", Util::ToString(value, 8));
    EXPECT_EQ("12297829382473034410", Util::ToString(value, 10));
    EXPECT_EQ("AAAAAAAAAAAAAAAA", Util::ToString(value, 16));
}

TEST_FIXTURE(UtilTest, ToStringFloat)
{
    float value = 1.0002e4F;
    EXPECT_EQ("10002", Util::ToString(value));
    value = 5.0E-37F;
    EXPECT_EQ("5.000000179695649e-37", Util::ToString(value));
    EXPECT_EQ("5e-37", Util::ToString(value, 1));
}

TEST_FIXTURE(UtilTest, ToStringDouble)
{
    double value = 1.00002E05;
    EXPECT_EQ("100002", Util::ToString(value));
    value = 1e-200;
    EXPECT_EQ("1e-200", Util::ToString(value));
    value = 1.001e-200;
    EXPECT_EQ("1.001e-200", Util::ToString(value));
    EXPECT_EQ("1e-200", Util::ToString(value, 2));
}

TEST_FIXTURE(UtilTest, ToStringString)
{
    string value = "a";
    EXPECT_EQ("\"a\"", Util::ToString(value));
    EXPECT_EQ("a", Util::ToString(value, false));
}

TEST_FIXTURE(UtilTest, NextPowerOfTwo)
{
    EXPECT_EQ((int)1, Util::NextPowerOfTwo(0));
    EXPECT_EQ((int)1, Util::NextPowerOfTwo(1));
    EXPECT_EQ((int)2, Util::NextPowerOfTwo(2));
    EXPECT_EQ((int)4, Util::NextPowerOfTwo(3));
    EXPECT_EQ((int)4, Util::NextPowerOfTwo(4));
    EXPECT_EQ((int)8, Util::NextPowerOfTwo(5));
    EXPECT_EQ((int)8, Util::NextPowerOfTwo(6));
    EXPECT_EQ((int)8, Util::NextPowerOfTwo(7));
    EXPECT_EQ((int)8, Util::NextPowerOfTwo(8));
    EXPECT_EQ((int)16, Util::NextPowerOfTwo(9));
    EXPECT_EQ((int)16, Util::NextPowerOfTwo(10));
}

} // namespace Test

} // namespace Core
