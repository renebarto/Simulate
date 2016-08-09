#include "unit-test-c++/UnitTestC++.h"

#include <thread>
#include "core/ManualEvent.h"
#include "core/Util.h"

namespace Core
{

namespace Test
{

class ManualEventTest : public UnitTestCpp::TestFixture
{
public:
    static const int SHORTSLEEP = 2;
    static const int SLEEP = 10;
    static const int TIMEOUT = 100;
    static const int WaitIterations = 10;

    virtual void SetUp();
    virtual void TearDown();

    static bool eventPassed;
    static ManualEvent * event1;
    static ManualEvent * event2;
    static ManualEvent * event3;
    static ManualEvent * event4;

    static void TestThread(void)
    {
        eventPassed = (event1 != nullptr) && event1->Wait(TIMEOUT);
        if (eventPassed)
        {
            if (event2 != nullptr)
                event2->Set();
            Core::Util::Sleep(SLEEP);
            if (event4 != nullptr)
                event4->Set();
        }
        if ((event3 != nullptr) && event3->Wait(TIMEOUT))
        {
            if (event2 != nullptr)
                event2->Reset();
        }
    }
};

bool ManualEventTest::eventPassed = false;
ManualEvent * ManualEventTest::event1;
ManualEvent * ManualEventTest::event2;
ManualEvent * ManualEventTest::event3;
ManualEvent * ManualEventTest::event4;

void ManualEventTest::SetUp()
{
    eventPassed = false;
    event1 = nullptr;
    event2 = nullptr;
    event3 = nullptr;
    event4 = nullptr;
}

void ManualEventTest::TearDown()
{
}

TEST_FIXTURE(ManualEventTest, Construction)
{
    ManualEvent event1;
    EXPECT_FALSE(event1.GetValue());
}

TEST_FIXTURE(ManualEventTest, SimpleEvent)
{
    ManualEvent target1;
    ManualEvent target2;
    event1 = &target1;
    event2 = &target2;

    std::thread thread(TestThread);

    target1.Set();
    Core::Util::Sleep(SLEEP);
    EXPECT_TRUE(target2.Wait(TIMEOUT));
    EXPECT_TRUE(eventPassed);

    thread.join();
}

TEST_FIXTURE(ManualEventTest, SetEvent)
{
    ManualEvent target1;
    ManualEvent target2;
    ManualEvent target3;
    ManualEvent target4;
    event1 = &target1;
    event2 = &target2;
    event3 = &target3;
    event4 = &target4;

    std::thread thread(TestThread);

    target1.Set();
    Core::Util::Sleep(SLEEP);
    EXPECT_TRUE(target1.Wait(TIMEOUT));
    EXPECT_TRUE(target2.Wait(TIMEOUT));
    EXPECT_TRUE(target4.Wait(TIMEOUT));

    target3.Set();
    Core::Util::Sleep(SLEEP);
    EXPECT_TRUE(target1.Wait(TIMEOUT));
    EXPECT_FALSE(target2.Wait(TIMEOUT));
    EXPECT_TRUE(target3.Wait(TIMEOUT));
    EXPECT_TRUE(target4.Wait(TIMEOUT));

    thread.join();
}

TEST_FIXTURE(ManualEventTest, WaitEventTimeout)
{
    ManualEvent target1;
    ManualEvent target2;
    event1 = &target1;
    event2 = &target2;

    std::thread thread(TestThread);

    EXPECT_FALSE(target2.Wait(TIMEOUT));

    thread.join();
}

TEST_FIXTURE(ManualEventTest, WaitEvent)
{
    ManualEvent target1;
    ManualEvent target2;
    event1 = &target1;
    event2 = &target2;

    std::thread thread(TestThread);

    target1.Set();
    target2.Wait();

    thread.join();
}

TEST_FIXTURE(ManualEventTest, WaitTimeoutManyTimes)
{
    ManualEvent target1;
    ManualEvent target2;
    event1 = &target1;
    event2 = &target2;

    for (int i = 0; i < WaitIterations; i++)
    {
        target1.Reset();

        std::thread thread(TestThread);

        EXPECT_FALSE(target2.Wait(TIMEOUT));

        thread.join();
    }
}

TEST_FIXTURE(ManualEventTest, WaitManyTimes)
{
    ManualEvent target1;
    ManualEvent target2;
    event1 = &target1;
    event2 = &target2;

    for (int i = 0; i < WaitIterations; i++)
    {
        target1.Reset();

        std::thread thread(TestThread);

        target1.Set();
        target2.Wait();

        thread.join();

        target2.Reset();
    }
}

} // namespace Test

} // namespace Core
