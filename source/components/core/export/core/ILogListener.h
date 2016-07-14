#pragma once

#include <string>
#include "core/Logger.h"
#include "core/DateTime.h"

namespace Core
{

class ILogListener
{
public:
    virtual ~ILogListener() {}

    virtual void Log(const DateTime & timestamp,
                     const std::string & domainName,
                     const std::string & componentName,
                     LogLevel logLevel,
                     const std::string & message) = 0;
};

} // namespace Core
