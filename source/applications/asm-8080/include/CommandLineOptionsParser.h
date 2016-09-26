#pragma once

#include "core/CommandLineParser.h"
#include "core/CommandLineOptionGroup.h"

class CommandLineOptionsParser : public Core::CommandLineParser
{
public:
    CommandLineOptionsParser();

    std::string inputFilePath;
    std::string outputObjectFilePath;
    std::string outputReportingFilePath;
    bool listSymbols;
    bool listSymbolCrossReferences;
    bool emulate;

    void ResolveDefaults();
};


