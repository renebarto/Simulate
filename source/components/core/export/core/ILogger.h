#pragma once

#include <list>
#include "core/Observable.h"
#include "core/Util.h"
#include "core/LogHandler.h"

namespace Core
{

class ILogListener;

class ILogger : public Observable<ILogListener>
{
public:
    virtual ~ILogger() {}

    virtual LogLevel GetLogLevelFilter() = 0;
    virtual void SetLogLevelFilter(LogLevel logLevelFilter) = 0;
};

} // namespace Core
