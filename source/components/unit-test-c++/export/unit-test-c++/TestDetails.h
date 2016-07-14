#pragma once

#include <string>

namespace UnitTestCpp
{

class TestDetails
{
public:
    TestDetails(std::string const & testName, 
                std::string const & fixtureName, 
                std::string const & suiteName,
                std::string const & fileName, int lineNumber);
    TestDetails(TestDetails const & details, int lineNumber);

    const std::string suiteName;
    const std::string fixtureName;
    const std::string testName;
    const std::string fileName;
    const int lineNumber;
};

} // namespace UnitTestCpp
