#include "unit-test-c++/UnitTestC++.h"
#include <fstream>
#include "osal/console.h"
#include "core/ConsoleLogger.h"
#include "core/DefaultLogger.h"
#include "unit-test-c++/ConsoleTestReporter.h"
#include "unit-test-c++/XMLTestReporter.h"
#include "CommandLineOptionsParser.h"

using namespace std;
using namespace UnitTestCpp;
using namespace Core;

int main(int argc, char* argv[])
{
    OSAL::Console console;
    const std::string moduleName = "core`";
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
    catch (const Exception & e)
    {
        console << fgcolor(OSAL::ConsoleColor::Red) << "Exception: " << e.what() << endl;
        console << fgcolor(OSAL::ConsoleColor::Yellow) << parser.GetHelp(applicationName, true) << endl;
        console << fgcolor(OSAL::ConsoleColor::Default);
        exit(1);
    }

    console << "Application: " << applicationName << endl;
    console << "XML output : " << parser.xmlOutput << endl;
    console << "Suite      : " << parser.testSuiteName << endl;
    console << "Fixture    : " << parser.testFixtureName << endl;
    console << "Test       : " << parser.testName << endl;

    int result = 0;

    if (!parser.xmlOutput.empty()) {
        ofstream outputFile;

        outputFile.open(parser.xmlOutput);
        XMLTestReporter reporter(outputFile);
        const char * suiteName = parser.testSuiteName.empty() ? 0 : parser.testSuiteName.c_str();
        const char * fixtureName = parser.testFixtureName.empty() ? 0 : parser.testFixtureName.c_str();
        const char * testName = parser.testName.empty() ? 0 : parser.testName.c_str();
        if ((suiteName != 0) || (fixtureName != 0) || (testName != 0)) {
            return RunSelectedTests(reporter, InSelection(suiteName, fixtureName, testName));
        }
        result = RunAllTests(reporter);
    } else {
        ConsoleTestReporter reporter;
        const char * suiteName = parser.testSuiteName.empty() ? 0 : parser.testSuiteName.c_str();
        const char * fixtureName = parser.testFixtureName.empty() ? 0 : parser.testFixtureName.c_str();
        const char * testName = parser.testName.empty() ? 0 : parser.testName.c_str();
        if ((suiteName != 0) || (fixtureName != 0) || (testName != 0)) {
            return RunSelectedTests(reporter, InSelection(suiteName, fixtureName, testName));
        }
        result = RunAllTests(reporter);
    }
    return result;
}

