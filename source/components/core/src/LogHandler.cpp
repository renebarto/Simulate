#include "core/LogHandler.h"

#include "osal/unused.h"

using namespace std;
using namespace Core;

ostream & Core::operator << (ostream & stream, LogLevel logLevel)
{
    string str;
    if ((logLevel & LogLevel::Fatal) != LogLevel::None)
    {
        str += "Fatal";
    }
    if ((logLevel & LogLevel::Recursion) != LogLevel::None)
    {
        if (!str.empty())
            str += "|";
        str += "Recursion";
    }
    if ((logLevel & LogLevel::Error) != LogLevel::None)
    {
        if (!str.empty())
            str += "|";
        str += "Error";
    }
    if ((logLevel & LogLevel::Warning) != LogLevel::None)
    {
        if (!str.empty())
            str += "|";
        str += "Warning";
    }
    if ((logLevel & LogLevel::Debug) != LogLevel::None)
    {
        if (!str.empty())
            str += "|";
        str += "Debug";
    }
    if ((logLevel & LogLevel::Info) != LogLevel::None)
    {
        if (!str.empty())
            str += "|";
        str += "Info";
    }
    if ((logLevel & LogLevel::Message) != LogLevel::None)
    {
        if (!str.empty())
            str += "|";
        str += "Message";
    }
    if (logLevel == LogLevel::None)
    {
        str = "-";
    }
    return stream << str;
}

const string LogHandler::DefaultDomain {};

LogHandlerInfo LogHandler::defaultLogHandlerInfo(LogLevel::All, LogHandler::DefaultLogHandler,
                                                 nullptr);
map<string, LogHandlerInfo> LogHandler::logHandlerInfo;
recursive_mutex LogHandler::guard;

LogHandlerInfo LogHandler::Set(const string & domainName,
                               LogLevel logLevelFilter,
                               LogHandlerFunction * newHandler,
                               void * userData)
{
    lock_guard<recursive_mutex> lock(guard);
    LogHandlerInfo * info = FindOrCreateInfo(domainName);
    if (info != nullptr)
    {
        LogHandlerInfo result = *info;
        info->Set(logLevelFilter, newHandler, userData);
        return result;
    }
    return LogHandlerInfo();
}

LogHandlerInfo LogHandler::Set(const string & domainName, const LogHandlerInfo & handlerInfo)
{
    lock_guard<recursive_mutex> lock(guard);
    LogHandlerInfo * info = FindOrCreateInfo(domainName);
    if (info != nullptr)
    {
        LogHandlerInfo result = *info;
        info->Set(handlerInfo);
        return result;
    }
    return LogHandlerInfo();
}

void LogHandler::Reset(const string & domainName)
{
    lock_guard<recursive_mutex> lock(guard);
    if (domainName.empty())
    {
        defaultLogHandlerInfo.Set(LogLevel::All, DefaultLogHandler, nullptr);
        return;
    }
    LogHandlerInfo * info = FindInfo(domainName);
    if (info != nullptr)
    {
        logHandlerInfo.erase(domainName);
    }
}

LogLevel LogHandler::GetLogLevelFilter(const string & domainName)
{
    lock_guard<recursive_mutex> lock(guard);
    LogHandlerInfo * info = FindInfo(domainName);
    if (info != nullptr)
    {
        return info->logLevelFilter;
    }
    return LogLevel::None;
}

void LogHandler::SetLogLevelFilter(const string & domainName, LogLevel logLevelFilter)
{
    lock_guard<recursive_mutex> lock(guard);
    LogHandlerInfo * info = FindInfo(domainName);
    if (info != nullptr)
    {
        info->logLevelFilter = logLevelFilter;
    }
}

void LogHandler::Log(const string & domainName,
                     const string & componentName,
                     LogLevel logLevel,
                     const string & message)
{
    lock_guard<recursive_mutex> lock(guard);
    LogHandlerInfo * info = FindInfo(domainName);
    if (info == nullptr)
    {
        info = FindInfo(DefaultDomain);
    }
    if (!info->IsFilteredOut(logLevel))
    {
        if (info->handler != nullptr)
            info->handler(domainName, componentName, logLevel, message, info->userData);
        else
            DefaultLogHandler(domainName, componentName, logLevel, message, info->userData);
    }
}

LogHandlerInfo * LogHandler::FindInfo(const string & domainName)
{
    if (domainName.empty())
    {
        return &defaultLogHandlerInfo;
    }
    map<string, LogHandlerInfo>::iterator foundHandlerInfo = logHandlerInfo.find(domainName);
    if (foundHandlerInfo == logHandlerInfo.end())
    {
        return nullptr;
    }
    return &(foundHandlerInfo->second);
}

LogHandlerInfo * LogHandler::FindOrCreateInfo(const string & domainName)
{
    LogHandlerInfo * info = FindInfo(domainName);
    if (info == nullptr)
    {
        LogHandlerInfo info(LogLevel::All, nullptr, nullptr);
        logHandlerInfo.insert(pair<string, LogHandlerInfo>(domainName, info));
        return FindInfo(domainName);
    }
    return info;
}

void LogHandler::DefaultLogHandler(const string & domainName,
                                   const string & componentName,
                                   LogLevel logLevel,
                                   const string & message,
                                   void * UNUSED(userData))
{
    cout << "Domain : " << (domainName.empty() ? "-" : domainName)
         << " Component : " << componentName
         << " Level: " << logLevel << ": " << message << endl;
}

