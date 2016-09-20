#pragma once

#include "AbstractSyntaxTree.h"
#include "CPUType.h"
#include "MachineCode.h"
#include "Scanner.h"
#include "ErrorHandler.h"

namespace Assembler
{

class ICPUParser;
class ICPUAssembler;

class Parser
{
public:
	Parser(Scanner & scanner, AssemblerMessages & messages, std::wostream & errorStream, std::wostream & reportStream);
	~Parser();

	void Parse();
    std::vector<uint8_t> const & GetMachineCode() { return machineCode; }

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
    MachineCode machineCode;
    std::unique_ptr<ICPUParser> cpuAssemblerParser;
    std::unique_ptr<ICPUAssembler> cpuAssembler;

    void Init();
    void PrintErrors();

	void SyntaxError(TokenType tokenType);
	void SemanticError(std::wstring const & msg);
	void Get();
	void Expect(TokenType tokenType);
    void ParseAssembler();
    std::unique_ptr<ICPUParser> CreateAssemblerParser();
    std::unique_ptr<ICPUAssembler> CreateAssembler();
}; // Parser

} // namespace Assembler
