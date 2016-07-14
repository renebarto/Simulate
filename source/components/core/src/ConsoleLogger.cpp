#include "core/ConsoleLogger.h"

using namespace std;
using namespace OSAL;
using namespace Core;

ConsoleLogger::ConsoleLogger(ILogger & logger, Console & console, bool autoRegister/* = true*/)
    : logger(logger)
    , console(console)
{
    if (autoRegister)
        Register();
}

ConsoleLogger::~ConsoleLogger()
{
    UnRegister();
}

ConsoleColor ColorForLogLevel(LogLevel logLevel)
{
    switch (logLevel)
    {
    case LogLevel::Error:
        return ConsoleColor::Red | ConsoleColor::Intensity;
    case LogLevel::Warning:
        return ConsoleColor::Yellow | ConsoleColor::Intensity;
    case LogLevel::Message:
        return ConsoleColor::Magenta | ConsoleColor::Intensity;
    case LogLevel::Info:
        return ConsoleColor::Green;
    case LogLevel::Debug:
        return ConsoleColor::Cyan;
    default:
        break;
    }
    return ConsoleColor::Default;
}

void ConsoleLogger::Register()
{
    logger.AddObserver(this);
}

void ConsoleLogger::UnRegister()
{
    logger.RemoveObserver(this);
}

void ConsoleLogger::Log(const DateTime & timestamp,
                        const std::string & domainName,
                        const std::string & componentName,
                        LogLevel logLevel,
                        const std::string & message)
{
    console << fgcolor(ColorForLogLevel(logLevel))
            << timestamp << " - "
            << (domainName.empty() ? "Default" : domainName) << ": "
            << componentName << ": " << message
            << fgcolor(ConsoleColor::Default) << endl;
}

