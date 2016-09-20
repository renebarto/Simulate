#pragma once

#include <sstream>
#include "core/Util.h"
#include "Exceptions.h"
#include "CPUType.h"
#include "ICPUParser.h"
#include "Scanner.h"
#include "ErrorHandler.h"
#include "KeywordMap.h"
#include "Nodes.h"
#include "OpcodeMap.h"
#include "SymbolMap.h"
#include "AbstractSyntaxTree.h"

namespace Assembler
{

inline int64_t ConvertToValue(std::wstring const & text)
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

template<class AddressType>
struct Label
{
    std::wstring name;
    bool locationDefined;
    AddressType location;
    Label(std::wstring const & name)
        : name(name)
        , locationDefined(false)
        , location()
    {
    }
    Label(std::wstring const & name, AddressType location)
        : name(name)
        , locationDefined(true)
        , location(location)
    {
    }
    void SetLocation(AddressType value)
    {
        if (locationDefined)
        {
            std::ostringstream stream;
            stream << "Label already has location defined: " << Core::String::ToString(name) << "= " << location;
            throw AssemblerException(stream.str());
        }
        location = value;
        locationDefined = true;
    }
};

template<class OperandType, class AddressType>
struct InstructionMapping
{
    OperandType operandType;
    AddressType instructionSize;

    bool operator == (InstructionMapping const & other) const
    {
        return (operandType == other.operandType) &&
               (instructionSize == other.instructionSize);
    }
};

template<class OpcodeType, class OperandType, class AddressType>
class CPUParser : public ICPUParser
{
public:
	CPUParser(CPUType cpuType, Scanner & scanner, ErrorHandler & errorHandler, std::wostream & reportStream);
	virtual ~CPUParser();

	void Parse() override;
	void Generate(ICPUAssembler & assembler) override;

    ASTree const & GetAST() const { return ast; }

protected:
    CPUType cpuType;
    Scanner & scanner;
	ErrorHandler & errorHandler;
	std::wostream & reportStream;
    Token currentToken;
    Token lastToken;

    void AddCPUNode();

    SymbolMap<Label<AddressType>> labels;
    void AddLabel(Label<AddressType> const & label);
    void UpdateLabel(std::wstring const & name, AddressType value);
    Label<AddressType> const & GetLabel(std::wstring const & name) const;
    bool HaveLabel(std::wstring const & name) const;

    KeywordMap<OpcodeType> opcodes;

    ASTree ast;
    AddressType programCounter;

	void SyntaxError(TokenType tokenType);
    void SemanticError(Location const & location, std::wstring const & msg);
	void SemanticError(std::wstring const & msg);
	void Get();
	void Expect(TokenType tokenType);
    void ParseAssembler();
    void HandleComment();
    bool CheckForAssemblerDirectives();

    virtual void HandleOpcodeAndOperands(LabelNode::Ptr label) = 0;
    virtual void HandleOperands(typename OpcodeNode<OpcodeType, AddressType>::Ptr opcode, OperandType state) = 0;
}; // CPUParser

template<class OpcodeType, class OperandType, class AddressType>
CPUParser<OpcodeType, OperandType, AddressType>::CPUParser(CPUType cpuType, Scanner & scanner, ErrorHandler & errorHandler, std::wostream & reportStream)
    : cpuType(cpuType)
    , scanner(scanner)
    , errorHandler(errorHandler)
    , reportStream(reportStream)
    , currentToken()
    , lastToken()
    , labels()
    , opcodes()
    , ast()
    , programCounter()
{
}

template<class OpcodeType, class OperandType, class AddressType>
CPUParser<OpcodeType, OperandType, AddressType>::~CPUParser()
{
}

template<class OpcodeType, class OperandType, class AddressType>
void CPUParser<OpcodeType, OperandType, AddressType>::Parse()
{
    currentToken = Token();
    lastToken = Token();
    Get();
    AddCPUNode();
    ParseAssembler();
}

template<class OpcodeType, class OperandType, class AddressType>
void CPUParser<OpcodeType, OperandType, AddressType>::Generate(ICPUAssembler & assembler)
{
}

template<class OpcodeType, class OperandType, class AddressType>
void CPUParser<OpcodeType, OperandType, AddressType>::AddCPUNode()
{
    std::wostringstream stream;
    stream << cpuType;
    ast.AddNode(Nodes::CreateNode(ASTNodeType::CPU, stream.str(), Location(1, 1, 0)));
}

template<class OpcodeType, class OperandType, class AddressType>
void CPUParser<OpcodeType, OperandType, AddressType>::AddLabel(Label<AddressType> const & label)
{
    labels.Add(label.name, label);
}

template<class OpcodeType, class OperandType, class AddressType>
void CPUParser<OpcodeType, OperandType, AddressType>::UpdateLabel(std::wstring const & name, AddressType value)
{
    Label<AddressType> & label = labels.Lookup(name);
    label.SetLocation(value);
}

template<class OpcodeType, class OperandType, class AddressType>
Label<AddressType> const & CPUParser<OpcodeType, OperandType, AddressType>::GetLabel(std::wstring const & name) const
{
    return labels.Lookup(name);
}

template<class OpcodeType, class OperandType, class AddressType>
bool CPUParser<OpcodeType, OperandType, AddressType>::HaveLabel(std::wstring const & name) const
{
    return labels.Exists(name);
}

template<class OpcodeType, class OperandType, class AddressType>
void CPUParser<OpcodeType, OperandType, AddressType>::SyntaxError(TokenType tokenType)
{
    errorHandler.SyntaxError(currentToken.location, tokenType);
}

template<class OpcodeType, class OperandType, class AddressType>
void CPUParser<OpcodeType, OperandType, AddressType>::SemanticError(Location const & location, std::wstring const & msg)
{
    errorHandler.Error(location, msg);
}

template<class OpcodeType, class OperandType, class AddressType>
void CPUParser<OpcodeType, OperandType, AddressType>::SemanticError(std::wstring const & msg)
{
    SemanticError(currentToken.location, msg);
}

template<class OpcodeType, class OperandType, class AddressType>
void CPUParser<OpcodeType, OperandType, AddressType>::Get()
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

template<class OpcodeType, class OperandType, class AddressType>
void CPUParser<OpcodeType, OperandType, AddressType>::Expect(TokenType tokenType)
{
    if (currentToken.kind == tokenType) 
        Get(); 
    else 
        SyntaxError(tokenType);
}

template<class OpcodeType, class OperandType, class AddressType>
void CPUParser<OpcodeType, OperandType, AddressType>::ParseAssembler()
{
    bool done = false;
    LabelNode::Ptr label;
    programCounter = 0;
    while (!done)
    {
        switch (currentToken.kind)
        {
        case TokenType::EndOfFile:
            Expect(TokenType::ENDCommand);
            ast.AddNode(Nodes::CreateStatement(ASTNodeType::END, lastToken.value, label, lastToken.location));
            label.reset();
            done = true;
            break;
        case TokenType::EOL:
            Get();
            break;
        case TokenType::ENDCommand:
            Get();
            ast.AddNode(Nodes::CreateStatement(ASTNodeType::END, lastToken.value, label, lastToken.location));
            label.reset();
            HandleComment();
            done = true;
            break;
        case TokenType::Identifier:
            {
                Get();
                if (currentToken.kind == TokenType::Colon)
                {
                    label = Nodes::CreateLabel(lastToken.value, lastToken.location);
                    if (!HaveLabel(lastToken.value))
                        AddLabel(Label<AddressType>(lastToken.value, programCounter));
                    else
                        UpdateLabel(lastToken.value, programCounter);
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
                programCounter = AddressType(ConvertToValue(lastToken.value));
                label.reset();
            }
            break;
        case TokenType::Comment:
            HandleComment();
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
    scanner.Pushback(currentToken);
}

template<class OpcodeType, class OperandType, class AddressType>
void CPUParser<OpcodeType, OperandType, AddressType>::HandleComment()
{
    if (currentToken.kind == TokenType::Comment)
    {
        Get();
        ast.AddNode(Nodes::CreateNode(ASTNodeType::Comment, lastToken.value, lastToken.location));
    }
}

template<class OpcodeType, class OperandType, class AddressType>
bool CPUParser<OpcodeType, OperandType, AddressType>::CheckForAssemblerDirectives()
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
