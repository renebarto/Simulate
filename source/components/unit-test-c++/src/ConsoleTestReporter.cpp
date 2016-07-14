#include "unit-test-c++/ConsoleTestReporter.h"

#include <cstdio>
#include <iomanip>
#include <osal.h>
#include <osal/flagoperators.h>
#include "unit-test-c++/Settings.h"
#include "unit-test-c++/TestDetails.h"
#include "unit-test-c++/TestResults.h"

using namespace std;

namespace UnitTestCpp
{

void ConsoleTestReporter::ReportTestRunStart(int numberOfTestSuites,
                                             int numberOfTestFixtures,
                                             int numberOfTests)
{
    if (console.ShouldUseColor())
		console.SetTerminalColor(OSAL::ConsoleColor::Green);

    stream << TestRunSeparator;

    if (console.ShouldUseColor())
		console.ResetTerminalColor();

    stream << " " << TestRunStartMessage(numberOfTestSuites, numberOfTestFixtures,
                                         numberOfTests) << endl;
}

void ConsoleTestReporter::ReportTestRunFinish(int numberOfTestSuites,
                                              int numberOfTestFixtures,
                                              int numberOfTests,
                                              int milliSecondsElapsed)
{
    if (console.ShouldUseColor())
		console.SetTerminalColor(OSAL::ConsoleColor::Green);

    stream << TestRunSeparator;

    if (console.ShouldUseColor())
		console.ResetTerminalColor();

    stream << " " << TestRunFinishMessage(numberOfTestSuites, numberOfTestFixtures, numberOfTests,
                                          milliSecondsElapsed) << endl;
}

void ConsoleTestReporter::ReportTestRunSummary(const TestResults * results, int milliSecondsElapsed)
{
    if (console.ShouldUseColor())
    {
        if (results->GetFailureCount() > 0)
            console.SetTerminalColor(OSAL::ConsoleColor::Red);
        else
			console.SetTerminalColor(OSAL::ConsoleColor::Green);
    }

    stream << TestRunSummaryMessage(results, milliSecondsElapsed) << endl;

    if (console.ShouldUseColor())
		console.ResetTerminalColor();
}

void ConsoleTestReporter::ReportTestRunOverview(const TestResults * results)
{
    stream << TestRunOverviewMessage(results) << endl;
}

void ConsoleTestReporter::ReportTestSuiteStart(const string & suiteName, int numberOfTestFixtures)
{
    if (console.ShouldUseColor())
		console.SetTerminalColor(OSAL::ConsoleColor::Cyan);

    stream << TestSuiteSeparator;

    if (console.ShouldUseColor())
		console.ResetTerminalColor();

    stream << " " << TestSuiteStartMessage(suiteName, numberOfTestFixtures) << endl;
}

void ConsoleTestReporter::ReportTestSuiteFinish(const string & suiteName,
                                                int numberOfTestFixtures,
                                                int milliSecondsElapsed)
{
    if (console.ShouldUseColor())
		console.SetTerminalColor(OSAL::ConsoleColor::Cyan);

    stream << TestSuiteSeparator;

    if (console.ShouldUseColor())
		console.ResetTerminalColor();

    stream << " " << TestSuiteFinishMessage(suiteName, numberOfTestFixtures,
                                            milliSecondsElapsed) << endl;
}

void ConsoleTestReporter::ReportTestFixtureStart(const string & fixtureName, int numberOfTests)
{
    if (console.ShouldUseColor())
		console.SetTerminalColor(OSAL::ConsoleColor::Yellow);

    stream << TestFixtureSeparator;

    if (console.ShouldUseColor())
		console.ResetTerminalColor();

    stream << " " << TestFixtureStartMessage(fixtureName, numberOfTests) << endl;
}

void ConsoleTestReporter::ReportTestFixtureFinish(const string & fixtureName,
                                                  int numberOfTests,
                                                  int milliSecondsElapsed)
{
    if (console.ShouldUseColor())
		console.SetTerminalColor(OSAL::ConsoleColor::Yellow);

    stream << TestFixtureSeparator;

    if (console.ShouldUseColor())
		console.ResetTerminalColor();

    stream << " " << TestFixtureFinishMessage(fixtureName, numberOfTests, milliSecondsElapsed);
}

void ConsoleTestReporter::ReportTestStart(const TestDetails & details)
{
    DeferredTestReporter::ReportTestStart(details);
}

void ConsoleTestReporter::ReportTestFinish(const TestDetails & details, bool success,
                                           int milliSecondsElapsed)
{
    DeferredTestReporter::ReportTestFinish(details, success, milliSecondsElapsed);
    if (console.ShouldUseColor())
    {
		console.SetTerminalColor(success ? OSAL::ConsoleColor::Green : OSAL::ConsoleColor::Red);
    }

    if (success)
        stream << TestSuccessSeparator;
    else
        stream << TestFailSeparator;

    if (console.ShouldUseColor())
		console.ResetTerminalColor();

    stream << " " << TestFinishMessage(details, success, milliSecondsElapsed) << endl;
}

void ConsoleTestReporter::ReportTestFailure(TestDetails const& details, const string & failure)
{
    DeferredTestReporter::ReportTestFailure(details, failure);
}

} // namespace UnitTestCpp
