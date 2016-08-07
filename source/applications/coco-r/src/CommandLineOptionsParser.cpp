#include "CommandLineOptionsParser.h"
#include "osal/Console.h"
#include "core/String.h"
#include "core/Util.h"

CommandLineOptionsParser::CommandLineOptionsParser()
    : namespaceName()
    , frameFilesDirectory()
    , traceString()
    , outputDirectory()
    , emitLineNumbers()
    , inputFile()
{
    Core::CommandLineOptionGroupPtr group =
        std::make_shared<Core::CommandLineOptionGroup>("Test runner", "Runs tests");
    group->AddOptionRequiredArgument("namespace", 'n',
                                     "<namespaceName>",
                                     &namespaceName);
    group->AddOptionRequiredArgument("frames", 'f',
                                     "<frameFilesDirectory>",
                                     &frameFilesDirectory);
    group->AddOptionRequiredArgument("trace", 't',
                                     "<traceString>", &traceString);
    group->AddOptionRequiredArgument("output", 'o', "<outputDirectory>",
                                     &outputDirectory);
    group->AddOptionNoArgument("lines", 'l', "Emit line numbers",
                               &emitLineNumbers);
    AddGroup(group);
}

void CommandLineOptionsParser::OnParseParameter(std::string const & parameter)
{
    inputFile = parameter;
}

void CommandLineOptionsParser::Parse(int argc, wchar_t const * const * argv)
{
#if defined(_MSC_VER)
    char * * arguments = new char *[argc];
    for (int i = 1; i < argc; ++i)
    {
        std::wstring argumentW(argv[i]);
        std::string argument(Core::String::ToString(argumentW));
        char * argumentPtr = new char[argument.length() + 1];
        std::copy(argument.begin(), argument.end(), argumentPtr);
        argumentPtr[argument.length()] = '\0';
        arguments[i - 1] = argumentPtr;
    }
    arguments[argc - 1] = nullptr;
    Core::CommandLineParser::Parse(argc, arguments);
    for (int i = 0; i < argc - 1; ++i)
    {
        delete [] arguments[i];
    }    
    delete arguments;
#else
    char * * arguments = new char *[argc + 1];
    for (int i = 0; i < argc; ++i)
    {
        std::wstring argumentW(argv[i]);
        std::string argument(Core::String::ToString(argumentW));
        char * argumentPtr = new char[argument.length() + 1];
        std::copy(argument.begin(), argument.end(), argumentPtr);
        argumentPtr[argument.length()] = '\0';
        arguments[i] = argumentPtr;
    }
    arguments[argc] = nullptr;
    Core::CommandLineParser::Parse(argc, arguments);
    for (int i = 0; i < argc; ++i)
    {
        delete [] arguments[i];
    }    
    delete arguments;
#endif
}

void CommandLineOptionsParser::ShowHelp()
{
    OSAL::Console console;

    console << fgcolor(OSAL::ConsoleColor::Yellow);
    console << "Usage: Coco Grammar.ATG {Option}" << std::endl;
    console << "Options:" << std::endl;
    console << "  -namespace <namespaceName>" << std::endl;
    console << "  -frames    <frameFilesDirectory>" << std::endl;
    console << "  -trace     <traceString>" << std::endl;
    console << "  -o         <outputDirectory>" << std::endl;
    console << "  -lines" << std::endl;
    console << "Valid characters in the trace string:" << std::endl;
    console << "  A  trace automaton" << std::endl;
    console << "  F  list first/follow sets" << std::endl;
    console << "  G  print syntax graph" << std::endl;
    console << "  I  trace computation of first sets" << std::endl;
    console << "  J  list ANY and SYNC sets" << std::endl;
    console << "  P  print statistics" << std::endl;
    console << "  S  list symbol table" << std::endl;
    console << "  X  list cross reference table" << std::endl;
    console << "Scanner.frame and Parser.frame files needed in ATG directory" << std::endl;
    console << "or in a directory specified in the -frames option." << std::endl;
    console << fgcolor(OSAL::ConsoleColor::Default);
}
