#pragma once

#include <string>
#include <iostream>
#include <map>
#include <mutex>
#include "osal/flagoperators.h"
#include "core/Util.h"

namespace Core
{

enum class LogLevel : uint8_t
{
    Fatal = 1 << 1,
    Recursion = 1 << 2,
    Error = 1 << 3,
    Warning = 1 << 4,
    Message = 1 << 5,
    Info = 1 << 6,
    Debug = 1 << 7,
    All = Error | Warning | Message | Info | Debug,
    None = 0x00,
};

DEFINE_FLAG_OPERATORS(LogLevel, uint8_t);

std::ostream & operator << (std::ostream & stream, LogLevel logLevel);

typedef void LogHandlerFunction(const std::string & domainName,
                                const std::string & componentName,
                                LogLevel logLevel,
                                const std::string & message,
                                void * userData);

struct LogHandlerInfo
{
    LogHandlerInfo()
        : logLevelFilter(LogLevel::All)
        , handler(nullptr)
        , userData(nullptr)
    {
    }
    LogHandlerInfo(LogLevel logLevelFilter, LogHandlerFunction * handler, void * userData)
        : logLevelFilter(logLevelFilter)
        , handler(handler)
        , userData(userData)
    {
    }
    bool IsFilteredOut(LogLevel logLevel)
    {
        return (logLevel & this->logLevelFilter) == LogLevel::None;
    }
    void Set(LogLevel logLevelFilter, LogHandlerFunction * handler, void * userData)
    {
        this->logLevelFilter = logLevelFilter;
        this->handler = handler;
        this->userData = userData;
    }
    void Set(const LogHandlerInfo & other)
    {
        this->logLevelFilter = other.logLevelFilter;
        this->handler = other.handler;
        this->userData = other.userData;
    }

    LogLevel logLevelFilter;
    LogHandlerFunction * handler;
    void * userData;
};

class LogHandler
{
public:
    static const std::string DefaultDomain;
    static LogHandlerInfo Set(const std::string & domainName,
                              LogLevel logLevelFilter,
                              LogHandlerFunction * newHandler,
                              void * userData);
    static LogHandlerInfo Set(const std::string & domainName, const LogHandlerInfo & handlerInfo);
    static void Reset(const std::string & domainName);
    static LogLevel GetLogLevelFilter(const std::string & domainName);
    static void SetLogLevelFilter(const std::string & domainName, LogLevel logLevelFilter);

    static void Log(const std::string & domainName,
                    const std::string & componentName,
                    LogLevel logLevel,
                    const std::string & message);
    static bool IsFilteredOut(LogLevel logLevel);

private:
    typedef std::recursive_mutex Mutex;
    typedef std::lock_guard<Mutex> Lock;

    static LogHandlerInfo defaultLogHandlerInfo;
    static std::map<std::string, LogHandlerInfo> logHandlerInfo;
    static Mutex guard;

    static LogHandlerInfo * FindInfo(const std::string & domainName);
    static LogHandlerInfo * FindOrCreateInfo(const std::string & domainName);

    static void DefaultLogHandler(const std::string & domainName,
                                  const std::string & componentName,
                                  LogLevel logLevel,
                                  const std::string & message,
                                  void * userData);
};

} // namespace Core
