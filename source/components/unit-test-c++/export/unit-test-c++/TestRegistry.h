#pragma once

#include <string>
#include "unit-test-c++/TestSuiteInfo.h"

namespace UnitTestCpp
{

class TestInfo;

class TestRegistry
{
public:
    TestRegistry();
    TestRegistry(TestRegistry const &) = delete;
    virtual ~TestRegistry();

    TestRegistry & operator = (TestRegistry const &) = delete;

    TestSuiteInfo * GetTestSuite(std::string const & suiteName);
    TestSuiteInfo * GetHead() const;

    int CountSuites();
    int CountFixtures();
    int CountTests();
    template <typename Predicate> int CountSuitesIf(Predicate predicate);
    template <typename Predicate> int CountFixturesIf(Predicate predicate);
    template <typename Predicate> int CountTestsIf(Predicate predicate);

private:
    TestSuiteInfo * head;
    TestSuiteInfo * tail;

    void AddSuite(TestSuiteInfo * testSuite);
};

class TestRegistrar
{
public:
    TestRegistrar(TestRegistry & registry, Test * test);
};

template <typename Predicate> int TestRegistry::CountSuitesIf(Predicate predicate)
{
    int numberOfTestSuites = 0;
    TestSuiteInfo * testSuite = GetHead();
    while (testSuite)
    {
        if (predicate(testSuite))
            ++numberOfTestSuites;
        testSuite = testSuite->next;
    }
    return numberOfTestSuites;
}

template <typename Predicate> int TestRegistry::CountFixturesIf(Predicate predicate)
{
    int numberOfTestFixtures = 0;
    TestSuiteInfo * testSuite = GetHead();
    while (testSuite)
    {
        if (predicate(testSuite))
            numberOfTestFixtures += testSuite->CountFixturesIf(predicate);
        testSuite = testSuite->next;
    }
    return numberOfTestFixtures;
}

template <typename Predicate> int TestRegistry::CountTestsIf(Predicate predicate)
{
    int numberOfTests = 0;
    TestSuiteInfo * testSuite = GetHead();
    while (testSuite)
    {
        if (predicate(testSuite))
            numberOfTests += testSuite->CountTestsIf(predicate);
        testSuite = testSuite->next;
    }
    return numberOfTests;
}

} // namespace UnitTestCpp
