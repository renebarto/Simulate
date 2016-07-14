#pragma once

#include <exception>
#include <string>

namespace UnitTestCpp
{

class TestResults;
class TestDetails;

namespace CurrentTest
{
    TestResults * & Results();
    const TestDetails * & Details();

    void UnhandledException(std::string const & context, int line, std::exception const & e);
    void UnhandledException(std::string const & context, int line);
}

} // namespace UnitTestCpp
