#pragma once

#include <vector>
#include "core/CommandLineOption.h"

namespace Core
{

typedef std::vector <CommandLineOptionPtr> CommandLineOptionsList;

class CommandLineOptionGroup
{
public:
    CommandLineOptionGroup() = delete;
    CommandLineOptionGroup(const std::string & name, const std::string description = "");
    CommandLineOptionGroup(const CommandLineOptionGroup &) = delete;
    virtual ~CommandLineOptionGroup();

    CommandLineOptionGroup & operator = (const CommandLineOptionGroup &) = delete;

    const std::string & Name() const
    {
        return name;
    }
    void Name(const std::string & name)
    {
        this->name = name;
    }
    const std::string & Description() const
    {
        return description;
    }
    void Description(const std::string & description)
    {
        this->description = description;
    }

    void Add(const CommandLineOptionPtr option);
    void AddOptionNoArgument(const std::string & name, char shortName,
                             const std::string & description)
    {
        Add(CommandLineOptionPtr(new CommandLineOption(name, shortName, description)));
    }
    void AddOptionNoArgument(const std::string & name, char shortName,
                             const std::string & description, bool * optionFound)
    {
        Add(CommandLineOptionPtr(new CommandLineOption(name, shortName, description, optionFound)));
    }

    template <class T> void AddOptionRequiredArgument(const std::string & name, char shortName,
            const std::string & description, T * argument)
    {
        Add(CommandLineOptionPtr(new CommandLineOptionWithArgument<T>(name, shortName, description,
                                 argument)));
    }

    template <class T> void AddOptionRequiredArgument(const std::string & name, char shortName,
            const std::string & description, bool * optionFound,
            T * argument)
    {
        Add(CommandLineOptionPtr(new CommandLineOptionWithArgument<T>(name, shortName, description,
                                 optionFound, argument)));
    }

    template <class T> void AddOptionOptionalArgument(const std::string & name, char shortName,
            const std::string & description, T * argument)
    {
        Add(CommandLineOptionPtr(new CommandLineOptionWithArgument<T>(name, shortName, description,
                                 argument, false)));
    }

    template <class T> void AddOptionOptionalArgument(const std::string & name, char shortName,
            const std::string & description,
            bool * optionFound, T * argument)
    {
        Add(CommandLineOptionPtr(new CommandLineOptionWithArgument<T>(name, shortName, description,
                                 optionFound, argument, false)));
    }

    const CommandLineOptionsList & Options() const
    {
        return options;
    }

protected:
    std::string name;
    std::string description;
    CommandLineOptionsList options;
};

typedef std::shared_ptr<CommandLineOptionGroup> CommandLineOptionGroupPtr;

} // namespace Core
