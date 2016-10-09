#pragma once

#include "assembler/AbstractSyntaxTree.h"
#include "assembler/CPUType.h"
#include "object-file/ObjectCode.h"
#include "assembler/Scanner.h"
#include "assembler/ErrorHandler.h"
#include "assembler/PrettyPrinter.h"

namespace Assembler
{

class ICPUParser;
class ICPUAssembler;

class Parser
{
public:
	Parser(std::string const & moduleName, Scanner & scanner, AssemblerMessages & messages, std::wostream & reportStream);
	~Parser();

    bool Parse();
    void PrintSymbols();
    void PrintSymbolCrossReference();
    void DumpAST();
    ObjectFile::ObjectCode const & GetObjectCode() const { return objectCode; }
    CPUType GetCPUType() const { return cpuType; }

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
    ObjectFile::ObjectCode objectCode;
    std::shared_ptr<ICPUParser> cpuAssemblerParser;

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

