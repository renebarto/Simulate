#include <unit-test-c++/UnitTestC++.h>

using namespace UnitTestCpp;

TEST_SUITE(PrintValue)
{

// We are now in namespace Suite##PrintValue

class PrintValueTest : public UnitTestCpp::TestFixture
{
public:
    void SetUp()
    {

    }
    void TearDown()
    {

    }
};

TEST_FIXTURE(PrintValueTest, PrintToStringSimple)
{
    EXPECT_EQ("true", PrintToString(true));
    EXPECT_EQ("'A' (65, 0x41)", PrintToString((int8_t)65));
    EXPECT_EQ("12345", PrintToString((int16_t)12345));
    EXPECT_EQ("12345678", PrintToString((int32_t)12345678));
    EXPECT_EQ("1234567890", PrintToString((int64_t)1234567890));
    EXPECT_EQ("'A' (65, 0x41)", PrintToString((uint8_t)65));
    EXPECT_EQ("12345", PrintToString((uint16_t)12345));
    EXPECT_EQ("12345678", PrintToString((uint32_t)12345678));
    EXPECT_EQ("1234567890", PrintToString((uint64_t)1234567890));
    EXPECT_EQ("'A' (65, 0x41)", PrintToString((char)65));
    EXPECT_EQ("12345", PrintToString((short)12345));
    EXPECT_EQ("12345678", PrintToString((int)12345678));
    EXPECT_EQ("12345678", PrintToString((long)12345678));
    EXPECT_EQ("1234567890", PrintToString((long long)1234567890));
    EXPECT_EQ("'A' (65, 0x41)", PrintToString((unsigned char)65));
    EXPECT_EQ("12345", PrintToString((unsigned short)12345));
    EXPECT_EQ("12345678", PrintToString((unsigned int)12345678));
    EXPECT_EQ("12345678", PrintToString((unsigned long)12345678));
    EXPECT_EQ("1234567890", PrintToString((unsigned long long)1234567890));
}

TEST_FIXTURE(PrintValueTest, PrintToStringString)
{
    EXPECT_EQ("\"ABCD\"", PrintToString("ABCD"));
    EXPECT_EQ("\"ABCD\"", PrintToString(string("ABCD")));
#if defined(__GNUC__)
	EXPECT_EQ("L\"\\x4F60\\x597D\"", PrintToString(L"你好"));
    EXPECT_EQ("L\"\\x4F60\\x597D\"", PrintToString(wstring(L"你好")));
#elif defined(_MSC_VER)
	EXPECT_EQ("L\"\\xE4\\xBD\\xA0\\xE5\\xA5\\xBD\"", PrintToString(L"你好"));
	EXPECT_EQ("L\"\\xE4\\xBD\\xA0\\xE5\\xA5\\xBD\"", PrintToString(wstring(L"你好")));
#endif
}

TEST_FIXTURE(PrintValueTest, PrintToStringArray)
{
    uint8_t chararray[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    EXPECT_EQ("{ '\\0', '\\x1' (1), '\\x2' (2), '\\x3' (3), "
              "'\\x4' (4), '\\x5' (5), '\\x6' (6), '\\a' (7), "
              "'\\b' (8), '\\t' (9), '\\n' (10, 0xA), '\\v' (11, 0xB), "
              "'\\f' (12, 0xC), '\\r' (13, 0xD), '\\xE' (14), '\\xF' (15) }", PrintToString(chararray));
    uint32_t intarray[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    EXPECT_EQ("{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }", PrintToString(intarray));
}

enum EnumYesNo
{
    Yes, No
};

enum class EnumClassYesNo
{
    Yes, No
};

enum  EnumClassYesNoWithStreamOperator
{
    YesYes, NoNo
};

ostream & operator << (ostream & os, EnumClassYesNoWithStreamOperator value)
{
    switch (value)
    {
    case EnumClassYesNoWithStreamOperator::YesYes:
        os << "Yes";
        break;
    case EnumClassYesNoWithStreamOperator::NoNo:
        os << "No";
        break;
    }
    return os;
}

TEST_FIXTURE(PrintValueTest, PrintToStringEnum)
{
    EnumYesNo valueEnumYes = Yes;
    EnumYesNo valueEnumNo = No;
    EXPECT_EQ("0", PrintToString(valueEnumYes));
    EXPECT_EQ("1", PrintToString(valueEnumNo));
    EnumClassYesNo valueEnumClassYes = EnumClassYesNo::Yes;
    EnumClassYesNo valueEnumClassNo = EnumClassYesNo::No;
    EXPECT_EQ("0", PrintToString(valueEnumClassYes));
    EXPECT_EQ("1", PrintToString(valueEnumClassNo));
    EnumClassYesNoWithStreamOperator valueEnumClassYesWithStream = EnumClassYesNoWithStreamOperator::YesYes;
    EnumClassYesNoWithStreamOperator valueEnumClassNoWithStream = EnumClassYesNoWithStreamOperator::NoNo;
    EXPECT_EQ("Yes", PrintToString(valueEnumClassYesWithStream));
    EXPECT_EQ("No", PrintToString(valueEnumClassNoWithStream));
}

TEST_FIXTURE(PrintValueTest, PrintToStringEnumPtr)
{
    EnumYesNo valueEnumYes = Yes;
    EnumYesNo valueEnumNo = No;
    ASSERT_NE("0", PrintToString(&valueEnumYes));
    ASSERT_NE("1", PrintToString(&valueEnumNo));
    EnumClassYesNo valueEnumClassYes = EnumClassYesNo::Yes;
    EnumClassYesNo valueEnumClassNo = EnumClassYesNo::No;
    ASSERT_NE("0", PrintToString(&valueEnumClassYes));
    ASSERT_NE("1", PrintToString(&valueEnumClassNo));
    EnumClassYesNoWithStreamOperator valueEnumClassYesWithStream = EnumClassYesNoWithStreamOperator::YesYes;
    EnumClassYesNoWithStreamOperator valueEnumClassNoWithStream = EnumClassYesNoWithStreamOperator::NoNo;
    ASSERT_NE("Yes", PrintToString(&valueEnumClassYesWithStream));
    ASSERT_NE("No", PrintToString(&valueEnumClassNoWithStream));
}

class MyClass
{
public:
    MyClass(int i): value(i) {}
    int value;
};

class MyClassWithStreamOperator
{
public:
    MyClassWithStreamOperator(int i): value(i) {}
    int value;
};

ostream & operator << (ostream & os, const MyClassWithStreamOperator & x)
{
    os << x.value;
    return os;
}

#if defined(_MSC_VER)
const std::string expectedName = "class SuitePrintValue::MyClass";
#elif defined(__GNUC__)
const std::string expectedName = "SuitePrintValue::MyClass";
#endif

TEST_FIXTURE(PrintValueTest, PrintToStringClass)
{
    MyClass value(1);
    EXPECT_EQ(expectedName, PrintToString(value));
    MyClassWithStreamOperator valueWithStream(2);
    EXPECT_EQ("2", PrintToString(valueWithStream));
}

TEST_FIXTURE(PrintValueTest, PrintToStringClassPtr)
{
    MyClass value(1);
    ASSERT_NE(expectedName, PrintToString(&value));
    MyClassWithStreamOperator valueWithStream(2);
    ASSERT_NE("2", PrintToString(&valueWithStream));
}

} // TEST_SUITE(PrintValue)
