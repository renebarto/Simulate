#pragma once

#include "osal/console.h"
#include "core/ILogListener.h"

namespace Core
{

class ConsoleLogger : public ILogListener
{
public:
    ConsoleLogger() = delete;
    ConsoleLogger(ILogger & logger, OSAL::Console & console, bool autoRegister = true);
    ConsoleLogger(const ConsoleLogger &) = delete;
    virtual ~ConsoleLogger();

    ConsoleLogger & operator = (const ConsoleLogger &) = delete;

    void Register();
    void UnRegister();

    virtual void Log(const DateTime & timestamp,
                     const std::string & domainName,
                     const std::string & componentName,
                     LogLevel logLevel,
                     const std::string & message);

protected:
    ILogger & logger;
    OSAL::Console & console;
};

} // namespace Core

