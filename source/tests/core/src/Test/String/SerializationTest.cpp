#include "unit-test-c++/UnitTestC++.h"
#include <limits>
#include <sstream>
#include <typeinfo>
#include "core/String/Deserialize.h"
#include "core/String/Serialize.h"

namespace Core
{
namespace Test
{

class StringSerializationTest
    : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp() {}
    virtual void TearDown() {}

    template<typename T>
    void ArithmeticFixture()
    {
        EXPECT_EQ(std::numeric_limits<T>::max(),
                  String::Deserialize<T>(
                      String::Serialize<T>(
                          std::numeric_limits<T>::max())));

        EXPECT_EQ(std::numeric_limits<T>::min(),
                  String::Deserialize<T>(
                      String::Serialize<T>(
                          std::numeric_limits<T>::min())));

        EXPECT_EQ(std::numeric_limits<T>::lowest(),
                  String::Deserialize<T>(
                      String::Serialize<T>(
                          std::numeric_limits<T>::lowest())));

        EXPECT_EQ(std::numeric_limits<T>::epsilon(),
                  String::Deserialize<T>(
                      String::Serialize<T>(
                          std::numeric_limits<T>::epsilon())));
    }
};

TEST_FIXTURE(StringSerializationTest, ArithmeticTypes)
{
    this->ArithmeticFixture<char>();
    this->ArithmeticFixture<signed char>();
    this->ArithmeticFixture<unsigned char>();
    this->ArithmeticFixture<short>();
    this->ArithmeticFixture<unsigned short>();
    this->ArithmeticFixture<int>();
    this->ArithmeticFixture<unsigned int>();
    this->ArithmeticFixture<long>();
    this->ArithmeticFixture<unsigned long>();
    this->ArithmeticFixture<long long>();
    this->ArithmeticFixture<unsigned long long>();
    this->ArithmeticFixture<float>();
    this->ArithmeticFixture<double>();
    this->ArithmeticFixture<long double>();
}

TEST_FIXTURE(StringSerializationTest, Bool)
{
    EXPECT_TRUE(String::Deserialize<bool>(String::Serialize<bool>(true)));
    EXPECT_FALSE(String::Deserialize<bool>(String::Serialize<bool>(false)));
}

} // namespace Test
} // namespace Core
