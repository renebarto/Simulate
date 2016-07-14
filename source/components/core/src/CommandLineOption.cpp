#include "core/CommandLineOption.h"

#include "core/Exception.h"

using namespace std;
using namespace Core;

ostream & Core::operator << (ostream & stream, CommandLineArgumentType type)
{
    switch (type)
    {
    case CommandLineArgumentType::NoArgument:
        stream << "No argument";
        break;
    case CommandLineArgumentType::OptionalArgument:
        stream << "Optional argument";
        break;
    case CommandLineArgumentType::RequiredArgument:
        stream << "Required argument";
        break;
    }
    return stream;
}

CommandLineOption::CommandLineOption(const string & longName, char shortName,
                                     const string & description,
                                     CommandLineArgumentType argType
                                     /* = CommandLineArgumentType::NoArgument */)
    : longName(longName)
    , shortName(shortName)
    , description(description)
    , argType(argType)
    , optionFound(false)
    , optionFoundFlag(nullptr)
{
}

CommandLineOption::CommandLineOption(const string & longName, char shortName,
                                     const string & description,
                                     bool * optionFoundFlag,
                                     CommandLineArgumentType argType
                                     /* = CommandLineArgumentType::NoArgument */)
    : longName(longName)
    , shortName(shortName)
    , description(description)
    , argType(argType)
    , optionFound(false)
    , optionFoundFlag(optionFoundFlag)
{
    if (optionFoundFlag != nullptr)
        *optionFoundFlag = false;
}

CommandLineOption::CommandLineOption(const CommandLineOption & other)
{
    operator = (other);
}

CommandLineOption::~CommandLineOption()
{
}

CommandLineOption & CommandLineOption::operator = (const CommandLineOption & other)
{
    if (this != &other)
    {
        this->longName = other.longName;
        this->shortName = other.shortName;
        this->description = other.description;
        this->argType = other.argType;
        this->optionFound = other.optionFound;
        this->optionFoundFlag = other.optionFoundFlag;
    }

    return *this;
}
