#pragma once

namespace UnitTestCpp
{

class TestFixture
{
protected:
    TestFixture() {}
    TestFixture(const TestFixture &) = delete;
    ~TestFixture() {}

    TestFixture & operator = (const TestFixture &) = delete;

    virtual void SetUp() = 0;
    virtual void TearDown() = 0;

private:
    void Setup(); // Guard against incorrect override
    void Teardown(); // Guard against incorrect override
};

} // namespace UnitTestCpp
