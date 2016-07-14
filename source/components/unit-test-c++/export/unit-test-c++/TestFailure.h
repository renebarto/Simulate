#pragma once
#include <string>
#include <iostream>

namespace UnitTestCpp
{

class TestDetails;

class TestFailure
{
public:
    TestFailure() = delete;
    TestFailure(const TestDetails & details, const std::string & message);
    TestFailure(const TestFailure &) = delete;

    TestFailure & operator = (const TestFailure &) = delete;

    const std::string suiteName;
    const std::string fixtureName;
    const std::string testName;
    const std::string fileName;
    const int lineNumber;
    const std::string message;
};

inline std::ostream & operator << (std::ostream & stream, const TestFailure & testFailure)
{
    return stream << testFailure.fileName << ":" << testFailure.lineNumber << " : Failure in "
                  << testFailure.suiteName << "::" << testFailure.fixtureName << "::" 
                  << testFailure.testName << ": "
                  << testFailure.message;
}


} // namespace UnitTestCpp
