#include "unit-test-c++/TestSuiteInfo.h"

#include <cassert>
#include <iostream>
#include "unit-test-c++/TestFixtureInfo.h"
#include "unit-test-c++/TestResults.h"
#include "unit-test-c++/Timer.h"

using namespace std;

namespace UnitTestCpp
{

TestSuiteInfo::TestSuiteInfo(string const & suiteName)
    : next(nullptr)
    , head(nullptr)
    , tail(nullptr)
    , suiteName(suiteName)
{
}

TestSuiteInfo::~TestSuiteInfo()
{
    TestFixtureInfo * testFixture = GetHead();
    while (testFixture)
    {
        TestFixtureInfo * currentFixture = testFixture;
        testFixture = testFixture->next;
        delete currentFixture;
    }
}

TestFixtureInfo * TestSuiteInfo::GetTestFixture(string const & fixtureName)
{
#ifdef TRACE
    cout << "Find suite " << fixtureName << " ... ";
#endif
    TestFixtureInfo * testFixture = head;
    while (testFixture && (testFixture->Name() != fixtureName))
        testFixture = testFixture->next;
    if (!testFixture)
    {
#ifdef TRACE
        cout << "not found, creating new object" << endl;
#endif
        testFixture = new TestFixtureInfo(fixtureName);
        AddFixture(testFixture);
    }
    else
    {
#ifdef TRACE
        cout << "found" << endl;
#endif
    }
    return testFixture;
}

void TestSuiteInfo::AddFixture(TestFixtureInfo * testFixture)
{
    if (!tail)
    {
        assert(!head);
        head = testFixture;
        tail = testFixture;
    }
    else
    {
        tail->next = testFixture;
        tail = testFixture;
    }
}

TestFixtureInfo * TestSuiteInfo::GetHead() const
{
    return head;
}

const string & TestSuiteInfo::Name() const
{
    return suiteName;
}

int TestSuiteInfo::CountFixtures()
{
    int numberOfTestFixtures = 0;
    TestFixtureInfo * testFixture = GetHead();
    while (testFixture)
    {
        ++numberOfTestFixtures;
        testFixture = testFixture->next;
    }
    return numberOfTestFixtures;
}

int TestSuiteInfo::CountTests()
{
    int numberOfTests = 0;
    TestFixtureInfo * testFixture = GetHead();
    while (testFixture)
    {
        numberOfTests += testFixture->CountTests();
        testFixture = testFixture->next;
    }
    return numberOfTests;
}

} // namespace UnitTestCpp
