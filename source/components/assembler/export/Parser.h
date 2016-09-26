#pragma once

#include "AbstractSyntaxTree.h"
#include "CPUType.h"
#include "MachineCode.h"
#include "Scanner.h"
#include "ErrorHandler.h"
#include "PrettyPrinter.h"

namespace Assembler
{

class ICPUParser;
class ICPUAssembler;

class Parser
{
public:
	Parser(Scanner & scanner, AssemblerMessages & messages, std::wostream & reportStream);
	~Parser();

	bool Parse();
    void PrintSymbols();
    void PrintSymbolCrossReference();
    void DumpAST();
    std::vector<uint8_t> const & GetMachineCode() { return machineCode; }

    size_t NumErrors() const { return errorHandler.NumErrors(); }
    size_t NumWarnings() const { return errorHandler.NumWarnings(); }
    size_t NumExceptions() const { return errorHandler.NumExceptions(); }

    ASTree const & GetAST() const;

private:

	Scanner & scanner;
	ErrorHandler errorHandler;
	PrettyPrinter<wchar_t> printer;
    Token currentToken;
    Token lastToken;

    KeywordMap<CPUType> knownCPU;
    CPUType cpuType;
    MachineCode machineCode;
    std::shared_ptr<ICPUParser> cpuAssemblerParser;
    std::unique_ptr<ICPUAssembler> cpuAssembler;
    bool showSymbols;
    bool showCrossReference;

    void Init();
    void PrintErrors();

	void SyntaxError(TokenType tokenType);
	void SemanticError(std::wstring const & msg);
	void Get();
	void Expect(TokenType tokenType);
    void ParseAssembler();
    std::shared_ptr<ICPUParser> CreateAssemblerParser();
    std::unique_ptr<ICPUAssembler> CreateAssembler(std::shared_ptr<ICPUParser> parser);
}; // Parser

} // namespace Assembler
