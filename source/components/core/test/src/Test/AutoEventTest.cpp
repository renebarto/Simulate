#include "unit-test-c++/UnitTestC++.h"

#include <thread>
#include "core/AutoEvent.h"
#include "core/DefaultLogger.h"
#include "core/Util.h"

using namespace std;

namespace Core
{

namespace Test
{

class AutoEventTest : public UnitTestCpp::TestFixture
{
public:
    static const int SLEEP = 10;
    static const int TIMEOUT = 100;

    virtual void SetUp();
    virtual void TearDown();

    static bool eventPassed;
    static AutoEventHandle eventHandle1;
    static AutoEventHandle eventHandle2;

    static void TestThread(void)
    {
        Core::TheLogger().Debug("TestThread", "Started");

        AutoEvent event1;
        AutoEvent event2;
        if (eventHandle1 != 0)
            event1 = eventHandle1;
        if (eventHandle2 != 0)
            event2 = eventHandle2;

        ostringstream stream;
        stream << "Event 1: " << event1.GetValue() << endl;
        stream << "Event 2: " << event2.GetValue();
        Core::TheLogger().Debug("TestThread", stream.str());

        eventPassed = event1.Wait(TIMEOUT);

        stream.str("");
        stream << "Event 1: " << event1.GetValue() << endl;
        stream << "Event 2: " << event2.GetValue();
        Core::TheLogger().Debug("TestThread", stream.str());

        if (eventPassed)
        {
            event2.Set();
        }

        stream.str("");
        stream << "Event 1: " << event1.GetValue() << endl;
        stream << "Event 2: " << event2.GetValue();
        Core::TheLogger().Debug("TestThread", stream.str());
        Core::TheLogger().Debug("TestThread", "Ended");
    }
};

bool AutoEventTest::eventPassed = false;
AutoEventHandle AutoEventTest::eventHandle1;
AutoEventHandle AutoEventTest::eventHandle2;

void AutoEventTest::SetUp()
{
    eventPassed = false;
    eventHandle1 = 0;
    eventHandle2 = 0;
}

void AutoEventTest::TearDown()
{
}

TEST_FIXTURE(AutoEventTest, Construction)
{
    AutoEvent event1;
    EXPECT_TRUE(((AutoEventHandle)event1) != 0);
}

TEST_FIXTURE(AutoEventTest, ConstructionCopy)
{
    AutoEvent event1;
    AutoEvent event2(event1);
    EXPECT_EQ((AutoEventHandle)event1, (AutoEventHandle)event2);
}

TEST_FIXTURE(AutoEventTest, ConstructionInit)
{
    AutoEvent event1;
    AutoEventHandle eventHandle = event1;
    AutoEvent event2(eventHandle);
    EXPECT_EQ(eventHandle, (AutoEventHandle)event2);
}

TEST_FIXTURE(AutoEventTest, AssignmentEvent)
{
    AutoEvent event1;
    AutoEvent event2;
    event2 = event1;
    EXPECT_EQ((AutoEventHandle)event1, (AutoEventHandle)event2);
}

TEST_FIXTURE(AutoEventTest, AssignmentHandle)
{
    AutoEvent event1;
    AutoEventHandle eventHandle = event1;
    AutoEvent event2;
    event2 = eventHandle;
    EXPECT_EQ(eventHandle, (AutoEventHandle)event2);
}

TEST_FIXTURE(AutoEventTest, SetEvent)
{
    Core::TheLogger().Debug("AutoEventTest", "SetEvent start");

    AutoEvent event1;
    AutoEvent event2;
    eventHandle1 = (AutoEventHandle)event1;
    eventHandle2 = (AutoEventHandle)event2;

    std::thread thread(TestThread);

    ostringstream stream;
    stream << "Event 1: " << event1.GetValue() << endl;
    stream << "Event 2: " << event2.GetValue();
    Core::TheLogger().Debug("TestThread", stream.str());

    event1.Set();

    stream.str("");
    stream << "Event 1: " << event1.GetValue() << endl;
    stream << "Event 2: " << event2.GetValue();
    Core::TheLogger().Debug("TestThread", stream.str());

    Core::Util::Sleep(SLEEP);
    EXPECT_TRUE(event2.Wait(TIMEOUT));
    EXPECT_TRUE(eventPassed);

        stream.str("");
        stream << "Event 1: " << event1.GetValue() << endl;
        stream << "Event 2: " << event2.GetValue();
        Core::TheLogger().Debug("TestThread", stream.str());

    thread.join();

    Core::TheLogger().Debug("AutoEventTest", "SetEvent end");
}

TEST_FIXTURE(AutoEventTest, WaitEventTimeout)
{
    Core::TheLogger().Debug("AutoEventTest", "WaitEventTimeout start");

    AutoEvent event1;
    AutoEvent event2;
    eventHandle1 = (AutoEventHandle)event1;
    eventHandle2 = (AutoEventHandle)event2;

    std::thread thread(TestThread);

    ostringstream stream;
    stream << "Event 1: " << event1.GetValue() << endl;
    stream << "Event 2: " << event2.GetValue();
    Core::TheLogger().Debug("TestThread", stream.str());

    EXPECT_FALSE(event2.Wait(TIMEOUT));
    EXPECT_FALSE(eventPassed);

    stream.str("");
    stream << "Event 1: " << event1.GetValue() << endl;
    stream << "Event 2: " << event2.GetValue();
    Core::TheLogger().Debug("TestThread", stream.str());

    thread.join();

    Core::TheLogger().Debug("AutoEventTest", "WaitEventTimeout end");
}

TEST_FIXTURE(AutoEventTest, WaitEvent)
{
    Core::TheLogger().Debug("AutoEventTest", "WaitEvent start");

    AutoEvent event1;
    AutoEvent event2;
    eventHandle1 = (AutoEventHandle)event1;
    eventHandle2 = (AutoEventHandle)event2;

    std::thread thread(TestThread);

    event1.Set();
    event2.Wait();
    EXPECT_TRUE(eventPassed);

    thread.join();

    Core::TheLogger().Debug("AutoEventTest", "WaitEvent start");
}

} // namespace Test

} // namespace Core
