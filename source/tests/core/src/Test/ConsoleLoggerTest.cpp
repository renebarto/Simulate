#include "unit-test-c++/UnitTestC++.h"
#include "core/ConsoleLogger.h"
#include "core/DefaultLogger.h"

using namespace std;

namespace Core
{

namespace Test
{

static const string DomainName = "Domain";
static const LogLevel LogLevelFilterAll = LogLevel::All;
static const LogLevel LogLevelFilterError = LogLevel::Error;
static const LogLevel LogLevelFilterWarning = LogLevel::Warning;
static const string ComponentName = "ConsoleLoggerTest";
static const string Message = "Message";
static const char * Exception = "Exception";

class DummyException : public exception
{
public:
    const char* what() const noexcept
    {
        return Exception;
    }
};

class ConsoleLoggerTest : public UnitTestCpp::TestFixture
{
public:
    ConsoleLoggerTest()
        : console()
        , logger(TheLogger(), console)
    {
    }

    virtual void SetUp();
    virtual void TearDown();

    OSAL::Console console;
    ConsoleLogger logger;
};

void ConsoleLoggerTest::SetUp()
{
}

void ConsoleLoggerTest::TearDown()
{
}

TEST_FIXTURE(ConsoleLoggerTest, Error)
{
    TheLogger().Error(ComponentName, Message);
}

TEST_FIXTURE(ConsoleLoggerTest, ErrorException)
{
    TheLogger().Error(ComponentName, Message, DummyException());
}

TEST_FIXTURE(ConsoleLoggerTest, Warn)
{
    TheLogger().Warn(ComponentName, Message);
}

TEST_FIXTURE(ConsoleLoggerTest, WarnException)
{
    TheLogger().Warn(ComponentName, Message, DummyException());
}

TEST_FIXTURE(ConsoleLoggerTest, Message)
{
    TheLogger().Message(ComponentName, Message);
}

TEST_FIXTURE(ConsoleLoggerTest, Info)
{
    TheLogger().Info(ComponentName, Message);
}

TEST_FIXTURE(ConsoleLoggerTest, Debug)
{
    TheLogger().Debug(ComponentName, Message);
}

} // namespace Test

} // namespace Core
