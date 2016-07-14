#pragma once

#include <list>
#include <mutex>
#include "core/Logger.h"

namespace Core
{

class ILogListener;

class DefaultLogger : public Logger
{
public:
    DefaultLogger() = delete;
    DefaultLogger(LogLevel logLevelFilter)
        : Logger(LogHandler::DefaultDomain, logLevelFilter)
    {
    }
    DefaultLogger(const DefaultLogger &) = delete;

    DefaultLogger & operator = (const DefaultLogger &) = delete;
};

extern DefaultLogger & TheLogger();

} // namespace Core
