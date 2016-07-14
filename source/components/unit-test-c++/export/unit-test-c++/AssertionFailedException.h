#pragma once

#include <exception>
#include <string>

namespace UnitTestCpp
{

class AssertionFailedException : public std::exception
{
public:
    AssertionFailedException(std::string const & fileName, int lineNumber);
    virtual ~AssertionFailedException();

    virtual char const * what() const noexcept { return "Assertion failed"; }

    std::string const & Filename() const { return fileName; }
    int LineNumber() const { return lineNumber; }

private:
    std::string fileName;
    int lineNumber;
};

} // namespace UnitTestCpp
