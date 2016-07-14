#include "unit-test-c++/UnitTestC++.h"

#include "core/FixedArray.h"

namespace Core
{

namespace Test
{

class FixedArrayTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();
};

void FixedArrayTest::SetUp()
{

}

void FixedArrayTest::TearDown()
{

}

TEST_FIXTURE(FixedArrayTest, ConstructorSize)
{
    uint8_t ref[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    size_t size = sizeof(ref) / sizeof(uint8_t);
    FixedArray<uint8_t> valueArray(size);
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), ref, size * sizeof(uint8_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, ConstructorDataLength)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(uint8_t);
    FixedArray<uint8_t> valueArray(data, size);
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), data, size * sizeof(uint8_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, CopyConstructor)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(uint8_t);
    FixedArray<uint8_t> valueArray(data, size);
    FixedArray<uint8_t> target(valueArray);
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), data, size * sizeof(uint8_t)) == 0);
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(target.Data(), data, size * sizeof(uint8_t)) == 0);
    EXPECT_NE(target.Data(), valueArray.Data());
}

TEST_FIXTURE(FixedArrayTest, MoveConstructor)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(uint8_t);
    FixedArray<uint8_t> valueArray(data, size);
    FixedArray<uint8_t> target(std::move(valueArray));
    EXPECT_EQ(size_t(0), valueArray.Size());
    EXPECT_NULL(valueArray.Data());
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(target.Data(), data, size * sizeof(uint8_t)) == 0);

}

TEST_FIXTURE(FixedArrayTest, ConstructorInitializer)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(uint8_t);
    FixedArray<uint8_t> valueArray({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), data, size * sizeof(uint8_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, CastToElementPointer)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(uint8_t);
    FixedArray<uint8_t> valueArray({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp((const uint8_t *)valueArray, data, size * sizeof(uint8_t)) == 0);
    EXPECT_TRUE(memcmp(static_cast<const uint8_t *>(valueArray), data, size * sizeof(uint8_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, OperatorAssign)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(uint8_t);
    FixedArray<uint8_t> valueArray(data, size);
    FixedArray<uint8_t> target(0);
    target = valueArray;
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), data, size * sizeof(uint8_t)) == 0);
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(target.Data(), data, size * sizeof(uint8_t)) == 0);
    EXPECT_NE(target.Data(), valueArray.Data());
}

TEST_FIXTURE(FixedArrayTest, OperatorMove)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(uint8_t);
    FixedArray<uint8_t> valueArray(data, size);
    FixedArray<uint8_t> target(0);
    target = std::move(valueArray);
    EXPECT_EQ(size_t(0), valueArray.Size());
    EXPECT_NULL(valueArray.Data());
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(target.Data(), data, size * sizeof(uint8_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, Clear)
{
    uint8_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(uint8_t);
    FixedArray<uint8_t> target(data, size);
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(data, target.Data(), size) == 0);
    target.Clear();
    EXPECT_EQ(size, target.Size());
}

TEST_FIXTURE(FixedArrayTest, SetInitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    uint8_t values[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    uint8_t data[16];
    size_t size = sizeof(data) / sizeof(uint8_t);
    FixedArray<uint8_t> valueArray(data, size);
    valueArray.Set(0, 0x01);
    valueArray.Set(1, 0x02);
    valueArray.Set(2, 0x03);
    valueArray.Set(3, 0x04);
    valueArray.Set(4, 0x05);
    valueArray.Set(5, 0x06);
    valueArray.Set(6, 0x07);
    valueArray.Set(7, 0x08);
    valueArray.Set(8, 0x09);
    valueArray.Set(9, 0x0A);
    valueArray.Set(10, 0x0B);
    valueArray.Set(11, 0x0C);
    valueArray.Set(12, values, sizeof(values) / sizeof(uint8_t));
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), ref, size * sizeof(uint8_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, Get)
{
    uint8_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    FixedArray<uint8_t> valueArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(uint8_t);
    EXPECT_EQ((uint8_t)0x01, valueArray.Get(0));
    EXPECT_EQ((uint8_t)0x02, valueArray.Get(1));
    EXPECT_EQ((uint8_t)0x03, valueArray.Get(2));
    EXPECT_EQ((uint8_t)0x04, valueArray.Get(3));
    EXPECT_EQ((uint8_t)0x05, valueArray.Get(4));
    EXPECT_EQ((uint8_t)0x06, valueArray.Get(5));
    EXPECT_EQ((uint8_t)0x07, valueArray.Get(6));
    EXPECT_EQ((uint8_t)0x08, valueArray.Get(7));
    EXPECT_EQ((uint8_t)0x09, valueArray.Get(8));
    EXPECT_EQ((uint8_t)0x0A, valueArray.Get(9));
    EXPECT_EQ((uint8_t)0x0B, valueArray.Get(10));
    EXPECT_EQ((uint8_t)0x0C, valueArray.Get(11));
    FixedArray<uint8_t> values = valueArray.Get(12, 4);
    EXPECT_EQ(size, values.Size());
    EXPECT_TRUE(memcmp(values.Data(), ref, size * sizeof(uint8_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, GetPtr)
{
    uint8_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    FixedArray<uint8_t> valueArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    const size_t size = sizeof(ref) / sizeof(uint8_t);

    uint8_t data[size];
    EXPECT_EQ(size, valueArray.Get(12, data, size));
    EXPECT_TRUE(memcmp(ref, data, size * sizeof(uint8_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, GetFixedArrayByRef)
{
    uint8_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    FixedArray<uint8_t> valueArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(uint8_t);

    FixedArray<uint8_t> data(0);
    EXPECT_EQ(size, valueArray.Get(12, data, size));
    EXPECT_EQ(size, data.Size());
    EXPECT_TRUE(memcmp(ref, data.Data(), size) == 0);
}

TEST_FIXTURE(FixedArrayTest, GetFixedArray)
{
    uint8_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    FixedArray<uint8_t> valueArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(uint8_t);
    FixedArray<uint8_t> values = valueArray.Get(12, size);
    EXPECT_EQ(size, values.Size());
    EXPECT_TRUE(memcmp(values.Data(), ref, size * sizeof(uint8_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, SetUninitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    uint8_t values[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref) / sizeof(uint8_t);
    FixedArray<uint8_t> valueArray(size);
    valueArray.Set(0, 0x01);
    valueArray.Set(1, 0x02);
    valueArray.Set(2, 0x03);
    valueArray.Set(3, 0x04);
    valueArray.Set(4, 0x05);
    valueArray.Set(5, 0x06);
    valueArray.Set(6, 0x07);
    valueArray.Set(7, 0x08);
    valueArray.Set(8, 0x09);
    valueArray.Set(9, 0x0A);
    valueArray.Set(10, 0x0B);
    valueArray.Set(11, 0x0C);
    valueArray.Set(12, values, sizeof(values) / sizeof(uint8_t));
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), ref, size * sizeof(uint8_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, SetFixedArrayInitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    FixedArray<uint8_t> values({ 0x0D, 0x0E, 0x0F, 0x10 });
    uint8_t data[16];
    size_t size = sizeof(data) / sizeof(uint8_t);
    FixedArray<uint8_t> valueArray(data, size);
    valueArray.Set(0, 0x01);
    valueArray.Set(1, 0x02);
    valueArray.Set(2, 0x03);
    valueArray.Set(3, 0x04);
    valueArray.Set(4, 0x05);
    valueArray.Set(5, 0x06);
    valueArray.Set(6, 0x07);
    valueArray.Set(7, 0x08);
    valueArray.Set(8, 0x09);
    valueArray.Set(9, 0x0A);
    valueArray.Set(10, 0x0B);
    valueArray.Set(11, 0x0C);
    valueArray.Set(12, values);
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), ref, size * sizeof(uint8_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, SetFixedArrayUninitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    FixedArray<uint8_t> values({ 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(uint8_t);
    FixedArray<uint8_t> valueArray(size);
    valueArray.Set(0, 0x01);
    valueArray.Set(1, 0x02);
    valueArray.Set(2, 0x03);
    valueArray.Set(3, 0x04);
    valueArray.Set(4, 0x05);
    valueArray.Set(5, 0x06);
    valueArray.Set(6, 0x07);
    valueArray.Set(7, 0x08);
    valueArray.Set(8, 0x09);
    valueArray.Set(9, 0x0A);
    valueArray.Set(10, 0x0B);
    valueArray.Set(11, 0x0C);
    valueArray.Set(12, values);
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), ref, size * sizeof(uint8_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, SetInitializerInitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    uint8_t data[16];
    size_t size = sizeof(data) / sizeof(uint8_t);
    FixedArray<uint8_t> valueArray(data, size);
    valueArray.Set(0, 0x01);
    valueArray.Set(1, 0x02);
    valueArray.Set(2, 0x03);
    valueArray.Set(3, 0x04);
    valueArray.Set(4, 0x05);
    valueArray.Set(5, 0x06);
    valueArray.Set(6, 0x07);
    valueArray.Set(7, 0x08);
    valueArray.Set(8, 0x09);
    valueArray.Set(9, 0x0A);
    valueArray.Set(10, 0x0B);
    valueArray.Set(11, 0x0C);
    valueArray.Set(12, { 0x0D, 0x0E, 0x0F, 0x10 });
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), ref, size * sizeof(uint8_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, SetInitializerUninitializedArray)
{
    uint8_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref) / sizeof(uint8_t);
    FixedArray<uint8_t> valueArray(size);
    valueArray.Set(0, 0x01);
    valueArray.Set(1, 0x02);
    valueArray.Set(2, 0x03);
    valueArray.Set(3, 0x04);
    valueArray.Set(4, 0x05);
    valueArray.Set(5, 0x06);
    valueArray.Set(6, 0x07);
    valueArray.Set(7, 0x08);
    valueArray.Set(8, 0x09);
    valueArray.Set(9, 0x0A);
    valueArray.Set(10, 0x0B);
    valueArray.Set(11, 0x0C);
    valueArray.Set(12, { 0x0D, 0x0E, 0x0F, 0x10 });
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), ref, size * sizeof(uint8_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, OperatorEquals)
{
    FixedArray<uint8_t> valueArrayRef1({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    FixedArray<uint8_t> valueArrayRef2({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    FixedArray<uint8_t> valueArrayRef3({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E });
    FixedArray<uint8_t> valueArrayDummy(0);
    FixedArray<uint8_t> valueArrayData({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    EXPECT_FALSE(valueArrayRef1 == valueArrayData);
    EXPECT_TRUE(valueArrayRef2 == valueArrayData);
    EXPECT_FALSE(valueArrayRef3 == valueArrayData);
    EXPECT_FALSE(valueArrayDummy == valueArrayData);
    // cppcheck-suppress duplicateExpression
    EXPECT_TRUE(valueArrayData == valueArrayData);
}

TEST_FIXTURE(FixedArrayTest, OperatorNotEquals)
{
    FixedArray<uint8_t> valueArrayRef1({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    FixedArray<uint8_t> valueArrayRef2({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    FixedArray<uint8_t> valueArrayRef3({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E });
    FixedArray<uint8_t> valueArrayDummy(0);
    FixedArray<uint8_t> valueArrayData({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    EXPECT_TRUE(valueArrayRef1 != valueArrayData);
    EXPECT_FALSE(valueArrayRef2 != valueArrayData);
    EXPECT_TRUE(valueArrayRef3 != valueArrayData);
    EXPECT_TRUE(valueArrayDummy != valueArrayData);
    // cppcheck-suppress duplicateExpression
    EXPECT_FALSE(valueArrayData != valueArrayData);
}

TEST_FIXTURE(FixedArrayTest, OperatorIndex)
{
    FixedArray<uint8_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    EXPECT_EQ(0x01, target[0]);
    EXPECT_EQ(0x02, target[1]);
    EXPECT_EQ(0x03, target[2]);
    EXPECT_EQ(0x04, target[3]);
    EXPECT_EQ(0x05, target[4]);
    EXPECT_EQ(0x06, target[5]);
    EXPECT_EQ(0x07, target[6]);
    EXPECT_EQ(0x08, target[7]);
    EXPECT_EQ(0x09, target[8]);
    EXPECT_EQ(0x0A, target[9]);
    EXPECT_EQ(0x0B, target[10]);
    EXPECT_EQ(0x0C, target[11]);
    EXPECT_EQ(0x0D, target[12]);
    EXPECT_EQ(0x0E, target[13]);
    EXPECT_EQ(0x0F, target[14]);
    EXPECT_EQ(0x10, target[15]);
    target[15] = 0xFF;
    EXPECT_EQ(0xFF, target[15]);
    EXPECT_THROW(target[-1], Core::ArgumentOutOfRangeException);
    EXPECT_THROW(target[16], Core::ArgumentOutOfRangeException);
}

TEST_FIXTURE(FixedArrayTest, OperatorIndexConst)
{
    FixedArray<uint8_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    const FixedArray<uint8_t> &targetConst = target;
    EXPECT_EQ(0x01, targetConst[0]);
    EXPECT_EQ(0x02, targetConst[1]);
    EXPECT_EQ(0x03, targetConst[2]);
    EXPECT_EQ(0x04, targetConst[3]);
    EXPECT_EQ(0x05, targetConst[4]);
    EXPECT_EQ(0x06, targetConst[5]);
    EXPECT_EQ(0x07, targetConst[6]);
    EXPECT_EQ(0x08, targetConst[7]);
    EXPECT_EQ(0x09, targetConst[8]);
    EXPECT_EQ(0x0A, targetConst[9]);
    EXPECT_EQ(0x0B, targetConst[10]);
    EXPECT_EQ(0x0C, targetConst[11]);
    EXPECT_EQ(0x0D, targetConst[12]);
    EXPECT_EQ(0x0E, targetConst[13]);
    EXPECT_EQ(0x0F, targetConst[14]);
    EXPECT_EQ(0x10, targetConst[15]);
    EXPECT_THROW(targetConst[-1], Core::ArgumentOutOfRangeException);
    EXPECT_THROW(targetConst[16], Core::ArgumentOutOfRangeException);
}

TEST_FIXTURE(FixedArrayTest, ConstructorSizeInt32)
{
    int32_t ref[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    size_t size = sizeof(ref) / sizeof(int32_t);
    FixedArray<int32_t> valueArray(size);
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), ref, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, ConstructorDataLengthInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    FixedArray<int32_t> valueArray(data, size);
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), data, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, CopyConstructorInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    FixedArray<int32_t> valueArray(data, size);
    FixedArray<int32_t> target(valueArray);
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), data, size * sizeof(int32_t)) == 0);
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(target.Data(), data, size * sizeof(int32_t)) == 0);
    EXPECT_NE(target.Data(), valueArray.Data());
}

TEST_FIXTURE(FixedArrayTest, ConstructorInitializerInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    FixedArray<int32_t> valueArray({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), data, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, CastToElementPointerInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    FixedArray<int32_t> valueArray({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 });
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp((const int32_t *)valueArray, data, size * sizeof(int32_t)) == 0);
    EXPECT_TRUE(memcmp(static_cast<const int32_t *>(valueArray), data, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, OperatorAssignInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    FixedArray<int32_t> valueArray(data, size);
    FixedArray<int32_t> target(0);
    target = valueArray;
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), data, size * sizeof(int32_t)) == 0);
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(target.Data(), data, size * sizeof(int32_t)) == 0);
    EXPECT_NE(target.Data(), valueArray.Data());
}

TEST_FIXTURE(FixedArrayTest, ClearInt32)
{
    int32_t data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    size_t size = sizeof(data) / sizeof(int32_t);
    FixedArray<int32_t> target(data, size);
    EXPECT_EQ(size, target.Size());
    EXPECT_TRUE(memcmp(data, target.Data(), size) == 0);
    target.Clear();
    EXPECT_EQ(size, target.Size());
}

TEST_FIXTURE(FixedArrayTest, SetInitializedArrayInt32)
{
    int32_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    int32_t values[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    int32_t data[16];
    size_t size = sizeof(data) / sizeof(int32_t);
    FixedArray<int32_t> valueArray(data, size);
    valueArray.Set(0, 0x01);
    valueArray.Set(1, 0x02);
    valueArray.Set(2, 0x03);
    valueArray.Set(3, 0x04);
    valueArray.Set(4, 0x05);
    valueArray.Set(5, 0x06);
    valueArray.Set(6, 0x07);
    valueArray.Set(7, 0x08);
    valueArray.Set(8, 0x09);
    valueArray.Set(9, 0x0A);
    valueArray.Set(10, 0x0B);
    valueArray.Set(11, 0x0C);
    valueArray.Set(12, values, sizeof(values) / sizeof(int32_t));
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), ref, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, GetInt32)
{
    int32_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    FixedArray<int32_t> valueArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(int32_t);
    EXPECT_EQ((int32_t)0x01, valueArray.Get(0));
    EXPECT_EQ((int32_t)0x02, valueArray.Get(1));
    EXPECT_EQ((int32_t)0x03, valueArray.Get(2));
    EXPECT_EQ((int32_t)0x04, valueArray.Get(3));
    EXPECT_EQ((int32_t)0x05, valueArray.Get(4));
    EXPECT_EQ((int32_t)0x06, valueArray.Get(5));
    EXPECT_EQ((int32_t)0x07, valueArray.Get(6));
    EXPECT_EQ((int32_t)0x08, valueArray.Get(7));
    EXPECT_EQ((int32_t)0x09, valueArray.Get(8));
    EXPECT_EQ((int32_t)0x0A, valueArray.Get(9));
    EXPECT_EQ((int32_t)0x0B, valueArray.Get(10));
    EXPECT_EQ((int32_t)0x0C, valueArray.Get(11));
    FixedArray<int32_t> values = valueArray.Get(12, 4);
    EXPECT_EQ(size, values.Size());
    EXPECT_TRUE(memcmp(values.Data(), ref, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, GetPtrInt32)
{
    int32_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    FixedArray<int32_t> valueArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    const size_t size = sizeof(ref) / sizeof(int32_t);

    int32_t data[size];
    EXPECT_EQ(size, valueArray.Get(12, data, size));
    EXPECT_TRUE(memcmp(ref, data, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, GetFixedArrayByRefInt32)
{
    int32_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    FixedArray<int32_t> valueArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(int32_t);

    FixedArray<int32_t> data(0);
    EXPECT_EQ(size, valueArray.Get(12, data, size));
    EXPECT_EQ(size, data.Size());
    EXPECT_TRUE(memcmp(ref, data.Data(), size) == 0);
}

TEST_FIXTURE(FixedArrayTest, GetFixedArrayInt32)
{
    int32_t ref[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    FixedArray<int32_t> valueArray({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(int32_t);
    FixedArray<int32_t> values = valueArray.Get(12, size);
    EXPECT_EQ(size, values.Size());
    EXPECT_TRUE(memcmp(values.Data(), ref, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, SetUninitializedArrayInt32)
{
    int32_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    int32_t values[] = { 0x0D, 0x0E, 0x0F, 0x10 };
    size_t size = sizeof(ref) / sizeof(int32_t);
    FixedArray<int32_t> valueArray(size);
    valueArray.Set(0, 0x01);
    valueArray.Set(1, 0x02);
    valueArray.Set(2, 0x03);
    valueArray.Set(3, 0x04);
    valueArray.Set(4, 0x05);
    valueArray.Set(5, 0x06);
    valueArray.Set(6, 0x07);
    valueArray.Set(7, 0x08);
    valueArray.Set(8, 0x09);
    valueArray.Set(9, 0x0A);
    valueArray.Set(10, 0x0B);
    valueArray.Set(11, 0x0C);
    valueArray.Set(12, values, sizeof(values) / sizeof(int32_t));
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), ref, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, SetFixedArrayInitializedArrayInt32)
{
    int32_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    FixedArray<int32_t> values({ 0x0D, 0x0E, 0x0F, 0x10 });
    int32_t data[16];
    size_t size = sizeof(data) / sizeof(int32_t);
    FixedArray<int32_t> valueArray(data, size);
    valueArray.Set(0, 0x01);
    valueArray.Set(1, 0x02);
    valueArray.Set(2, 0x03);
    valueArray.Set(3, 0x04);
    valueArray.Set(4, 0x05);
    valueArray.Set(5, 0x06);
    valueArray.Set(6, 0x07);
    valueArray.Set(7, 0x08);
    valueArray.Set(8, 0x09);
    valueArray.Set(9, 0x0A);
    valueArray.Set(10, 0x0B);
    valueArray.Set(11, 0x0C);
    valueArray.Set(12, values);
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), ref, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, SetFixedArrayUninitializedArrayInt32)
{
    int32_t ref[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 };
    FixedArray<int32_t> values({ 0x0D, 0x0E, 0x0F, 0x10 });
    size_t size = sizeof(ref) / sizeof(int32_t);
    FixedArray<int32_t> valueArray(size);
    valueArray.Set(0, 0x01);
    valueArray.Set(1, 0x02);
    valueArray.Set(2, 0x03);
    valueArray.Set(3, 0x04);
    valueArray.Set(4, 0x05);
    valueArray.Set(5, 0x06);
    valueArray.Set(6, 0x07);
    valueArray.Set(7, 0x08);
    valueArray.Set(8, 0x09);
    valueArray.Set(9, 0x0A);
    valueArray.Set(10, 0x0B);
    valueArray.Set(11, 0x0C);
    valueArray.Set(12, values);
    EXPECT_EQ(size, valueArray.Size());
    EXPECT_TRUE(memcmp(valueArray.Data(), ref, size * sizeof(int32_t)) == 0);
}

TEST_FIXTURE(FixedArrayTest, OperatorEqualsInt32)
{
    FixedArray<int32_t> valueArrayRef1({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    FixedArray<int32_t> valueArrayRef2({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    FixedArray<int32_t> valueArrayRef3({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E });
    FixedArray<int32_t> valueArrayDummy(0);
    FixedArray<int32_t> valueArrayData({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    EXPECT_FALSE(valueArrayRef1 == valueArrayData);
    EXPECT_TRUE(valueArrayRef2 == valueArrayData);
    EXPECT_FALSE(valueArrayRef3 == valueArrayData);
    EXPECT_FALSE(valueArrayDummy == valueArrayData);
    // cppcheck-suppress duplicateExpression
    EXPECT_TRUE(valueArrayData == valueArrayData);
}

TEST_FIXTURE(FixedArrayTest, OperatorNotEqualsInt32)
{
    FixedArray<int32_t> valueArrayRef1({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    FixedArray<int32_t> valueArrayRef2({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    FixedArray<int32_t> valueArrayRef3({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E });
    FixedArray<int32_t> valueArrayDummy(0);
    FixedArray<int32_t> valueArrayData({ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F });
    EXPECT_TRUE(valueArrayRef1 != valueArrayData);
    EXPECT_FALSE(valueArrayRef2 != valueArrayData);
    EXPECT_TRUE(valueArrayRef3 != valueArrayData);
    EXPECT_TRUE(valueArrayDummy != valueArrayData);
    // cppcheck-suppress duplicateExpression
    EXPECT_FALSE(valueArrayData != valueArrayData);
}

TEST_FIXTURE(FixedArrayTest, OperatorIndexInt32)
{
    FixedArray<int32_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    EXPECT_EQ(0x01, target[0]);
    EXPECT_EQ(0x02, target[1]);
    EXPECT_EQ(0x03, target[2]);
    EXPECT_EQ(0x04, target[3]);
    EXPECT_EQ(0x05, target[4]);
    EXPECT_EQ(0x06, target[5]);
    EXPECT_EQ(0x07, target[6]);
    EXPECT_EQ(0x08, target[7]);
    EXPECT_EQ(0x09, target[8]);
    EXPECT_EQ(0x0A, target[9]);
    EXPECT_EQ(0x0B, target[10]);
    EXPECT_EQ(0x0C, target[11]);
    EXPECT_EQ(0x0D, target[12]);
    EXPECT_EQ(0x0E, target[13]);
    EXPECT_EQ(0x0F, target[14]);
    EXPECT_EQ(0x10, target[15]);
    target[15] = 0xFF;
    EXPECT_EQ(0xFF, target[15]);
    EXPECT_THROW(target[-1], Core::ArgumentOutOfRangeException);
    EXPECT_THROW(target[16], Core::ArgumentOutOfRangeException);
}

TEST_FIXTURE(FixedArrayTest, OperatorIndexConstInt32)
{
    FixedArray<int32_t> target({ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10 });
    const FixedArray<int32_t> &targetConst = target;
    EXPECT_EQ(0x01, targetConst[0]);
    EXPECT_EQ(0x02, targetConst[1]);
    EXPECT_EQ(0x03, targetConst[2]);
    EXPECT_EQ(0x04, targetConst[3]);
    EXPECT_EQ(0x05, targetConst[4]);
    EXPECT_EQ(0x06, targetConst[5]);
    EXPECT_EQ(0x07, targetConst[6]);
    EXPECT_EQ(0x08, targetConst[7]);
    EXPECT_EQ(0x09, targetConst[8]);
    EXPECT_EQ(0x0A, targetConst[9]);
    EXPECT_EQ(0x0B, targetConst[10]);
    EXPECT_EQ(0x0C, targetConst[11]);
    EXPECT_EQ(0x0D, targetConst[12]);
    EXPECT_EQ(0x0E, targetConst[13]);
    EXPECT_EQ(0x0F, targetConst[14]);
    EXPECT_EQ(0x10, targetConst[15]);
    EXPECT_THROW(targetConst[-1], Core::ArgumentOutOfRangeException);
    EXPECT_THROW(targetConst[16], Core::ArgumentOutOfRangeException);
}

} // namespace Test

} // namespace Core
