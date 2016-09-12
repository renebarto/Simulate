#pragma once

#include "core/Util.h"
#include "CPUType.h"
#include "ICPUParser.h"
#include "Scanner.h"
#include "ErrorHandler.h"
#include "KeywordMap.h"
#include "Nodes.h"
#include "SymbolMap.h"
#include "AbstractSyntaxTree.h"

namespace Assembler
{

struct Label
{
    std::wstring name;
    size_t location;
    Label(std::wstring const & name)
        : name(name)
        , location()
    {
    }
    Label(std::wstring const & name, size_t location)
        : name(name)
        , location(location)
    {
    }
};

template<class OpcodeType, class OperandState>
class CPUParser : public ICPUParser
{
public:
	CPUParser(CPUType cpuType, Scanner & scanner, ErrorHandler & errorHandler);
	virtual ~CPUParser();

	void Parse() override;

    ASTree const & GetAST() const { return ast; }

protected:
    CPUType cpuType;
    Scanner & scanner;
	ErrorHandler & errorHandler;
	std::wostream & reportStream;
    Token currentToken;
    Token lastToken;

    SymbolMap<Label> labels;
    void AddLabel(Label const & label);
    void UpdateLabel(Label const & label);
    Label const & GetLabel(std::wstring const & name) const;
    bool HaveLabel(std::wstring const & name) const;

    KeywordMap<OpcodeType> opcodes;
	StartStates<OpcodeType, OperandState> startStates;

    ASTree ast;

	void SyntaxError(TokenType tokenType);
    void SemanticError(Location const & location, std::wstring const & msg);
	void SemanticError(std::wstring const & msg);
	void Get();
	void Expect(TokenType tokenType);
    int64_t ConvertToValue(std::wstring const & text);
    void ParseAssembler();
    bool CheckForAssemblerDirectives();

    virtual void HandleOpcodeAndOperands(LabelNode::Ptr label) = 0;
    virtual void HandleOperands(typename OpcodeNode<OpcodeType>::Ptr opcode, OperandState state) = 0;
}; // CPUParser

template<class OpcodeType, class OperandState>
CPUParser<OpcodeType, OperandState>::CPUParser(CPUType cpuType, Scanner & scanner, ErrorHandler & errorHandler)
    : cpuType(cpuType)
    , scanner(scanner)
    , errorHandler(errorHandler)
    , reportStream(scanner.GetReportStream())
    , currentToken()
    , lastToken()
    , labels()
    , opcodes()
    , startStates()
{
}

template<class OpcodeType, class OperandState>
CPUParser<OpcodeType, OperandState>::~CPUParser()
{
}

template<class OpcodeType, class OperandState>
void CPUParser<OpcodeType, OperandState>::Parse()
{
    currentToken = Token();
    lastToken = Token();
    Get();
    ParseAssembler();
}

template<class OpcodeType, class OperandState>
void CPUParser<OpcodeType, OperandState>::AddLabel(Label const & label)
{
    labels.Add(label.name, label);
}

template<class OpcodeType, class OperandState>
Label const & CPUParser<OpcodeType, OperandState>::GetLabel(std::wstring const & name) const
{
    return labels.Lookup(name);
}

template<class OpcodeType, class OperandState>
bool CPUParser<OpcodeType, OperandState>::HaveLabel(std::wstring const & name) const
{
    return labels.Exists(name);
}

template<class OpcodeType, class OperandState>
void CPUParser<OpcodeType, OperandState>::SyntaxError(TokenType tokenType)
{
    errorHandler.SyntaxError(currentToken.location, tokenType);
}

template<class OpcodeType, class OperandState>
void CPUParser<OpcodeType, OperandState>::SemanticError(Location const & location, std::wstring const & msg)
{
    errorHandler.Error(location, msg);
}

template<class OpcodeType, class OperandState>
void CPUParser<OpcodeType, OperandState>::SemanticError(std::wstring const & msg)
{
    SemanticError(currentToken.location, msg);
}

template<class OpcodeType, class OperandState>
void CPUParser<OpcodeType, OperandState>::Get()
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

template<class OpcodeType, class OperandState>
void CPUParser<OpcodeType, OperandState>::Expect(TokenType tokenType)
{
    if (currentToken.kind == tokenType) 
        Get(); 
    else 
        SyntaxError(tokenType);
}

template<class OpcodeType, class OperandState>
int64_t CPUParser<OpcodeType, OperandState>::ConvertToValue(std::wstring const & text)
{
    int64_t value;
    if (text.empty())
        return 0;
    switch (text[0])
    {
    case '%':
        if (!Core::Util::TryParse(text.substr(1), value, 2))
            return 0;
        break;
    case '@':
        if (!Core::Util::TryParse(text.substr(1), value, 8))
            return 0;
        break;
    case '$':
        if (!Core::Util::TryParse(text.substr(1), value, 16))
            return 0;
        break;
    default:
        {
            wchar_t lastChar = towupper(text[text.length() - 1]);
            if (lastChar == L'H')
            {
                if (!Core::Util::TryParse(text.substr(0, text.length() - 1), value, 16))
                    return 0;
            }
            else if ((lastChar == L'O') || (lastChar == L'Q'))
            {
                if (!Core::Util::TryParse(text.substr(0, text.length() - 1), value, 8))
                    return 0;
            }
            else if (lastChar == L'B')
            {
                if (!Core::Util::TryParse(text.substr(0, text.length() - 1), value, 2))
                    return 0;
            }
            else
                if (!Core::Util::TryParse(text, value))
                    return 0;
        }
    }
    return value;
}

template<class OpcodeType, class OperandState>
void CPUParser<OpcodeType, OperandState>::ParseAssembler()
{
    bool done = false;
    LabelNode::Ptr label;
    while (!done)
    {
        switch (currentToken.kind)
        {
        case TokenType::EndOfFile:
            Expect(TokenType::ENDCommand);
            done = true;
            break;
        case TokenType::EOL:
            Get();
            break;
        case TokenType::ENDCommand:
            done = true;
            break;
        case TokenType::Identifier:
            {
                Get();
                if (currentToken.kind == TokenType::Colon)
                {
                    label = Nodes::CreateLabel(lastToken.value, lastToken.location);
                    AddLabel(Label(lastToken.value));
                    Get();
                }
                else
                {
                    if (!CheckForAssemblerDirectives())
                        HandleOpcodeAndOperands(label);
                    label.reset();
                }
            }
            break;
        case TokenType::ORGCommand:
            {
                Get();
                Expect(TokenType::Number);
                ast.AddNode(Nodes::CreateStatement(ASTNodeType::ORG, lastToken.value, label, lastToken.location));
                label.reset();
                Expect(TokenType::EOL);
            }
            break;
        default:
            {
                std::wostringstream stream;
                stream << L"Unexpected token: " << lastToken.value;
                SemanticError(lastToken.location, stream.str());
                Get();
            }
            break;
        }
    }
    Expect(TokenType::ENDCommand);
    scanner.Pushback(currentToken);
}

template<class OpcodeType, class OperandState>
bool CPUParser<OpcodeType, OperandState>::CheckForAssemblerDirectives()
{
    switch (currentToken.kind)
    {
        case TokenType::EQUCommand:
            {
                Get();
                Expect(TokenType::Number);
                //CreateStatementNode(ASTNodeType::EQU, lastToken.value, label, lastToken.location);
                //label.reset();
                Expect(TokenType::EOL);
            }
            break;
        case TokenType::SETCommand:
            {
                Get();
                Expect(TokenType::Number);
                //CreateStatementNode(ASTNodeType::SET, lastToken.value, label, lastToken.location);
                //label.reset();
                Expect(TokenType::EOL);
            }
            break;
        case TokenType::DBCommand:
            {
                Get();
                Expect(TokenType::Number);
                //CreateStatementNode(ASTNodeType::DB, lastToken.value, label, lastToken.location);
                //label.reset();
                Expect(TokenType::EOL);
            }
            break;
        case TokenType::DWCommand:
            {
                Get();
                Expect(TokenType::Number);
                //CreateStatementNode(ASTNodeType::DW, lastToken.value, label, lastToken.location);
                //label.reset();
                Expect(TokenType::EOL);
            }
            break;
    }
    return false;
}

} // namespace Assembler
