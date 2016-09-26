#include "CommandLineOptionsParser.h"

#include <iostream>
#include "core/Path.h"
#include "core/Util.h"

static const std::string ApplicationName = "asm-8080";
static const std::string DefaultObjectExtension = ".dat";
static const std::string DefaultReportExtension = ".txt";

CommandLineOptionsParser::CommandLineOptionsParser()
    : inputFilePath()
    , outputObjectFilePath()
    , outputReportingFilePath()
    , listSymbols()
    , listSymbolCrossReferences()
    , emulate()
{
    Core::CommandLineOptionGroupPtr group = std::make_shared<Core::CommandLineOptionGroup>("Main", "Global options");
    group->AddOptionRequiredArgument("input", 'i', "Input file (required)", &inputFilePath);
    group->AddOptionRequiredArgument("output", 'o', "Object output file (default = <input base path>" + DefaultObjectExtension + ")", &outputObjectFilePath);
    group->AddOptionRequiredArgument("report", 'r', "Output reporting file (default = <input base path>-lst" + DefaultReportExtension + ")", &outputReportingFilePath);
    group->AddOptionNoArgument("symbols", 's', "Output symbols list to reporting file", &listSymbols);
    group->AddOptionNoArgument("xref", 'x', "Output symbols cross reference to reporting file", &listSymbolCrossReferences);
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
    if (outputReportingFilePath.empty())
    {
        outputReportingFilePath = Core::Path::StripExtension(outputObjectFilePath) + "-lst" + DefaultReportExtension;
    }
}