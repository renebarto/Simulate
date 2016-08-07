#pragma once

#include "core/CommandLineParser.h"
#include "core/CommandLineOptionGroup.h"

class CommandLineOptionsParser : public Core::CommandLineParser
{
public:
    CommandLineOptionsParser();

    void Parse(int argc, wchar_t const * const * argv);

    std::string namespaceName;
    std::string frameFilesDirectory;
    std::string traceString;
    std::string outputDirectory;
    bool emitLineNumbers;
    std::string inputFile;

    void OnParseParameter(std::string const & parameter) override;

    void ShowHelp();
};

