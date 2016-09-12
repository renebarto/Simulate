#include "Parser.h"

#include "CPUParserIntel8080_8085.h"

namespace Assembler
{

Parser::Parser(Scanner & scanner, AssemblerMessages & messages, std::wostream & errorStream)
    : scanner(scanner)
    , errorHandler(errorStream, messages)
    , reportStream(scanner.GetReportStream())
    , currentToken()
    , lastToken()
    , knownCPU()
    , cpuType(CPUType::Undefined)
{
    Init();
}

Parser::~Parser()
{
}

void Parser::Parse()
{
    currentToken = Token();
    lastToken = Token();
    Get();
    ParseAssembler();
    Expect(TokenType::EndOfFile);
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
            errorHandler.NoError();
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
            Expect(TokenType::EOL);
            scanner.Pushback(currentToken);
            cpuAssemblerParser = CreateAssemblerParser();
            cpuAssemblerParser->Parse();
        }
    }
    else
        SyntaxError(TokenType::Identifier);
    Get();
    while (currentToken.kind == TokenType::EOL)
    {
        Get();
    }
}

ASTree const & Parser::GetAST() const
{
    if (cpuAssemblerParser == nullptr)
        throw std::runtime_error("No CPU parser instantiated");
    return cpuAssemblerParser->GetAST();
}

std::unique_ptr<ICPUParser> Parser::CreateAssemblerParser()
{
    switch (cpuType)
    {
    case CPUType::Intel8080:
    case CPUType::Intel8085:
        return std::unique_ptr<ICPUParser>(new CPUParserIntel8080_8085(cpuType, scanner, errorHandler));
    default:
        {
            std::wostringstream stream;
            stream << L"Unsupported CPU type: " << cpuType;
            SemanticError(stream.str());
            return std::unique_ptr<ICPUParser>();
        }
    }
}

} // namespace Assembler

