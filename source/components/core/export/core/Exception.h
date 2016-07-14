#pragma once

#include <string>
#include <exception>
#include <sstream>
#include <iomanip>
#include <cstring>
#include "core/Util.h"

namespace Core
{

class BaseException : public std::exception
{
protected:
    std::string message;
    const std::exception * innerException;
    mutable std::string whatMessage;
public:
    BaseException()
        : message()
        , innerException(nullptr)
    {
    }
    BaseException(const BaseException & innerException)
        : message()
        , innerException(&innerException)
    {
    }
    // Not a copy constructor!!
    BaseException(const std::exception & innerException)
        : message()
        , innerException(&innerException)
    {
    }
    BaseException(const std::string & message)
        : message(message)
        , innerException(nullptr)
    {
    }
    BaseException(const std::string & message, const std::exception & innerException)
        : message(message)
        , innerException(&innerException)
    {
    }
    ~BaseException() throw ()
    {
    }
    BaseException & operator = (const BaseException & other)
    {
        message = other.message;
        innerException = other.innerException;
        return *this;
    }
    const char* what() const throw()
    {
        whatMessage = FormatMessage();
        return whatMessage.c_str();
    }
    virtual std::string BuildMessage() const
    {
        return "";
    }
    const std::string & GetMessage() const
    {
        return message;
    }
    void SetMessage(const std::string & value)
    {
        message = value;
    }
private:
    std::string FormatMessage() const
    {
        std::ostringstream stream;
        stream << OSAL::TypeName(*this);
        std::string buildMessage = BuildMessage();
        if (!buildMessage.empty())
            stream << " - " << BuildMessage();
        if (!message.empty())
            stream << ": " << message;
        if (innerException)
        {
            stream << std::endl << "-->" << innerException->what();
        }
        return stream.str();
    }
};

class Exception : public BaseException
{
protected:
    char const * functionName;
    char const * fileName;
    int line;
public:
    Exception() = delete;
    Exception(char const * functionName, char const * fileName, int line)
        : BaseException()
        , functionName(functionName)
        , fileName(fileName)
        , line(line)
    {
    }
    Exception(char const * functionName, char const * fileName, int line,
              const std::string & message)
        : BaseException(message)
        , functionName(functionName)
        , fileName(fileName)
        , line(line)
    {
    }
    Exception(char const * functionName, char const * fileName, int line,
              const std::string & message, const std::exception & innerException)
        : BaseException(message, innerException)
        , functionName(functionName)
        , fileName(fileName)
        , line(line)
    {
    }
    ~Exception() throw ()
    {
    }
    virtual std::string BuildMessage() const
    {
        std::ostringstream stream;
        if (functionName != nullptr)
            stream << "in " << functionName;
        if (fileName != nullptr)
        {
            stream << " [" << fileName;
            if (line > 0)
                stream << ":" << line;
            stream << "]";
        }
        return stream.str();
    }
};

class SystemError : public Exception
{
protected:
    int errorCode;
public:
    SystemError(char const * functionName, char const * fileName, int line, int errorCode)
        : Exception(functionName, fileName, line)
        , errorCode(errorCode)
    {
    }
    SystemError(char const * functionName, char const * fileName, int line, int errorCode, std::string message)
        : Exception(functionName, fileName, line, message)
        , errorCode(errorCode)
    {
    }
    ~SystemError() throw ()
    {
    }
    int GetErrorCode() const
    {
        return errorCode;
    }
    virtual std::string BuildMessage() const
    {
        std::ostringstream stream;
        stream << Exception::BuildMessage() << " errno=" << errorCode
               << " (0x" << std::hex << std::setw(8) << std::setfill('0') << errorCode << "): "
               << '"' << OSAL::strerror(errorCode) << '"';
        return stream.str();
    }
};

class RuntimeError : public Exception
{
public:
    RuntimeError(char const * functionName, char const * fileName, int line, std::string message)
        : Exception(functionName, fileName, line, message)
    {
    }
    ~RuntimeError() throw ()
    {
    }
};

class ArgumentException : public Exception
{
protected:
    std::string argument;
public:
    ArgumentException(char const * functionName, char const * fileName, int line,
                      std::string argument)
        : Exception(functionName, fileName, line)
        , argument(argument)
    {
    }
    ArgumentException(char const * functionName, char const * fileName, int line,
                      std::string argument, std::string message)
        : Exception(functionName, fileName, line, message)
        , argument(argument)
    {
    }
    ~ArgumentException() throw ()
    {
    }
    virtual std::string BuildMessage() const
    {
        std::ostringstream stream;
        stream << Exception::BuildMessage();
        if (!argument.empty())
            stream << ": argument: " << argument;
        return stream.str();
    }
};

class ArgumentOutOfRangeException : public ArgumentException
{
public:
    ArgumentOutOfRangeException(char const * functionName, char const * fileName, int line,
                                std::string argument)
        : ArgumentException(functionName, fileName, line, argument)
    {
    }
    ArgumentOutOfRangeException(char const * functionName, char const * fileName, int line,
                                std::string argument, std::string message)
        : ArgumentException(functionName, fileName, line, argument, message)
    {
    }
    ~ArgumentOutOfRangeException() throw ()
    {
    }
    virtual std::string BuildMessage() const
    {
        std::ostringstream stream;
        stream << Exception::BuildMessage();
        if (!argument.empty())
            stream << ": argument out of range: " << argument;
        return stream.str();
    }
};

class ArgumentNullException : public ArgumentException
{
public:
    ArgumentNullException(char const * functionName, char const * fileName, int line,
                          std::string argument) :
        ArgumentException(functionName, fileName, line, argument)
    {
    }
    ~ArgumentNullException() throw ()
    {
    }
    virtual std::string BuildMessage() const
    {
        std::ostringstream stream;
        stream << Exception::BuildMessage();
        if (!argument.empty())
            stream << ": argument null: " << argument;
        return stream.str();
    }
};

class OperationException : public Exception
{
public:
    OperationException(char const * functionName, char const * fileName, int line,
                       std::string message)
        : Exception(functionName, fileName, line, message)
    {
    }
    ~OperationException() throw ()
    {
    }
};

class OperationNotSupportedException : public OperationException
{
public:
    OperationNotSupportedException(char const * functionName, char const * fileName, int line,
                                   std::string message)
        : OperationException(functionName, fileName, line, message)
    {
    }
    ~OperationNotSupportedException() throw ()
    {
    }
    virtual std::string BuildMessage() const
    {
        std::ostringstream stream;
        stream << Exception::BuildMessage();
        stream << ": operation not supported";
        return stream.str();
    }
};

} // namespace Core
