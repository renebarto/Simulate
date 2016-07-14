#include "unit-test-c++/TestFixtureInfo.h"

#include <cassert>
#include "unit-test-c++/TestInfo.h"

using namespace std;

namespace UnitTestCpp
{

TestFixtureInfo::TestFixtureInfo(string const & fixtureName)
    : next(nullptr)
    , head(nullptr)
    , tail(nullptr)
    , fixtureName(fixtureName)
{
}

void TestFixtureInfo::AddTest(Test * test)
{
    if (!tail)
    {
        assert(!head);
        head = test;
        tail = test;
    }
    else
    {
        tail->next = test;
        tail = test;
    }
}

Test * TestFixtureInfo::GetHead() const
{
    return head;
}

const string & TestFixtureInfo::Name() const
{
    return fixtureName;
}

int TestFixtureInfo::CountTests()
{
    int numberOfTests = 0;
    Test * test = GetHead();
    while (test)
    {
        ++numberOfTests;
        test = test->next;
    }
    return numberOfTests;
}

} // namespace UnitTestCpp
