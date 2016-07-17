#include "CommandLineOptionsParser.h"

#include <iostream>
#include "core/Path.h"
#include "core/Util.h"

static const std::string ApplicationName = "asm-8080";
static const std::string DefaultObjectExtension = ".dat";
static const std::string DefaultListingExtension = ".txt";

CommandLineOptionsParser::CommandLineOptionsParser() :
    inputFilePath(),
    outputObjectFilePath(),
    outputListingFilePath(),
    outputCrossReferenceFilePath()
{
    Core::CommandLineOptionGroupPtr group = std::make_shared<Core::CommandLineOptionGroup>("Main", "Global options");
    group->AddOptionRequiredArgument("input", 'i', "Input file (required)", &inputFilePath);
    group->AddOptionRequiredArgument("output", 'o', "Object output file (default = <input base path>" + DefaultObjectExtension + ")", &outputObjectFilePath);
    group->AddOptionRequiredArgument("listing", 'l', "Output listing file (default = <object output base path>" + DefaultListingExtension + ")", &outputListingFilePath);
    group->AddOptionRequiredArgument("xref", 'x', "Output cross-reference file (default = none)", &outputCrossReferenceFilePath);
    AddGroup(group);
}

void CommandLineOptionsParser::ResolveDefaults()
{
    if (inputFilePath.empty())
    {
        std::cerr << GetHelp(ApplicationName) << std::endl;
        throw std::runtime_error("No input file specified");
    }
    if (!Core::Path::FileExists(inputFilePath))
    {
        std::cerr << GetHelp(ApplicationName) << std::endl;
        throw std::runtime_error("Non-existing input file specified");
    }
    if (outputObjectFilePath.empty())
    {
        outputObjectFilePath = Core::Path::StripExtension(inputFilePath) + DefaultObjectExtension;
    }
    if (outputListingFilePath.empty())
    {
        outputListingFilePath = Core::Path::StripExtension(outputObjectFilePath) + DefaultListingExtension;
    }
}