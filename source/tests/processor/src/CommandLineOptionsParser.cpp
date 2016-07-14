#include "CommandLineOptionsParser.h"
#include "core/Util.h"

CommandLineOptionsParser::CommandLineOptionsParser() :
    testSuiteName(),
    testFixtureName(),
    testName(),
    xmlOutput()
{
    Core::CommandLineOptionGroupPtr group =
        std::make_shared<Core::CommandLineOptionGroup>("Test runner", "Runs tests");
    group->AddOptionRequiredArgument("runsuite", 's',
                                     "Run only specified test suite (default = all)",
                                     &testSuiteName);
    group->AddOptionRequiredArgument("runfixture", 'f',
                                     "Run only specified test fixture (default = all)",
                                     &testFixtureName);
    group->AddOptionRequiredArgument("runtest", 't',
                                     "Run only specified test (default = all)", &testName);
    group->AddOptionRequiredArgument("xmloutput", 0, "Send output to specified XML file",
                                     &xmlOutput);
    AddGroup(group);
}

