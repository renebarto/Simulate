#include "unit-test-c++/UnitTestC++.h"

#include <iostream>
#include "core/ActiveObject.h"
#include "core/Util.h"

namespace Core
{

namespace Test
{

static const int SLEEP = 100;

class MyActiveObject: public ActiveObject
{
public:
    static bool isDeleted;

    MyActiveObject(): ActiveObject("ActiveObjTst1")
    {
        MyActiveObject::isDeleted = false;
    }
    ~MyActiveObject()
    {
        isDeleted = true;
    }
    virtual void InitThread() {}
    virtual void Run() { Core::Util::Sleep(SLEEP); }
    virtual void ExitThread() {}
    virtual void FlushThread() {}
};

bool MyActiveObject::isDeleted;

class MyActiveObject2: public ActiveObject
{
public:
    MyActiveObject2() : ActiveObject("ActiveObjTst2") {}
    virtual void InitThread() { }
    virtual void Run()
    {
        while (!IsDying())
        {
            Core::Util::Sleep(SLEEP * 2);
        }
    }
    virtual void ExitThread() { }
    virtual void FlushThread() { }
};

class ActiveObjectTest : public UnitTestCpp::TestFixture
{
public:
    static const int WaitIterations = 10;

    virtual void SetUp();
    virtual void TearDown();
};

void ActiveObjectTest::SetUp()
{
}

void ActiveObjectTest::TearDown()
{
}

TEST_FIXTURE(ActiveObjectTest, Simple)
{
    MyActiveObject object;
    object.Create();
    Core::Util::Sleep(SLEEP);
    object.Kill();
}

TEST_FIXTURE(ActiveObjectTest, Loop)
{
    MyActiveObject2 object;
    object.Create();
    Core::Util::Sleep(SLEEP);
    object.Kill();
}

TEST_FIXTURE(ActiveObjectTest, StartThreadManyTimes)
{
    MyActiveObject2 object;
    for (int i = 0; i < WaitIterations; i++)
    {
        object.Create();
        Core::Util::Sleep(SLEEP);
        object.Kill();
    }
}

} // namespace Test

} // namespace Core
