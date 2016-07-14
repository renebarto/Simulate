#pragma once

#include <memory>
#include <vector>
#include "core/Util.h"

namespace Core
{

enum class CommandLineArgumentType
{
    NoArgument,
    RequiredArgument,
    OptionalArgument,
};

std::ostream & operator << (std::ostream & stream, CommandLineArgumentType type);

class CommandLineOption
{
public:
    CommandLineOption() = delete;
    CommandLineOption(const std::string & longName, char shortName, const std::string & description,
                      CommandLineArgumentType argType = CommandLineArgumentType::NoArgument);
    CommandLineOption(const std::string & longName, char shortName, const std::string & description,
                      bool * optionFoundFlag,
                      CommandLineArgumentType argType = CommandLineArgumentType::NoArgument);
    CommandLineOption(const CommandLineOption & other);
    virtual ~CommandLineOption();

    CommandLineOption & operator = (const CommandLineOption & other);

    const std::string & LongName() const
    {
        return longName;
    }
    void LongName(const std::string & name)
    {
        longName = name;
    }
    char ShortName() const
    {
        return shortName;
    }
    void ShortName(char name)
    {
        shortName = name;
    }
    const std::string & Description() const
    {
        return description;
    }
    void Description(const std::string & description)
    {
        this->description = description;
    }
    CommandLineArgumentType ArgType() const
    {
        return argType;
    }
    bool FoundOption() const
    {
        return optionFound;
    }
    void FoundOption(bool optionFound)
    {
        this->optionFound = optionFound;
        if (optionFoundFlag != nullptr)
            *optionFoundFlag = optionFound;
    }
    bool * OptionFoundFlag() const
    {
        return optionFoundFlag;
    }
    void OptionFoundFlag(bool * optionFoundFlag)
    {
        this->optionFoundFlag = optionFoundFlag;
    }

    std::string Value()
    {
        return value;
    }
    virtual void Value(const std::string & value)
    {
        this->value = value;
    }

private:
    std::string longName;
    char shortName;
    std::string description;
    CommandLineArgumentType argType;
    bool optionFound;
    bool * optionFoundFlag;
    std::string value;
};

typedef std::shared_ptr<CommandLineOption> CommandLineOptionPtr;

template <class T> class CommandLineOptionWithArgument : public CommandLineOption
{
public:
    CommandLineOptionWithArgument() = delete;
    CommandLineOptionWithArgument(const std::string & name, char shortName,
                                  const std::string & description,
                                  T * argument, bool requiredArgument = true)
        : CommandLineOption(name, shortName, description,
                            requiredArgument
                                ? CommandLineArgumentType::RequiredArgument
                                : CommandLineArgumentType::OptionalArgument)
        , argument(argument)
    {
    }
    CommandLineOptionWithArgument(const std::string & name, char shortName,
                                  const std::string & description,
                                  bool * optionFound, T * argument, bool requiredArgument = true)
        : CommandLineOption(name, shortName, description, optionFound,
                            requiredArgument
                                ? CommandLineArgumentType::RequiredArgument
                                : CommandLineArgumentType::OptionalArgument)
        , argument(argument)
    {
    }
    CommandLineOptionWithArgument(const CommandLineOptionWithArgument & other)
        : CommandLineOption(other)
    {
        this->argument = other.argument;
    }
    virtual ~CommandLineOptionWithArgument()
    {
    }

    CommandLineOptionWithArgument & operator = (const CommandLineOptionWithArgument & other)
    {
        if (this != &other)
        {
            CommandLineOption::operator=(other);
            this->argument = other.argument;
        }

        return *this;
    }

    T * Argument()
    {
        return argument;
    }
    const T * Argument() const
    {
        return argument;
    }

    virtual void Value(const std::string & value)
    {
        CommandLineOption::Value(value);
        if (argument == nullptr)
            return;
        if (!Core::Util::TryParse(value, *argument))
        {
            *argument = T();
        }
    }

private:
    T * argument;
};

} // namespace Core
