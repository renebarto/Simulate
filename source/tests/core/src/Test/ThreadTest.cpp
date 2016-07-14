#include "unit-test-c++/UnitTestC++.h"

#include "core/Exception.h"
#include "core/Thread.h"
#include "core/Util.h"

using namespace std;

namespace Core
{

namespace Test
{

class ThreadTest : public UnitTestCpp::TestFixture
{
public:
    static const int SLEEP = 50;
    static const int TIMEOUT = 50;

    virtual void SetUp();
    virtual void TearDown();

    static bool havethread;
    static Thread * thread;

    static void TestThread(void * arg)
    {
        *(bool *)arg = true;
        Core::Util::Sleep(SLEEP);
    }
};

Thread * ThreadTest::thread;

void ThreadTest::SetUp()
{
    thread = nullptr;
}

void ThreadTest::TearDown()
{
    delete thread;
}

TEST_FIXTURE(ThreadTest, Construction)
{
    thread = new Thread();
    EXPECT_FALSE(thread->IsAlive());
    EXPECT_FALSE(thread->IsRunning());
    EXPECT_FALSE(thread->IsFinished());
    EXPECT_EQ("", thread->GetName());
    thread->WaitForDeath();
    EXPECT_FALSE(thread->IsAlive());
    EXPECT_FALSE(thread->IsRunning());
    EXPECT_FALSE(thread->IsFinished());
    EXPECT_FALSE(thread->HasDied());
}

TEST_FIXTURE(ThreadTest, ConstructionInitFunctionParam)
{
    bool flag = false;
    thread = new Thread(TestThread, &flag);
    EXPECT_TRUE(thread->IsAlive());
    EXPECT_TRUE(thread->IsRunning());
    EXPECT_FALSE(thread->IsFinished());
    EXPECT_EQ("", thread->GetName());
    Core::Util::Sleep(SLEEP);
    EXPECT_TRUE(flag);
    Core::Util::Sleep(SLEEP);
    thread->WaitForDeath();
    EXPECT_FALSE(thread->IsAlive());
    EXPECT_FALSE(thread->IsRunning());
    EXPECT_TRUE(thread->IsFinished());
    EXPECT_TRUE(thread->HasDied());
    EXPECT_EQ("", thread->GetName());
}

TEST_FIXTURE(ThreadTest, ConstructionInitFunctionParamName)
{
    bool flag = false;
    string name = "MyThread";
    thread = new Thread(TestThread, &flag, name);
    EXPECT_TRUE(thread->IsAlive());
    EXPECT_TRUE(thread->IsRunning());
    EXPECT_FALSE(thread->IsFinished());
    EXPECT_EQ(name, thread->GetName());
    Core::Util::Sleep(SLEEP);
    EXPECT_TRUE(flag);
    Core::Util::Sleep(SLEEP);
    thread->WaitForDeath();
    EXPECT_FALSE(thread->IsAlive());
    EXPECT_FALSE(thread->IsRunning());
    EXPECT_TRUE(thread->IsFinished());
    EXPECT_TRUE(thread->HasDied());
    EXPECT_EQ(name, thread->GetName());
}

TEST_FIXTURE(ThreadTest, Create)
{
    bool flag = false;
    thread = new Thread();
    EXPECT_FALSE(thread->IsAlive());
    EXPECT_FALSE(thread->IsRunning());
    EXPECT_FALSE(thread->IsFinished());
    EXPECT_EQ("", thread->GetName());
    dynamic_cast<Thread *>(thread)->CreateThread(TestThread, &flag);
    EXPECT_TRUE(thread->IsAlive());
    EXPECT_TRUE(thread->IsRunning());
    EXPECT_FALSE(thread->IsFinished());
    EXPECT_EQ("", thread->GetName());
    Core::Util::Sleep(SLEEP);
    EXPECT_TRUE(flag);
    Core::Util::Sleep(SLEEP);
    thread->WaitForDeath();
    EXPECT_FALSE(thread->IsAlive());
    EXPECT_FALSE(thread->IsRunning());
    EXPECT_TRUE(thread->IsFinished());
    EXPECT_TRUE(thread->HasDied());
    EXPECT_EQ("", thread->GetName());
}

TEST_FIXTURE(ThreadTest, CreateName)
{
    bool flag = false;
    string name = "MyThread";
    thread = new Thread();
    EXPECT_FALSE(thread->IsAlive());
    EXPECT_FALSE(thread->IsRunning());
    EXPECT_FALSE(thread->IsFinished());
    EXPECT_EQ("", thread->GetName());
    dynamic_cast<Thread *>(thread)->CreateThread(TestThread, &flag, name);
    EXPECT_TRUE(thread->IsAlive());
    EXPECT_TRUE(thread->IsRunning());
    EXPECT_FALSE(thread->IsFinished());
    EXPECT_EQ(name, thread->GetName());
    Core::Util::Sleep(SLEEP);
    EXPECT_TRUE(flag);
    Core::Util::Sleep(SLEEP);
    thread->WaitForDeath();
    EXPECT_FALSE(thread->IsAlive());
    EXPECT_FALSE(thread->IsRunning());
    EXPECT_TRUE(thread->IsFinished());
    EXPECT_TRUE(thread->HasDied());
    EXPECT_EQ(name, thread->GetName());
}

TEST_FIXTURE(ThreadTest, Destroy)
{
    bool flag = false;
    thread = new Thread(TestThread, &flag);
    EXPECT_TRUE(thread->IsAlive());
    EXPECT_TRUE(thread->IsRunning());
    EXPECT_FALSE(thread->IsFinished());
    EXPECT_EQ("", thread->GetName());
    Core::Util::Sleep(SLEEP);
    EXPECT_TRUE(flag);
    thread->DestroyThread();
    EXPECT_FALSE(thread->IsAlive());
    EXPECT_FALSE(thread->IsRunning());
    EXPECT_FALSE(thread->IsFinished());
    EXPECT_TRUE(thread->HasDied());
    EXPECT_EQ("", thread->GetName());
}

} // namespace Test

} // namespace Core
