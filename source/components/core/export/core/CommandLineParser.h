#pragma once

#include "core/CommandLineOptionGroup.h"
#include "core/Exception.h"
#include "core/Util.h"

namespace Core
{

class CommandLineParserException : public Exception
{
public:
    CommandLineParserException(char const * functionName, char const * fileName, int line)
        : Exception(functionName, fileName, line)
    {
    }
};

class CommandLineParserInvalidArgumentType : public CommandLineParserException
{
protected:
    CommandLineOptionPtr option;
public:
    CommandLineParserInvalidArgumentType(char const * functionName, char const * fileName, int line,
                                         CommandLineOptionPtr option)
        : CommandLineParserException(functionName, fileName, line)
        , option(option)
    {
    }
    virtual std::string BuildMessage() const
    {
        std::ostringstream stream;
        stream << "Option longname=" << option->LongName() << " shortname=" << option->ShortName()
               << " has invalid argument type:" << (int)option->ArgType() << std::endl;
        return stream.str();
    }
};

class CommandLineParserArgumentException : public CommandLineParserException
{
protected:
    int argc;
    char * const * argv;
    const std::string optionName;
public:
    CommandLineParserArgumentException(char const * functionName, char const * fileName, int line,
                                       int argc, char * const * argv,
                                       const std::string & optionName)
        : CommandLineParserException(functionName, fileName, line)
        , argc(argc)
        , argv(argv)
        , optionName(optionName)
    {
    }
    virtual std::string BuildMessage() const
    {
        std::ostringstream stream;
        stream << "Arguments:" << std::endl;
        for (int i = 0; i < argc; i++)
        {
            stream << argv[i] << " ";
        }
        stream << std::endl;
        return stream.str();
    }
};

class CommandLineParserUnexpectedArgumentException : public CommandLineParserArgumentException
{
public:
    CommandLineParserUnexpectedArgumentException(char const * functionName, char const * fileName,
                                                 int line, int argc, char * const * argv,
                                                 const std::string & optionName)
        : CommandLineParserArgumentException(functionName, fileName, line, argc, argv, optionName)
    {
    }
    virtual std::string BuildMessage() const
    {
        std::ostringstream stream;
        stream << CommandLineParserArgumentException::BuildMessage();
        stream << "option doesn't take an argument -- " << optionName << std::endl;
        return stream.str();
    }
};

class CommandLineParserExpectedArgumentException : public CommandLineParserArgumentException
{
public:
    CommandLineParserExpectedArgumentException(char const * functionName, char const * fileName,
                                               int line, int argc, char * const * argv,
                                               const std::string & optionName) :
        CommandLineParserArgumentException(functionName, fileName, line, argc, argv, optionName)
    {
    }
    virtual std::string BuildMessage() const
    {
        std::ostringstream stream;
        stream << CommandLineParserArgumentException::BuildMessage();
        stream << "option requires an argument -- " << optionName << std::endl;
        return stream.str();
    }
};

class CommandLineParserUnknownOptionException : public CommandLineParserArgumentException
{
public:
    CommandLineParserUnknownOptionException(char const * functionName, char const * fileName, int line,
                                            int argc, char * const * argv,
                                            const std::string & optionName) :
        CommandLineParserArgumentException(functionName, fileName, line, argc, argv, optionName)
    {
    }
    virtual std::string BuildMessage() const
    {
        std::ostringstream stream;
        stream << CommandLineParserArgumentException::BuildMessage();
        stream << "unknown option -- " << optionName << std::endl;
        return stream.str();
    }
};

class CommandLineParserCannotGroupException : public CommandLineParserArgumentException
{
public:
    CommandLineParserCannotGroupException(char const * functionName, char const * fileName,
                                          int line, int argc, char * const * argv,
                                          const std::string & optionName) :
        CommandLineParserArgumentException(functionName, fileName, line, argc, argv, optionName)
    {
    }
    virtual std::string BuildMessage() const
    {
        std::ostringstream stream;
        stream << CommandLineParserArgumentException::BuildMessage();
        stream << "cannot group option -- " << optionName
               << ", has optional or required parameter" << std::endl;
        return stream.str();
    }
};

enum class CommandLineParserFlags : uint8_t
{
    None = 0x00,
    Permute = 0x01,        /* Permute non-options to the end of argv */
    AllArgs = 0x02,        /* Treat non-options as args to option "-1" */
    LongOnly = 0x04,    /* Operate as getopt_long_only */
};

struct OptionDefinition;

typedef std::vector<CommandLineOptionGroupPtr> CommandLineOptionGroupList;

class CommandLineParser
{
public:
    static const std::string DefaultMainOptionsGroupName;
    static const std::string HelpOptionsGroupName;

    CommandLineParser(const std::string & name = DefaultMainOptionsGroupName,
                      const std::string & description = "");
    CommandLineParser(const CommandLineParser &) = delete;
    virtual ~CommandLineParser();

    CommandLineParser & operator = (const CommandLineParser &) = delete;

    void Parse(int argc, char * const * argv);
    int NumNonOptions()
    {
        return (int)nonOptions.size();
    }
    const std::vector< std::string > & NonOptions()
    {
        return nonOptions;
    }

    void AddOptionNoArgument(const std::string & name, char shortName,
                             const std::string & description)
    {
        MainGroup()->AddOptionNoArgument(name, shortName, description);
    }

    void AddOptionNoArgument(const std::string & name, char shortName,
                             const std::string & description, bool * optionFound)
    {
        MainGroup()->AddOptionNoArgument(name, shortName, description, optionFound);
    }

    template <class T> void AddOptionRequiredArgument(const std::string & name, char shortName,
            const std::string & description, T * argument)
    {
        MainGroup()->AddOptionRequiredArgument(name, shortName, description, argument);
    }

    template <class T> void AddOptionRequiredArgument(const std::string & name, char shortName,
            const std::string & description,
            bool * optionFound, T * argument)
    {
        MainGroup()->AddOptionRequiredArgument(name, shortName, description, optionFound, argument);
    }

    template <class T> void AddOptionOptionalArgument(const std::string & name, char shortName,
            const std::string & description, T * argument)
    {
        MainGroup()->AddOptionOptionalArgument(name, shortName, description, argument);
    }

    template <class T> void AddOptionOptionalArgument(const std::string & name, char shortName,
            const std::string & description,
            bool * optionFound, T * argument)
    {
        MainGroup()->AddOptionOptionalArgument(name, shortName, description, optionFound, argument);
    }

    virtual void OnParseOption(const CommandLineOptionPtr option);
    virtual void OnParseParameter(const std::string parameter);

    void AddGroup(const CommandLineOptionGroupPtr group);
    const CommandLineOptionGroupPtr MainGroup() const;
    void MainGroup(const CommandLineOptionGroupPtr group);
    const CommandLineOptionGroupList & Groups() const
    {
        return groups;
    }

    bool AutoHandleHelp()
    {
        return autoHelp;
    }
    void AutoHandleHelp(bool autoHelp)
    {
        this->autoHelp = autoHelp;
    }
    bool ShouldShowHelp()
    {
        return showHelp;
    }
    std::string GetHelp(const std::string & applicationName, bool mainHelp = true);
    std::string GetHelp(const std::string & applicationName, bool mainHelp,
                        const CommandLineOptionGroupPtr group);

protected:
    CommandLineOptionGroupList groups;
    CommandLineOptionGroupPtr mainGroup;

private:
    std::string name;
    std::string description;
    std::vector< std::string > nonOptions;
    bool autoHelp;
    bool showHelp;

    bool IsInGroups(const CommandLineOptionGroupPtr group);
    void SetupStandardGroups();

    void InternalParse(int argc, char * const * argv);
    void HandleLongOption(int argc, char * const * argv);
    void HandleShortOption(int argc, char * const * argv);

    void FillOptionList();
    void GetNextArgument(char * const * argv);
    void SelectOption(CommandLineOptionPtr option, char const * value);
    void AddParameter(const std::string & parameter);

    int MatchShortOption(char name);
    int MatchLongOption(char const * name, size_t nameLength, bool allowShortOptions);

    char * currentArgumentPtr;
    int currentArgumentIndex;
    std::vector<CommandLineOptionPtr> optionList;
};

} // namespace Core
