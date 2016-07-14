#include "core/CommandLineParser.h"

#include <iostream>
#include "osal/unused.h"
#include "core/Exception.h"

using namespace std;
using namespace Core;

const std::string CommandLineParser::DefaultMainOptionsGroupName = "Main";
const std::string CommandLineParser::HelpOptionsGroupName = "Help Options";

#define    EMSG        ""

struct OptionDefinition
{
    const char * longName;
    char shortName;
    CommandLineArgumentType argumentType;
    int val;
};

CommandLineParser::CommandLineParser(const string & name /* = DefaultMainOptionsGroupName*/,
                                     const string & description /* = ""*/)
    : groups()
    , mainGroup()
    , name(name)
    , description(description)
    , nonOptions()
    , autoHelp(true)
    , showHelp(false)
    , currentArgumentPtr(nullptr)
    , currentArgumentIndex(0)
    , optionList()
{
    SetupStandardGroups();
}

CommandLineParser::~CommandLineParser()
{
}

void CommandLineParser::Parse(int argc, char * const * argv)
{
    try
    {
        InternalParse(argc, argv);
        if (autoHelp && showHelp)
        {
            std::cerr << GetHelp(argv[0]);
            exit(0);
        }
    }
    catch (...)
    {
        if (autoHelp && showHelp)
        {
            std::cerr << GetHelp(argv[0]);
        }
        throw;
    }
}

void CommandLineParser::InternalParse(int argc, char * const * argv)
{
    currentArgumentIndex = 1;

    FillOptionList();

    while (currentArgumentIndex <= argc)
    {
        GetNextArgument(argv);

        if (currentArgumentPtr == nullptr)
            return;

        if (*currentArgumentPtr == '-')
        {
            currentArgumentPtr++;
            if (*currentArgumentPtr == '-')
            {
                currentArgumentPtr++;
                if (*currentArgumentPtr == '\0')
                {
                    AddParameter("--");
                }
                else
                {
                    HandleLongOption(argc, argv);
                }
            }
            else
            {
                if (*currentArgumentPtr == '\0')
                {
                    AddParameter("-");
                }
                else
                {
                    HandleShortOption(argc, argv);
                }
            }
        }
        else
        {
            AddParameter(currentArgumentPtr);
        }
    }
}

void CommandLineParser::HandleLongOption(int argc, char * const * argv)
{
    const char *positionEquals = strchr(currentArgumentPtr, '=');
    const char *positionArgument = nullptr;
    const char * currentArgument;
    size_t currentOptionNameLength = 0;

    bool haveEquals = (positionEquals != nullptr);
    if (haveEquals)
    {
        // --option=arg
        currentOptionNameLength = positionEquals - currentArgumentPtr;
        positionArgument = positionEquals + 1;
    }
    else
        currentOptionNameLength = strlen(currentArgumentPtr);
    string optionNameString = string(currentArgumentPtr, currentOptionNameLength);

    int match = MatchLongOption(currentArgumentPtr, currentOptionNameLength, true);
    if (match != -1)
    {
        CommandLineOptionPtr option = optionList[match];
        switch (option->ArgType())
        {
        case CommandLineArgumentType::NoArgument:
            if (positionArgument != nullptr)
            {
                throw CommandLineParserUnexpectedArgumentException(__func__, __FILE__, __LINE__,
                                                                   argc, argv, optionNameString);
            }
            SelectOption(option, nullptr);
            break;
        case CommandLineArgumentType::RequiredArgument:
            if (positionArgument != nullptr)
                currentArgument = positionArgument;
            else
            {
                GetNextArgument(argv);
                currentArgument = currentArgumentPtr;
            }
            if (currentArgument == nullptr)
            {
                throw CommandLineParserExpectedArgumentException(__func__, __FILE__, __LINE__,
                                                                 argc, argv, optionNameString);
            }
            SelectOption(option, currentArgument);
            break;
        case CommandLineArgumentType::OptionalArgument:
            if (positionArgument != nullptr)
                currentArgument = positionArgument;
            else
            {
                GetNextArgument(argv);
                currentArgument = currentArgumentPtr;
            }
            SelectOption(option, currentArgument);
            break;
        }
    }
    else
    {
        throw CommandLineParserUnknownOptionException(__func__, __FILE__, __LINE__,
                                                      argc, argv, optionNameString);
    }
}

void CommandLineParser::HandleShortOption(int argc, char * const * argv)
{
    char optionName = *currentArgumentPtr;
    string optionNameString = string(1, optionName);
    int match = MatchShortOption(optionName);

    currentArgumentPtr++;

    if (match != -1)
    {
        CommandLineOptionPtr option = optionList[match];
        switch (option->ArgType())
        {
        case CommandLineArgumentType::NoArgument:
            SelectOption(optionList[match], nullptr);
            while (*currentArgumentPtr != '\0')
            {
                int match = MatchShortOption(*currentArgumentPtr);
                if (match != -1)
                {
                    SelectOption(optionList[match], nullptr);
                }
                currentArgumentPtr++;
            }
            break;

        case CommandLineArgumentType::OptionalArgument:
            if (*currentArgumentPtr != '\0')
            {
                throw CommandLineParserCannotGroupException(__func__, __FILE__, __LINE__,
                                                            argc, argv, optionNameString);
            }

            GetNextArgument(argv);

            SelectOption(optionList[match], currentArgumentPtr);
            break;

        case CommandLineArgumentType::RequiredArgument:
            if (*currentArgumentPtr != '\0')
            {
                throw CommandLineParserCannotGroupException(__func__, __FILE__, __LINE__,
                                                            argc, argv, optionNameString);
            }

            GetNextArgument(argv);

            if (currentArgumentPtr == nullptr)
                throw CommandLineParserExpectedArgumentException(__func__, __FILE__, __LINE__,
                                                                 argc, argv, optionNameString);

            SelectOption(optionList[match], currentArgumentPtr);
            break;

        default:
            throw CommandLineParserInvalidArgumentType(__func__, __FILE__, __LINE__, option);
            break;
        }
    }
    else
    {
        throw CommandLineParserUnknownOptionException(__func__, __FILE__, __LINE__,
                                                      argc, argv, optionNameString);
    }
}

void CommandLineParser::FillOptionList()
{
    for (auto group : groups)
    {
        for (auto option : group->Options())
        {
            optionList.push_back(option);
        }
    }
}

void CommandLineParser::GetNextArgument(char * const * argv)
{
    currentArgumentPtr = argv[currentArgumentIndex];
    currentArgumentIndex++;
}

void CommandLineParser::SelectOption(CommandLineOptionPtr option, const char * value)
{
    option->FoundOption(true);
    switch (option->ArgType())
    {
    case CommandLineArgumentType::NoArgument:
        break;
    case CommandLineArgumentType::OptionalArgument:
        if (value != nullptr)
            option->Value(value);
        break;
    case CommandLineArgumentType::RequiredArgument:
        option->Value(value);
        break;
    }
    OnParseOption(option);
}

void CommandLineParser::AddParameter(const string & parameter)
{
    nonOptions.push_back(parameter);
}

void CommandLineParser::OnParseOption(const CommandLineOptionPtr UNUSED(option))
{
}

void CommandLineParser::OnParseParameter(const std::string UNUSED(parameter))
{
}

void CommandLineParser::AddGroup(const CommandLineOptionGroupPtr group)
{
    groups.push_back(group);
}

const CommandLineOptionGroupPtr CommandLineParser::MainGroup() const
{
    return mainGroup;
}

void CommandLineParser::MainGroup(const CommandLineOptionGroupPtr group)
{
    if (!IsInGroups(group))
    {
        AddGroup(group);
    }
    mainGroup = group;
}

string CommandLineParser::GetHelp(const std::string & applicationName, bool mainHelp /*= true*/)
{
    return GetHelp(applicationName, mainHelp, CommandLineOptionGroupPtr());
}

string CommandLineParser::GetHelp(const std::string & applicationName,
                                  bool UNUSED(mainHelp),
                                  const CommandLineOptionGroupPtr group)
{
    const int indentOptions = 2;
    const int indentDescription = 18;
    ostringstream stream;
    stream << "Usage:" << endl
           << string(indentOptions, ' ') << applicationName << " [OPTION...]" << endl << endl;
    for (auto foundGroup : groups)
    {
        if ((group != nullptr) && (group != foundGroup))
            continue;
        if (foundGroup->Options().size() > 0)
        {
            stream << foundGroup->Name() << ":" << endl;
            for (auto option : foundGroup->Options())
            {
                string optionsText = (option->ShortName() != 0)
                                     ? string("-") + option->ShortName() + ", "
                                     : "";
                optionsText += "--" + option->LongName();
                stream << string(indentOptions, ' ') << optionsText;
                if (optionsText.length() < indentDescription)
                    stream << string(indentDescription - optionsText.length(), ' ');
                stream << " " << option->Description() << endl;
            }
            stream << endl;
        }
    }
    return stream.str();
}

bool CommandLineParser::IsInGroups(const CommandLineOptionGroupPtr group)
{
    for (auto groupFound : groups)
    {
        if (groupFound == group)
            return true;
    }
    return false;
}

void CommandLineParser::SetupStandardGroups()
{
    CommandLineOptionGroupPtr mainGroup(new CommandLineOptionGroup(name, description));
    AddGroup(mainGroup);

    CommandLineOptionGroupPtr helpGroup(new CommandLineOptionGroup(HelpOptionsGroupName));
    AddGroup(helpGroup);
    helpGroup->AddOptionNoArgument("help", 'h', "Show help options", &showHelp);

    MainGroup(mainGroup);
}

int CommandLineParser::MatchShortOption(char name)
{
    for (size_t i = 0; i < optionList.size(); i++)
    {
        if (optionList[i]->ShortName() == name)
        {
            return (int)i;
        }
    }
    return -1;
}

int CommandLineParser::MatchLongOption(const char * name, size_t nameLength, bool allowShortOptions)
{
    for (size_t i = 0; i < optionList.size(); i++)
    {
        // find matching long option
        if (optionList[i]->LongName() == string(name, nameLength))
        {
            // exact match
            return static_cast<int>(i);
        }
        // If this is a known short option, don't allow a partial match of a single character.
        if (allowShortOptions && (nameLength == 1) && (*name == optionList[i]->ShortName()))
            return static_cast<int>(i);
    }
    return -1;
}
