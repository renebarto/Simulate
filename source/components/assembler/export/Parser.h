#pragma once

#include "AbstractSyntaxTree.h"
#include "CPUType.h"
#include "Scanner.h"
#include "ErrorHandler.h"

namespace Assembler
{

class ICPUParser;

class Parser
{
public:
	Parser(Scanner & scanner, AssemblerMessages & messages, std::wostream & errorStream);
	~Parser();

	void Parse();

    size_t NumErrors() const { return errorHandler.NumErrors(); }
    size_t NumWarnings() const { return errorHandler.NumWarnings(); }
    size_t NumExceptions() const { return errorHandler.NumExceptions(); }

    ASTree const & GetAST() const;

private:

	Scanner & scanner;
	ErrorHandler errorHandler;
	std::wostream & reportStream;
    Token currentToken;
    Token lastToken;

    KeywordMap<CPUType> knownCPU;
    CPUType cpuType;
    std::unique_ptr<ICPUParser> cpuAssemblerParser;

    void Init();

	void SyntaxError(TokenType tokenType);
	void SemanticError(std::wstring const & msg);
	void Get();
	void Expect(TokenType tokenType);
    void ParseAssembler();
    std::unique_ptr<ICPUParser> CreateAssemblerParser();
}; // Parser

} // namespace Assembler
