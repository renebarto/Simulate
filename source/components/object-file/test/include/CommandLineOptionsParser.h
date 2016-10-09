#pragma once

#include "core/CommandLineParser.h"
#include "core/CommandLineOptionGroup.h"

class CommandLineOptionsParser : public Core::CommandLineParser
{
public:
    CommandLineOptionsParser();

    std::string testSuiteName;
    std::string testFixtureName;
    std::string testName;
    std::string xmlOutput;
};


