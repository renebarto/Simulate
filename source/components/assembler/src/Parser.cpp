#include "assembler/Parser.h"

#include "assembler/CPUParserIntel8080_8085.h"
#include "assembler/CPUAssemblerIntel8080_8085.h"

namespace Assembler
{

Parser::Parser(std::string const & moduleName, Scanner & scanner, AssemblerMessages & messages, std::wostream & reportStream)
    : scanner(scanner)
    , errorHandler(messages)
    , printer(reportStream)
    , currentToken()
    , lastToken()
    , knownCPU()
    , cpuType(CPUType::Undefined)
    , objectCode(moduleName)
    , cpuAssemblerParser()
{
    Init();
}

Parser::~Parser()
{
}

bool Parser::Parse()
{
    currentToken = Token();
    lastToken = Token();
    Get();
    ParseAssembler();
    Expect(TokenType::EndOfFile);
    objectCode.Clear();
    if (cpuAssemblerParser == nullptr)
    {
        PrintErrors();
        return false;
    }
    if ((errorHandler.NumErrors() != 0) || (errorHandler.NumExceptions() != 0))
    {
        cpuAssemblerParser->PrintWithErrors();
        return false;
    }
    std::unique_ptr<ICPUAssembler> cpuAssembler(CreateAssembler(cpuAssemblerParser));
    if (!cpuAssembler->Generate(objectCode))
        return false;
    return true;
}

void Parser::PrintErrors()
{
    for (auto message : errorHandler)
    {
        printer << L"Error: " << message.Loc() << L" - " << message.Message() << std::endl;
    }
}

void Parser::PrintSymbols()
{
    cpuAssemblerParser->PrintSymbolTable();
}

void Parser::PrintSymbolCrossReference()
{
    cpuAssemblerParser->PrintSymbolCrossReference();
}

void Parser::DumpAST()
{
    cpuAssemblerParser->DumpAST(std::wcout, 1);
}

void Parser::Init()
{
    knownCPU.Set(L"Intel4004", CPUType::Intel4004);
    knownCPU.Set(L"Intel8008", CPUType::Intel8008);
    knownCPU.Set(L"Intel8080", CPUType::Intel8080);
    knownCPU.Set(L"Intel8085", CPUType::Intel8085);
}

void Parser::SyntaxError(TokenType tokenType)
{
    errorHandler.SyntaxError(currentToken.location, tokenType);
}

void Parser::SemanticError(std::wstring const & msg)
{
    errorHandler.Error(currentToken.location, msg);
}

void Parser::Get()
{
    for (;;)
    {
        lastToken = currentToken;
        currentToken = scanner.Scan();
        if (currentToken.kind != TokenType::Unknown)
        {
            break;
        }
    }
}

void Parser::Expect(TokenType tokenType)
{
    if (currentToken.kind == tokenType) 
        Get(); 
    else 
        SyntaxError(tokenType);
}

void Parser::ParseAssembler()
{
    Expect(TokenType::CPUDirective);
    if (currentToken.kind == TokenType::Identifier)
    {
        cpuType = knownCPU.Get(currentToken.value, CPUType::Undefined);
        if (cpuType == CPUType::Undefined)
        {
            std::wostringstream stream;
            stream << L"Unknown CPU type: " << currentToken.value;
            SemanticError(stream.str());
        }
        else
        {
            Get();
            scanner.Pushback(currentToken);
            cpuAssemblerParser = CreateAssemblerParser();
            cpuAssemblerParser->Parse();
        }
    }
    else if (currentToken.kind == TokenType::Number)
    {
        std::wostringstream stream;
        stream << L"Unknown CPU type: " << currentToken.value;
        SemanticError(stream.str());
    }
    else
        SyntaxError(TokenType::Identifier);
    Get();
    while (currentToken.kind != TokenType::EndOfFile)
    {
        Get();
    }
}

ASTree const & Parser::GetAST() const
{
    if (cpuAssemblerParser == nullptr)
        throw AssemblerException("No CPU parser instantiated");
    return cpuAssemblerParser->GetAST();
}

std::shared_ptr<ICPUParser> Parser::CreateAssemblerParser()
{
    switch (cpuType)
    {
    case CPUType::Intel8080:
    case CPUType::Intel8085:
        return std::shared_ptr<ICPUParser>(new CPUParserIntel8080_8085(cpuType, scanner, errorHandler, printer));
    default:
        {
            std::wostringstream stream;
            stream << L"Unsupported CPU type: " << cpuType;
            SemanticError(stream.str());
            return std::shared_ptr<ICPUParser>();
        }
    }
}

std::unique_ptr<ICPUAssembler> Parser::CreateAssembler(std::shared_ptr<ICPUParser> parser)
{
    switch (cpuType)
    {
    case CPUType::Intel8080:
    case CPUType::Intel8085:
        return std::unique_ptr<ICPUAssembler>(new CPUAssemblerIntel8080_8085(parser, errorHandler, printer));
    default:
        {
            std::wostringstream stream;
            stream << L"Unsupported CPU type: " << cpuType;
            SemanticError(stream.str());
            return std::unique_ptr<ICPUAssembler>();
        }
    }
}

} // namespace Assembler

