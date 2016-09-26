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
#include "PrettyPrinter.h"

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

template<class SegmentType, class AddressType>
struct Symbol
{
    std::wstring name;
    bool locationDefined;
    AddressType location;
    SegmentType segment;
    Symbol(std::wstring const & name)
        : name(name)
        , locationDefined(false)
        , location()
    {
    }
    Symbol(std::wstring const & name, SegmentType segment, AddressType location)
        : name(name)
        , locationDefined(true)
        , location(location)
        , segment(segment)
    {
    }
    void SetLocation(SegmentType segment, AddressType location)
    {
        if (locationDefined)
        {
            std::ostringstream stream;
            stream << "Symbol already has location defined: " << Core::String::ToString(name) << "= " << segment << ":" << location;
            throw AssemblerException(stream.str());
        }
        this->location = location;
        this->segment = segment;
        locationDefined = true;
    }
};

struct SymbolReference
{
    std::wstring name;
    std::vector<size_t> references;
    SymbolReference(std::wstring const & name)
        : name(name)
        , references()
    {
    }
    void AddReference(size_t line)
    {
        references.push_back(line);
    }
};

template<class SegmentType, class AddressType>
struct SegmentDescriptor
{
    SegmentType segment;
    std::wstring name;
    AddressType offset;
    AddressType size;
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

template<class OpcodeType, class OperandType, class SegmentType, class AddressType>
class CPUParser : public ICPUParser
{
public:
	CPUParser(CPUType cpuType, Scanner & scanner, ErrorHandler & errorHandler, PrettyPrinter<wchar_t> & reportStream);
	virtual ~CPUParser();

	void Parse() override;
	void Generate(ICPUAssembler & assembler) override;

    CPUType GetCPUType() const { return cpuType; }
    ASTree const & GetAST() const { return ast; }

    SymbolMap<Symbol<SegmentType, AddressType>> const & GetLabels() const;
    Symbol<SegmentType, AddressType> const & GetLabel(std::wstring const & name) const;
    bool HaveLabel(std::wstring const & name) const;

protected:
    CPUType cpuType;
    Scanner & scanner;
	ErrorHandler & errorHandler;
    PrettyPrinter<wchar_t> & printer;
	Token currentToken;
    Token lastToken;
    StatementLineNode::Ptr currentStatementLine;

    void AddCPUNode();

    SymbolMap<Symbol<SegmentType, AddressType>> labels;
    void AddLabel(Symbol<SegmentType, AddressType> const & label);
    void UpdateLabel(std::wstring const & name, SegmentType segment, AddressType location);

    KeywordMap<OpcodeType> opcodes;

    ASTree ast;
    AddressType programCounter;
    std::vector<SegmentDescriptor<SegmentType, AddressType>> segments;

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

template<class OpcodeType, class OperandType, class SegmentType, class AddressType>
CPUParser<OpcodeType, OperandType, SegmentType, AddressType>::CPUParser(CPUType cpuType, Scanner & scanner, ErrorHandler & errorHandler, PrettyPrinter<wchar_t> & reportStream)
    : cpuType(cpuType)
    , scanner(scanner)
    , errorHandler(errorHandler)
    , printer(reportStream)
    , currentToken()
    , lastToken()
    , labels()
    , opcodes()
    , ast()
    , programCounter()
{
}

template<class OpcodeType, class OperandType, class SegmentType, class AddressType>
CPUParser<OpcodeType, OperandType, SegmentType, AddressType>::~CPUParser()
{
}

template<class OpcodeType, class OperandType, class SegmentType, class AddressType>
void CPUParser<OpcodeType, OperandType, SegmentType, AddressType>::Parse()
{
    currentToken = Token();
    lastToken = Token();
    Get();
    AddCPUNode();
    ParseAssembler();
}

template<class OpcodeType, class OperandType, class SegmentType, class AddressType>
void CPUParser<OpcodeType, OperandType, SegmentType, AddressType>::Generate(ICPUAssembler & assembler)
{
}

template<class OpcodeType, class OperandType, class SegmentType, class AddressType>
void CPUParser<OpcodeType, OperandType, SegmentType, AddressType>::AddCPUNode()
{
    std::wostringstream stream;
    stream << cpuType;
    currentStatementLine = Nodes::CreateStatementLine(nullptr, Nodes::CreateCPUNode(stream.str(), Location(1, 1, 0)));
    ast.AddNode(currentStatementLine);
    HandleComment();
}

template<class OpcodeType, class OperandType, class SegmentType, class AddressType>
void CPUParser<OpcodeType, OperandType, SegmentType, AddressType>::AddLabel(Symbol<SegmentType, AddressType> const & label)
{
    labels.Add(label.name, label);
}

template<class OpcodeType, class OperandType, class SegmentType, class AddressType>
void CPUParser<OpcodeType, OperandType, SegmentType, AddressType>::UpdateLabel(std::wstring const & name, SegmentType segment, AddressType location)
{
    Symbol<SegmentType, AddressType> & label = labels.Lookup(name);
    label.SetLocation(segment, location);
}

template<class OpcodeType, class OperandType, class SegmentType, class AddressType>
SymbolMap<Symbol<SegmentType, AddressType>> const & CPUParser<OpcodeType, OperandType, SegmentType, AddressType>::GetLabels() const
{
    return labels;
}

template<class OpcodeType, class OperandType, class SegmentType, class AddressType>
Symbol<SegmentType, AddressType> const & CPUParser<OpcodeType, OperandType, SegmentType, AddressType>::GetLabel(std::wstring const & name) const
{
    return labels.Lookup(name);
}

template<class OpcodeType, class OperandType, class SegmentType, class AddressType>
bool CPUParser<OpcodeType, OperandType, SegmentType, AddressType>::HaveLabel(std::wstring const & name) const
{
    return labels.Exists(name);
}

template<class OpcodeType, class OperandType, class SegmentType, class AddressType>
void CPUParser<OpcodeType, OperandType, SegmentType, AddressType>::SyntaxError(TokenType tokenType)
{
    errorHandler.SyntaxError(currentToken.location, tokenType);
}

template<class OpcodeType, class OperandType, class SegmentType, class AddressType>
void CPUParser<OpcodeType, OperandType, SegmentType, AddressType>::SemanticError(Location const & location, std::wstring const & msg)
{
    errorHandler.Error(location, msg);
}

template<class OpcodeType, class OperandType, class SegmentType, class AddressType>
void CPUParser<OpcodeType, OperandType, SegmentType, AddressType>::SemanticError(std::wstring const & msg)
{
    SemanticError(currentToken.location, msg);
}

template<class OpcodeType, class OperandType, class SegmentType, class AddressType>
void CPUParser<OpcodeType, OperandType, SegmentType, AddressType>::Get()
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

template<class OpcodeType, class OperandType, class SegmentType, class AddressType>
void CPUParser<OpcodeType, OperandType, SegmentType, AddressType>::Expect(TokenType tokenType)
{
    if (currentToken.kind == tokenType) 
        Get(); 
    else 
        SyntaxError(tokenType);
}

template<class OpcodeType, class OperandType, class SegmentType, class AddressType>
void CPUParser<OpcodeType, OperandType, SegmentType, AddressType>::ParseAssembler()
{
    bool done = false;
    LabelNode::Ptr label;
    programCounter = 0;
    while (!done)
    {
        switch (currentToken.kind)
        {
        case TokenType::Identifier:
            {
                Get();
                if (currentToken.kind == TokenType::Colon)
                {
                    label = Nodes::CreateLabel(lastToken.value, lastToken.location);
                    if (!HaveLabel(lastToken.value))
                        AddLabel(Symbol<SegmentType, AddressType>(lastToken.value, SegmentType{}, programCounter));
                    else
                        UpdateLabel(lastToken.value, SegmentType{}, programCounter);
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
        case TokenType::EndOfFile:
            {
                Expect(TokenType::ENDCommand);
                StatementNode::Ptr statement = Nodes::CreateStatement(ASTNodeType::END, lastToken.value, lastToken.location);
                currentStatementLine = Nodes::CreateStatementLine(label, statement);
                ast.AddNode(currentStatementLine);
                label.reset();
                done = true;
            }
            break;
        case TokenType::EOL:
            Get();
            break;
        case TokenType::ORGCommand:
            {
                Get();
                Expect(TokenType::Number);
                StatementNode::Ptr statement = Nodes::CreateStatement(ASTNodeType::ORG, lastToken.value, lastToken.location);
                currentStatementLine = Nodes::CreateStatementLine(label, statement);
                ast.AddNode(currentStatementLine);
                programCounter = AddressType(ConvertToValue(lastToken.value));
                label.reset();
                HandleComment();
            }
            break;
        case TokenType::ENDCommand:
            {
                Get();
                StatementNode::Ptr statement = Nodes::CreateStatement(ASTNodeType::END, lastToken.value, lastToken.location);
                currentStatementLine = Nodes::CreateStatementLine(label, statement);
                ast.AddNode(currentStatementLine);
                label.reset();
                HandleComment();
                done = true;
            }
            break;
        case TokenType::Comment:
            {
                StatementNode::Ptr statement = Nodes::CreateStatement(ASTNodeType::Empty, lastToken.value, lastToken.location);
                currentStatementLine = Nodes::CreateStatementLine(label, statement);
                ast.AddNode(currentStatementLine);
                label.reset();
                HandleComment();
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
    scanner.Pushback(currentToken);
}

template<class OpcodeType, class OperandType, class SegmentType, class AddressType>
void CPUParser<OpcodeType, OperandType, SegmentType, AddressType>::HandleComment()
{
    if (currentToken.kind == TokenType::Comment)
    {
        Get();
        currentStatementLine->SetComment(Nodes::CreateCommentNode(lastToken.value, lastToken.location));
    }
}

template<class OpcodeType, class OperandType, class SegmentType, class AddressType>
bool CPUParser<OpcodeType, OperandType, SegmentType, AddressType>::CheckForAssemblerDirectives()
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
