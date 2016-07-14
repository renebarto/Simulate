#include "unit-test-c++/UnitTestC++.h"
#include "core/LogHandler.h"

using namespace std;

namespace Core
{

namespace Test
{

const string DomainName = "Domain";

class LogHandlerTest : public UnitTestCpp::TestFixture
{
public:
    virtual void SetUp();
    virtual void TearDown();

    static void MyHandlerDefault(const std::string & domainName,
                                 const std::string & componentName,
                                 LogLevel logLevel,
                                 const std::string & message,
                                 void * userData)
    {
        LogHandlerTest * pThis = reinterpret_cast<LogHandlerTest *>(userData);
        pThis->domainNameDefault = domainName;
        pThis->componentNameDefault = componentName;
        pThis->logLevelDefault = logLevel;
        pThis->messageDefault = message;
    }
    static void MyHandler(const std::string & domainName,
                          const std::string & componentName,
                          LogLevel logLevel,
                          const std::string & message,
                          void * userData)
    {
        LogHandlerTest * pThis = reinterpret_cast<LogHandlerTest *>(userData);
        pThis->domainName = domainName;
        pThis->componentName = componentName;
        pThis->logLevel = logLevel;
        pThis->message = message;
    }
    string domainNameDefault{};
    string componentNameDefault{};
    LogLevel logLevelDefault{};
    string messageDefault{};
    string domainName{};
    string componentName{};
    LogLevel logLevel{};
    string message{};
    LogHandlerInfo savedHandler;

    void ExpectDefault(const std::string & domainName,
                       const std::string & componentName,
                       LogLevel logLevel,
                       const std::string & message)
    {
        EXPECT_EQ(domainName, this->domainNameDefault);
        EXPECT_EQ(componentName, this->componentNameDefault);
        EXPECT_EQ(logLevel, this->logLevelDefault);
        EXPECT_EQ(message, this->messageDefault);
    }
    void ExpectDomain(const std::string & domainName,
                      const std::string & componentName,
                      LogLevel logLevel,
                      const std::string & message)
    {
        EXPECT_EQ(domainName, this->domainName);
        EXPECT_EQ(componentName, this->componentName);
        EXPECT_EQ(logLevel, this->logLevel);
        EXPECT_EQ(message, this->message);
    }
};

void LogHandlerTest::SetUp()
{
    savedHandler = LogHandler::Set(LogHandler::DefaultDomain, LogLevel::All, MyHandlerDefault, this);
}

void LogHandlerTest::TearDown()
{
    LogHandler::Set(LogHandler::DefaultDomain, savedHandler);
    LogHandler::Reset(DomainName);
}

TEST_FIXTURE(LogHandlerTest, DefaultHandler)
{
    LogHandler::Log(LogHandler::DefaultDomain, "LogHandlerTest", LogLevel::Error, "Should not be shown on console");
}

TEST_FIXTURE(LogHandlerTest, NonExistingDomainHandler)
{
    LogHandler::Log("Dummy", "LogHandlerTest", LogLevel::Error, "Should not be shown on console");
}

TEST_FIXTURE(LogHandlerTest, ResetDefaultDomainHandler)
{
    LogHandler::Reset(LogHandler::DefaultDomain);
    LogHandler::Log(LogHandler::DefaultDomain, "LogHandlerTest", LogLevel::Error, "Should be shown on cout");
}

TEST_FIXTURE(LogHandlerTest, ResetNonExistingDomainHandler)
{
    LogHandler::Reset("Dummy");
    LogHandler::Log("Dummy", "LogHandlerTest", LogLevel::Error, "Should not be shown on cout");
    LogHandler::Reset(LogHandler::DefaultDomain);
    LogHandler::Log("Dummy", "LogHandlerTest", LogLevel::Error, "Should be shown on cout");
}

TEST_FIXTURE(LogHandlerTest, SetDefaultHandler)
{
    string aComponentName = "LogHandlerTest";
    LogLevel aLogLevel = LogLevel::Error;
    string aMessage = "Message";

    LogHandler::Set(LogHandler::DefaultDomain, LogLevel::All, MyHandlerDefault, this);
    LogHandler::Log(DomainName, aComponentName, aLogLevel, aMessage);
    ExpectDefault(DomainName, aComponentName, aLogLevel, aMessage);
    ExpectDomain("", "", LogLevel::None, "");

    LogHandler::Log(LogHandler::DefaultDomain, aComponentName, aLogLevel, aMessage);
    ExpectDefault(LogHandler::DefaultDomain, aComponentName, aLogLevel, aMessage);
    ExpectDomain("", "", LogLevel::None, "");
}

TEST_FIXTURE(LogHandlerTest, SetDomainHandler)
{
    string aComponentName = "LogHandlerTest";
    LogLevel aLogLevel = LogLevel::Error;
    string aMessage = "Message";

    LogHandler::Set(DomainName, LogLevel::All, MyHandler, this);
    LogHandler::Log(DomainName, aComponentName, aLogLevel, aMessage);
    ExpectDefault("", "", LogLevel::None, "");
    ExpectDomain(DomainName, aComponentName, aLogLevel, aMessage);

    LogHandler::Log(LogHandler::DefaultDomain, aComponentName, aLogLevel, aMessage);
    ExpectDefault(LogHandler::DefaultDomain, aComponentName, aLogLevel, aMessage);
    ExpectDomain(DomainName, aComponentName, aLogLevel, aMessage);
}

TEST_FIXTURE(LogHandlerTest, SetDefaultHandlerInfo)
{
    string aComponentName = "LogHandlerTest";
    LogLevel aLogLevel = LogLevel::Error;
    string aMessage = "Message";

    LogHandlerInfo info(LogLevel::All, MyHandlerDefault, this);
    LogHandler::Set(LogHandler::DefaultDomain, info);
    LogHandler::Log(DomainName, aComponentName, aLogLevel, aMessage);
    ExpectDefault(DomainName, aComponentName, aLogLevel, aMessage);
    ExpectDomain("", "", LogLevel::None, "");

    LogHandler::Log(LogHandler::DefaultDomain, aComponentName, aLogLevel, aMessage);
    ExpectDefault(LogHandler::DefaultDomain, aComponentName, aLogLevel, aMessage);
    ExpectDomain("", "", LogLevel::None, "");
}

TEST_FIXTURE(LogHandlerTest, SetDomainHandlerInfo)
{
    string aComponentName = "LogHandlerTest";
    LogLevel aLogLevel = LogLevel::Error;
    string aMessage = "Message";

    LogHandlerInfo info(LogLevel::All, MyHandler, this);
    LogHandler::Set(DomainName, info);
    LogHandler::Log(DomainName, aComponentName, aLogLevel, aMessage);
    ExpectDefault("", "", LogLevel::None, "");
    ExpectDomain(DomainName, aComponentName, aLogLevel, aMessage);

    LogHandler::Log(LogHandler::DefaultDomain, aComponentName, aLogLevel, aMessage);
    ExpectDefault(LogHandler::DefaultDomain, aComponentName, aLogLevel, aMessage);
    ExpectDomain(DomainName, aComponentName, aLogLevel, aMessage);
}

TEST_FIXTURE(LogHandlerTest, SetDomainHandler2)
{
    string DomainName2 = "Domain2";
    string aComponentName = "LogHandlerTest";
    LogLevel aLogLevel = LogLevel::Error;
    string aMessage1 = "Message1";
    string aMessage2 = "Message2";

    LogHandler::Set(DomainName, LogLevel::All, MyHandler, this);
    LogHandler::Log(DomainName, aComponentName, aLogLevel, aMessage1);
    LogHandler::Log(DomainName2, aComponentName, aLogLevel, aMessage2);
    ExpectDefault(DomainName2, aComponentName, aLogLevel, aMessage2);
    ExpectDomain(DomainName, aComponentName, aLogLevel, aMessage1);

    LogHandler::Set(DomainName2, LogLevel::All, MyHandler, this);
    LogHandler::Log(DomainName, aComponentName, aLogLevel, aMessage1);
    ExpectDomain(DomainName, aComponentName, aLogLevel, aMessage1);
    LogHandler::Log(DomainName2, aComponentName, aLogLevel, aMessage2);
    ExpectDomain(DomainName2, aComponentName, aLogLevel, aMessage2);
}

TEST_FIXTURE(LogHandlerTest, ResetDomainHandler)
{
    string aComponentName = "LogHandlerTest";
    LogLevel aLogLevel = LogLevel::Error;
    string aMessage1 = "Message1";
    string aMessage2 = "Message2";

    LogHandler::Set(DomainName, LogLevel::All, MyHandler, this);
    LogHandler::Log(DomainName, aComponentName, aLogLevel, aMessage1);
    ExpectDefault("", "", LogLevel::None, "");
    ExpectDomain(DomainName, aComponentName, aLogLevel, aMessage1);

    LogHandler::Reset(DomainName);
    LogHandler::Log(DomainName, aComponentName, aLogLevel, aMessage2);
    ExpectDefault(DomainName, aComponentName, aLogLevel, aMessage2);
    ExpectDomain(DomainName, aComponentName, aLogLevel, aMessage1);
}

TEST_FIXTURE(LogHandlerTest, GetSetLogLevelFilterDefault)
{
    LogHandler::Set(LogHandler::DefaultDomain, LogLevel::Error, MyHandlerDefault, this);
    EXPECT_EQ(LogLevel::Error, LogHandler::GetLogLevelFilter(LogHandler::DefaultDomain));

    LogHandler::SetLogLevelFilter(LogHandler::DefaultDomain, LogLevel::Warning);
    EXPECT_EQ(LogLevel::Warning, LogHandler::GetLogLevelFilter(LogHandler::DefaultDomain));
}

TEST_FIXTURE(LogHandlerTest, GetSetLogLevelFilterDomain)
{
    LogHandler::Set(DomainName, LogLevel::Error, MyHandler, this);
    EXPECT_EQ(LogLevel::Error, LogHandler::GetLogLevelFilter(DomainName));

    LogHandler::SetLogLevelFilter(DomainName, LogLevel::Warning);
    EXPECT_EQ(LogLevel::Warning, LogHandler::GetLogLevelFilter(DomainName));
}

TEST_FIXTURE(LogHandlerTest, SetLogLevelFiltered)
{
    string aComponentName = "LogHandlerTest";
    LogLevel aLogLevel = LogLevel::Warning;
    string aMessage = "Message";

    LogHandler::Set(DomainName, LogLevel::Error, MyHandler, this);
    LogHandler::Log(DomainName, aComponentName, aLogLevel, aMessage);
    ExpectDefault("", "", LogLevel::None, "");
    ExpectDomain("", "", LogLevel::None, "");

    LogHandler::SetLogLevelFilter(DomainName, LogLevel::Warning);
    LogHandler::Log(DomainName, aComponentName, aLogLevel, aMessage);
    ExpectDefault("", "", LogLevel::None, "");
    ExpectDomain(DomainName, aComponentName, aLogLevel, aMessage);
}

} // namespace Test

} // namespace Core
