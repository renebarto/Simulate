#pragma once

#include <iostream>
#include "assembler/AssemblerMessage.h"
#include "assembler/Location.h"
#include "assembler/Token.h"

namespace Assembler
{

class ErrorHandler
{
public:
	ErrorHandler(AssemblerMessages & messages);
	void SyntaxError(Location const & location, TokenType tokenType);
	void Error(Location const & location, std::wstring const & s);
	void Warning(Location const & location, std::wstring const & s);
	void Warning(std::wstring const & s);
	void Exception(std::wstring const & s);

    size_t NumExceptions() const { return numExceptions; }
    size_t NumErrors() const { return numErrors; }
    size_t NumWarnings() const { return numWarnings; }
    AssemblerMessages::const_iterator begin() const { return messages.begin(); }
    AssemblerMessages::const_iterator end() const { return messages.end(); }

private:
    AssemblerMessages & messages;
    size_t numExceptions;
    size_t numErrors;
    size_t numWarnings;
}; // ErrorHandler

} // namespace Assembler
