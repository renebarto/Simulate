#include <unit-test-c++/UnitTestC++.h>

#include <fstream>
#include <string>
#include "osal/console.h"
#include "core/ConsoleLogger.h"
#include "core/DefaultLogger.h"
#include "CommandLineOptionsParser.h"

int main(int argc, char* argv[])
{
    OSAL::Console console;

    const std::string moduleName = "osal";
    console << fgcolor(OSAL::ConsoleColor::Magenta | OSAL::ConsoleColor::Intensity);
    console << "Running tests for: " << moduleName << std::endl;
    console << fgcolor(OSAL::ConsoleColor::Default);
    Core::ConsoleLogger logger(Core::TheLogger(), console);

    CommandLineOptionsParser parser;
    string applicationName = argv[0];

    try
    {
        parser.Parse(argc, argv);
    }
    catch (const Core::Exception & e)
    {
        console << fgcolor(OSAL::ConsoleColor::Red) << "Exception: " << e.what() << std::endl;
        console << fgcolor(OSAL::ConsoleColor::Yellow) << parser.GetHelp(applicationName, true)
                << std::endl;
        console << fgcolor(OSAL::ConsoleColor::Default);
        exit(1);
    }

    console << "Application: " << applicationName << std::endl;
    console << "XML output : " << parser.xmlOutput << std::endl;
    console << "Suite      : " << parser.testSuiteName << std::endl;
    console << "Fixture    : " << parser.testFixtureName << std::endl;
    console << "Test       : " << parser.testName << std::endl;

    int result = 0;

    if (!parser.xmlOutput.empty())
    {
        std::ofstream outputFile;

        outputFile.open(parser.xmlOutput);
        UnitTestCpp::XMLTestReporter reporter(outputFile);
        const char * suiteName = parser.testSuiteName.empty() ? 0 : parser.testSuiteName.c_str();
        const char * fixtureName = parser.testFixtureName.empty() ? 0 : parser.testFixtureName.c_str();
        const char * testName = parser.testName.empty() ? 0 : parser.testName.c_str();
        if ((suiteName != 0) || (fixtureName != 0) || (testName != 0))
        {
            return RunSelectedTests(reporter, UnitTestCpp::InSelection(suiteName, fixtureName, testName));
        }
        result = RunAllTests(reporter);
    }
    else
    {
        UnitTestCpp::ConsoleTestReporter reporter;
        const char * suiteName = parser.testSuiteName.empty() ? 0 : parser.testSuiteName.c_str();
        const char * fixtureName = parser.testFixtureName.empty() ? 0 : parser.testFixtureName.c_str();
        const char * testName = parser.testName.empty() ? 0 : parser.testName.c_str();
        if ((suiteName != 0) || (fixtureName != 0) || (testName != 0))
        {
            return RunSelectedTests(reporter, UnitTestCpp::InSelection(suiteName, fixtureName, testName));
        }
        result = RunAllTests(reporter);
    }
    return result;
}
