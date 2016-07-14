#pragma once

#include <exception>
#include <string>

namespace UnitTestCpp
{

class AssertException : public std::exception
{
public:
    AssertException(std::string const & description, std::string const & fileName, int lineNumber);
    virtual ~AssertException();

    virtual char const * what() const noexcept { return description.c_str(); }

    std::string const & Filename() const { return fileName; }
    int LineNumber() const { return lineNumber; }

private:
    std::string description;
    std::string fileName;
    int lineNumber;
};

} // namespace UnitTestCpp
