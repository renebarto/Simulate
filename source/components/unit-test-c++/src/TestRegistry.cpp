#include "unit-test-c++/TestRegistry.h"

#include <cassert>
#include <iostream>
#include "unit-test-c++/TestInfo.h"
#include "unit-test-c++/TestFixtureInfo.h"
#include "unit-test-c++/TestSuiteInfo.h"

using namespace std;

namespace UnitTestCpp
{

TestRegistry::TestRegistry()
    : head(nullptr)
    , tail(nullptr)
{
}

TestRegistry::~TestRegistry()
{
    TestSuiteInfo * testSuite = GetHead();
    while (testSuite)
    {
        TestSuiteInfo * currentSuite = testSuite;
        testSuite = testSuite->next;
        delete currentSuite;
    }
}

TestSuiteInfo * TestRegistry::GetTestSuite(string const & suiteName)
{
#ifdef TRACE
    cout << "Find suite " << suiteName << " ... ";
#endif
    TestSuiteInfo * testSuite = GetHead();
    while (testSuite && (testSuite->Name() != suiteName))
        testSuite = testSuite->next;
    if (!testSuite)
    {
#ifdef TRACE
        cout << "not found, creating new object" << endl;
#endif
        testSuite = new TestSuiteInfo(suiteName);
        AddSuite(testSuite);
    }
    else
    {
#ifdef TRACE
        cout << "found" << endl;
#endif
    }
    return testSuite;
}

void TestRegistry::AddSuite(TestSuiteInfo * testSuite)
{
    if (!tail)
    {
        assert(!head);
        head = testSuite;
        tail = testSuite;
    }
    else
    {
        tail->next = testSuite;
        tail = testSuite;
    }
}

TestSuiteInfo * TestRegistry::GetHead() const
{
    return head;
}

int TestRegistry::CountSuites()
{
    int numberOfTestSuites = 0;
    TestSuiteInfo * testSuite = GetHead();
    while (testSuite)
    {
        ++numberOfTestSuites;
        testSuite = testSuite->next;
    }
    return numberOfTestSuites;
}

int TestRegistry::CountFixtures()
{
    int numberOfTestFixtures = 0;
    TestSuiteInfo * testSuite = GetHead();
    while (testSuite)
    {
        numberOfTestFixtures += testSuite->CountFixtures();
        testSuite = testSuite->next;
    }
    return numberOfTestFixtures;
}

int TestRegistry::CountTests()
{
    int numberOfTests = 0;
    TestSuiteInfo * testSuite = GetHead();
    while (testSuite)
    {
        numberOfTests += testSuite->CountTests();
        testSuite = testSuite->next;
    }
    return numberOfTests;
}

TestRegistrar::TestRegistrar(TestRegistry & registry, Test * test)
{
#ifdef TRACE
    cout << "Register test " << test->details.testName
         << " in fixture " << test->details.fixtureName << endl;
#endif
    TestSuiteInfo * testSuite = registry.GetTestSuite(test->details.suiteName);
    TestFixtureInfo * testFixture = testSuite->GetTestFixture(test->details.fixtureName);
    testFixture->AddTest(test);
}

} // namespace UnitTestCpp
