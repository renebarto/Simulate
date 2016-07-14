#include "unit-test-c++/UnitTestC++.h"
#include "core/Logger.h"
#include "core/ILogListener.h"

using namespace std;
using namespace Core;

namespace Core
{

namespace Test
{

static const string DomainName = "Domain";
static const LogLevel LogLevelFilterAll = LogLevel::All;
static const LogLevel LogLevelFilterError = LogLevel::Error;
static const LogLevel LogLevelFilterWarning = LogLevel::Warning;
static const string ComponentName = "LoggerTest";
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

class LoggerListener : public ILogListener
{
public:
    LoggerListener()
    {
    }

    void Log(const DateTime & timestamp,
             const std::string & domainName,
             const std::string & componentName,
             LogLevel logLevel,
             const std::string & message)
    {
        ++this->logCount;
        this->timestamp = timestamp;
        this->domainName = domainName;
        this->componentName = componentName;
        this->logLevel = logLevel;
        this->message = message;
    }

    int logCount{};
    DateTime timestamp{};
    string domainName{};
    string componentName{};
    LogLevel logLevel{};
    string message{};
};

class LoggerTest : public UnitTestCpp::TestFixture
{
public:
    LoggerTest()
        : logger(DomainName, LogLevelFilterAll)
    {
    }

    virtual void SetUp();
    virtual void TearDown();

    Logger logger;
    LoggerListener listener;
};

void LoggerTest::SetUp()
{
}

void LoggerTest::TearDown()
{
    logger.RemoveObserver(&listener);
}

TEST_FIXTURE(LoggerTest, Construct)
{
    EXPECT_EQ(LogLevelFilterAll | LogLevel::Fatal | LogLevel::Recursion, logger.GetLogLevelFilter());
}

TEST_FIXTURE(LoggerTest, GetSetLogLevelFilter)
{
    logger.SetLogLevelFilter(LogLevelFilterError);
    EXPECT_EQ(LogLevelFilterError, logger.GetLogLevelFilter());
}

TEST_FIXTURE(LoggerTest, ErrorNoListener)
{
    logger.Error(ComponentName, Message);
    EXPECT_EQ(0, listener.logCount);
    EXPECT_EQ(string(), listener.domainName);
    EXPECT_EQ(string(), listener.componentName);
    EXPECT_EQ(LogLevel::None, listener.logLevel);
    EXPECT_EQ(string(), listener.message);
}

TEST_FIXTURE(LoggerTest, ErrorFiltered)
{
    logger.AddObserver(&listener);
    logger.SetLogLevelFilter(LogLevelFilterWarning);
    logger.Error(ComponentName, Message);
    EXPECT_EQ(0, listener.logCount);
    EXPECT_EQ(string(), listener.domainName);
    EXPECT_EQ(string(), listener.componentName);
    EXPECT_EQ(LogLevel::None, listener.logLevel);
    EXPECT_EQ(string(), listener.message);
}

TEST_FIXTURE(LoggerTest, Error)
{
    logger.AddObserver(&listener);
    logger.Error(ComponentName, Message);
    EXPECT_EQ(1, listener.logCount);
    EXPECT_EQ(DomainName, listener.domainName);
    EXPECT_EQ(ComponentName, listener.componentName);
    EXPECT_EQ(LogLevel::Error, listener.logLevel);
    EXPECT_EQ(Message, listener.message);
}

TEST_FIXTURE(LoggerTest, ErrorException)
{
    logger.AddObserver(&listener);
    logger.Error(ComponentName, Message, DummyException());
    EXPECT_EQ(1, listener.logCount);
    EXPECT_EQ(DomainName, listener.domainName);
    EXPECT_EQ(ComponentName, listener.componentName);
    EXPECT_EQ(LogLevel::Error, listener.logLevel);
    EXPECT_EQ(Message + " (" + Exception + ")", listener.message);
}

TEST_FIXTURE(LoggerTest, Warn)
{
    logger.AddObserver(&listener);
    logger.Warn(ComponentName, Message);
    EXPECT_EQ(1, listener.logCount);
    EXPECT_EQ(DomainName, listener.domainName);
    EXPECT_EQ(ComponentName, listener.componentName);
    EXPECT_EQ(LogLevel::Warning, listener.logLevel);
    EXPECT_EQ(Message, listener.message);
}

TEST_FIXTURE(LoggerTest, WarnException)
{
    logger.AddObserver(&listener);
    logger.Warn(ComponentName, Message, DummyException());
    EXPECT_EQ(1, listener.logCount);
    EXPECT_EQ(DomainName, listener.domainName);
    EXPECT_EQ(ComponentName, listener.componentName);
    EXPECT_EQ(LogLevel::Warning, listener.logLevel);
    EXPECT_EQ(Message + " (" + Exception + ")", listener.message);
}

TEST_FIXTURE(LoggerTest, Message)
{
    logger.AddObserver(&listener);
    logger.Message(ComponentName, Message);
    EXPECT_EQ(1, listener.logCount);
    EXPECT_EQ(DomainName, listener.domainName);
    EXPECT_EQ(ComponentName, listener.componentName);
    EXPECT_EQ(LogLevel::Message, listener.logLevel);
    EXPECT_EQ(Message, listener.message);
}

TEST_FIXTURE(LoggerTest, Info)
{
    logger.AddObserver(&listener);
    logger.Info(ComponentName, Message);
    EXPECT_EQ(1, listener.logCount);
    EXPECT_EQ(DomainName, listener.domainName);
    EXPECT_EQ(ComponentName, listener.componentName);
    EXPECT_EQ(LogLevel::Info, listener.logLevel);
    EXPECT_EQ(Message, listener.message);
}

TEST_FIXTURE(LoggerTest, Debug)
{
    logger.AddObserver(&listener);
    logger.Debug(ComponentName, Message);
    EXPECT_EQ(1, listener.logCount);
    EXPECT_EQ(DomainName, listener.domainName);
    EXPECT_EQ(ComponentName, listener.componentName);
    EXPECT_EQ(LogLevel::Debug, listener.logLevel);
    EXPECT_EQ(Message, listener.message);
}

} // namespace Test

} // namespace Core
