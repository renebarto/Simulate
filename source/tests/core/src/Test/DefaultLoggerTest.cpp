#include "unit-test-c++/UnitTestC++.h"
#include "core/DefaultLogger.h"
#include "core/ILogListener.h"

using namespace std;

namespace Core
{

namespace Test
{

static const LogLevel LogLevelFilterAll = LogLevel::All;
static const LogLevel LogLevelFilterError = LogLevel::Error;
static const LogLevel LogLevelFilterWarning = LogLevel::Warning;
static const string ComponentName = "DefaultLoggerTest";
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

class DefaultLoggerListener : public ILogListener
{
public:
    DefaultLoggerListener()
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

class DefaultLoggerTest : public UnitTestCpp::TestFixture
{
public:
    DefaultLoggerTest()
        : logger(LogLevelFilterAll)
    {
    }

    virtual void SetUp();
    virtual void TearDown();

    DefaultLogger logger;
    DefaultLoggerListener listener;
};

void DefaultLoggerTest::SetUp()
{
}

void DefaultLoggerTest::TearDown()
{
    logger.RemoveObserver(&listener);
}

TEST_FIXTURE(DefaultLoggerTest, Construct)
{
    EXPECT_EQ(LogLevelFilterAll | LogLevel::Fatal | LogLevel::Recursion, logger.GetLogLevelFilter());
}

TEST_FIXTURE(DefaultLoggerTest, GetSetLogLevelFilter)
{
    logger.SetLogLevelFilter(LogLevelFilterError);
    EXPECT_EQ(LogLevelFilterError, logger.GetLogLevelFilter());
}

TEST_FIXTURE(DefaultLoggerTest, ErrorNoListener)
{
    logger.Error(ComponentName, Message);
    EXPECT_EQ(0, listener.logCount);
    EXPECT_EQ(string(), listener.domainName);
    EXPECT_EQ(string(), listener.componentName);
    EXPECT_EQ(LogLevel::None, listener.logLevel);
    EXPECT_EQ(string(), listener.message);
}

TEST_FIXTURE(DefaultLoggerTest, ErrorFiltered)
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

TEST_FIXTURE(DefaultLoggerTest, Error)
{
    logger.AddObserver(&listener);
    logger.Error(ComponentName, Message);
    EXPECT_EQ(1, listener.logCount);
    EXPECT_EQ(LogHandler::DefaultDomain, listener.domainName);
    EXPECT_EQ(ComponentName, listener.componentName);
    EXPECT_EQ(LogLevel::Error, listener.logLevel);
    EXPECT_EQ(Message, listener.message);
}

TEST_FIXTURE(DefaultLoggerTest, ErrorException)
{
    logger.AddObserver(&listener);
    logger.Error(ComponentName, Message, DummyException());
    EXPECT_EQ(1, listener.logCount);
    EXPECT_EQ(LogHandler::DefaultDomain, listener.domainName);
    EXPECT_EQ(ComponentName, listener.componentName);
    EXPECT_EQ(LogLevel::Error, listener.logLevel);
    EXPECT_EQ(Message + " (" + Exception + ")", listener.message);
}

TEST_FIXTURE(DefaultLoggerTest, Warn)
{
    logger.AddObserver(&listener);
    logger.Warn(ComponentName, Message);
    EXPECT_EQ(1, listener.logCount);
    EXPECT_EQ(LogHandler::DefaultDomain, listener.domainName);
    EXPECT_EQ(ComponentName, listener.componentName);
    EXPECT_EQ(LogLevel::Warning, listener.logLevel);
    EXPECT_EQ(Message, listener.message);
}

TEST_FIXTURE(DefaultLoggerTest, WarnException)
{
    logger.AddObserver(&listener);
    logger.Warn(ComponentName, Message, DummyException());
    EXPECT_EQ(1, listener.logCount);
    EXPECT_EQ(LogHandler::DefaultDomain, listener.domainName);
    EXPECT_EQ(ComponentName, listener.componentName);
    EXPECT_EQ(LogLevel::Warning, listener.logLevel);
    EXPECT_EQ(Message + " (" + Exception + ")", listener.message);
}

TEST_FIXTURE(DefaultLoggerTest, Message)
{
    logger.AddObserver(&listener);
    logger.Message(ComponentName, Message);
    EXPECT_EQ(1, listener.logCount);
    EXPECT_EQ(LogHandler::DefaultDomain, listener.domainName);
    EXPECT_EQ(ComponentName, listener.componentName);
    EXPECT_EQ(LogLevel::Message, listener.logLevel);
    EXPECT_EQ(Message, listener.message);
}

TEST_FIXTURE(DefaultLoggerTest, Info)
{
    logger.AddObserver(&listener);
    logger.Info(ComponentName, Message);
    EXPECT_EQ(1, listener.logCount);
    EXPECT_EQ(LogHandler::DefaultDomain, listener.domainName);
    EXPECT_EQ(ComponentName, listener.componentName);
    EXPECT_EQ(LogLevel::Info, listener.logLevel);
    EXPECT_EQ(Message, listener.message);
}

TEST_FIXTURE(DefaultLoggerTest, Debug)
{
    logger.AddObserver(&listener);
    logger.Debug(ComponentName, Message);
    EXPECT_EQ(1, listener.logCount);
    EXPECT_EQ(LogHandler::DefaultDomain, listener.domainName);
    EXPECT_EQ(ComponentName, listener.componentName);
    EXPECT_EQ(LogLevel::Debug, listener.logLevel);
    EXPECT_EQ(Message, listener.message);
}

} // namespace Test

} // namespace Core
