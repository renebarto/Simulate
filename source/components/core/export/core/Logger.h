#pragma once

#include <list>
#include <mutex>
#include "core/ILogger.h"

namespace Core
{

class ILogListener;

class Logger : virtual public ILogger
{
public:
    Logger() = delete;
    Logger(const std::string & domainName, LogLevel logLevelFilter);
    Logger(const Logger &) = delete;
    virtual ~Logger();

    Logger & operator = (const Logger &) = delete;

    void Error(const std::string & componentName, const std::string & message);
    void Error(const std::string & componentName, const std::string & message,
               const std::exception & exception);
    void Warn(const std::string & componentName, const std::string & message);
    void Warn(const std::string & componentName, const std::string & message,
              const std::exception & exception);
    void Message(const std::string & componentName, const std::string & message);
    void Info(const std::string & componentName, const std::string & message);
    void Debug(const std::string & componentName, const std::string & message);

    LogLevel GetLogLevelFilter();
    void SetLogLevelFilter(LogLevel logLevelFilter);

protected:
    typedef std::recursive_mutex Mutex;
    typedef std::lock_guard<Mutex> Lock;

    std::string domainName;
    LogHandlerInfo originalLogHandler;
    Mutex listenersGuard;

    void Log(const std::string & componentName, LogLevel logLevel, const std::string & message,
             const std::exception * exception = nullptr);
    void OnLog(const std::string & domainName,
               const std::string & componentName,
               LogLevel logLevel,
               const std::string & message);

    static void Handler(const std::string & domainName,
                        const std::string & componentName,
                        LogLevel logLevel,
                        const std::string & message,
                        void * userData);
};

} // namespace Core
