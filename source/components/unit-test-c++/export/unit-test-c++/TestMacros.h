#pragma once

#include "unit-test-c++/TestSuite.h"
#include "unit-test-c++/TestRegistry.h"
#include "unit-test-c++/ExecuteTest.h"

#define TEST_EX(Name, List)                                                                        \
class Test##Name : public UnitTestCpp::Test                                                        \
{                                                                                                  \
public:                                                                                            \
    Test##Name() : Test(#Name, "", GetSuiteName(), __FILE__, __LINE__) {}                          \
private:                                                                                           \
    virtual void RunImpl() const;                                                                  \
} test##Name##Instance;                                                                            \
                                                                                                   \
UnitTestCpp::TestRegistrar registrar##Name(List, &test##Name##Instance);                           \
                                                                                                   \
void Test##Name::RunImpl() const

#define TEST(Name) TEST_EX(Name, UnitTestCpp::Test::GetTestRegistry())

#define TEST_FIXTURE_EX(Fixture,Name,List)                                                         \
class Fixture##Name##Helper : public Fixture                                                       \
{                                                                                                  \
public:                                                                                            \
    Fixture##Name##Helper(const Fixture##Name##Helper &) = delete;                                 \
    explicit Fixture##Name##Helper(UnitTestCpp::TestDetails const & details) :                     \
        details(details)                                                                           \
    {                                                                                              \
        SetUp();                                                                                   \
    }                                                                                              \
    ~Fixture##Name##Helper()                                                                       \
    {                                                                                              \
        TearDown();                                                                                \
    }                                                                                              \
    Fixture##Name##Helper & operator = (const Fixture##Name##Helper &) = delete;                   \
    void RunImpl();                                                                                \
    UnitTestCpp::TestDetails const & details;                                                      \
};                                                                                                 \
                                                                                                   \
class Test##Fixture##Name : public UnitTestCpp::Test                                               \
{                                                                                                  \
public:                                                                                            \
    Test##Fixture##Name() :                                                                        \
        Test(#Name, #Fixture, GetSuiteName(), __FILE__, __LINE__)                                  \
    {                                                                                              \
    }                                                                                              \
private:                                                                                           \
    virtual void RunImpl() const;                                                                  \
} test##Fixture##Name##Instance;                                                                   \
                                                                                                   \
UnitTestCpp::TestRegistrar registrar##Fixture##Name(List, &test##Fixture##Name##Instance);         \
                                                                                                   \
void Test##Fixture##Name::RunImpl() const                                                          \
{                                                                                                  \
    bool ctorOk = false;                                                                           \
    try                                                                                            \
    {                                                                                              \
        Fixture##Name##Helper fixtureHelper(details);                                              \
        ctorOk = true;                                                                             \
        UnitTestCpp::ExecuteTest(fixtureHelper, details);                                          \
    }                                                                                              \
    catch (const ::UnitTestCpp::AssertException & e)                                               \
    {                                                                                              \
        UnitTestCpp::CurrentTest::Results()->OnTestFailure(                                        \
            UnitTestCpp::TestDetails(details.testName,                                             \
                                     details.fixtureName,                                          \
                                     details.suiteName,                                            \
                                     e.Filename(),                                                 \
                                     e.LineNumber()),                                              \
                                     e.what());                                                    \
    }                                                                                              \
    catch (const std::exception & e)                                                               \
    {                                                                                              \
        std::ostringstream stream;                                                                 \
        stream << "Unhandled exception: " << e.what();                                             \
        UnitTestCpp::CurrentTest::Results()->OnTestFailure(details, stream.str());                 \
    }                                                                                              \
    catch (...)                                                                                    \
    {                                                                                              \
        if (ctorOk)                                                                                \
        {                                                                                          \
            UnitTestCpp::CurrentTest::Results()->OnTestFailure(                                    \
                UnitTestCpp::TestDetails(details, __LINE__),                                       \
                "Unhandled exception while destroying fixture " #Fixture);                         \
        }                                                                                          \
        else                                                                                       \
        {                                                                                          \
            UnitTestCpp::CurrentTest::Results()->OnTestFailure(                                    \
                UnitTestCpp::TestDetails(details, __LINE__),                                       \
                "Unhandled exception while constructing fixture " #Fixture);                       \
        }                                                                                          \
    }                                                                                              \
}                                                                                                  \
void Fixture##Name##Helper::RunImpl()

#define TEST_FIXTURE(Fixture,Name) TEST_FIXTURE_EX(Fixture,Name, UnitTestCpp::Test::GetTestRegistry())

#define TEST_SUITE(Name)                                                                           \
    namespace Suite##Name                                                                          \
    {                                                                                              \
        inline char const* GetSuiteName()                                                          \
        {                                                                                          \
            return #Name ;                                                                         \
        }                                                                                          \
    }                                                                                              \
    namespace Suite##Name
