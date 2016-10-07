#include "assembler/AbstractSyntaxTree.h"

namespace Assembler
{

std::wostream & operator << (std::wostream & stream, ASTNodeType nodeType)
{
    switch (nodeType)
    {
        case ASTNodeType::Tree:             stream << L"Tree"; break;
        case ASTNodeType::StatementLine:    stream << L"StatementLine"; break;
        case ASTNodeType::Comment:          stream << L"Comment"; break;
        case ASTNodeType::Label:            stream << L"Label"; break;
        case ASTNodeType::Empty:            stream << L"Empty"; break;
        case ASTNodeType::CPU:              stream << L"CPU"; break;
        case ASTNodeType::ORG:              stream << L"ORG"; break;
        case ASTNodeType::END:              stream << L"END"; break;
        case ASTNodeType::EQU:              stream << L"EQU"; break;
        case ASTNodeType::SET:              stream << L"SET"; break;
        case ASTNodeType::DB:               stream << L"DB"; break;
        case ASTNodeType::DW:               stream << L"DW"; break;
        case ASTNodeType::Opcode:           stream << L"Opcode"; break;
        case ASTNodeType::Register8:        stream << L"Register8"; break;
        case ASTNodeType::Register16:       stream << L"Register16"; break;
        case ASTNodeType::Expression:       stream << L"Expression"; break;
        case ASTNodeType::LocCounter:       stream << L"LocCounter"; break;
        case ASTNodeType::RefAddress:       stream << L"RefAddress"; break;
        case ASTNodeType::RefData:          stream << L"RefData"; break;
        case ASTNodeType::Data8:            stream << L"Data8"; break;
        case ASTNodeType::Data16:           stream << L"Data16"; break;
        case ASTNodeType::RSTCode:          stream << L"RSTCode"; break;
        case ASTNodeType::UndefinedOperand: stream << L"UndefinedOperand"; break;
    }
    return stream;
}

ASTree::ASTree()
    : ASTNode(ASTNodeType::Tree, L"", Location())
{
}

ASTree::~ASTree()
{
}

ASTNode::Ptr ASTree::FirstNode() const
{
    return ASTNode::FirstChild();
}

ASTNode::Ptr ASTree::Next() const
{
    return ASTNode::Next();
}

void ASTree::AddNode(ASTNode::Ptr node)
{
    ASTNode::AddChild(node);
}

ASTNode::ASTNode(ASTNodeType type, std::wstring const & value, Location const & location)
    : type(type)
    , value(value)
    , next()
    , location(location)
    , firstChild()
    , lastChild()
{
}

ASTNode::~ASTNode()
{
    next = nullptr;
    firstChild = nullptr;
    lastChild = nullptr;
}

ASTNodeType ASTNode::NodeType() const
{
    return type;
}

std::wstring const & ASTNode::Value() const
{
    return value;
}

Location const & ASTNode::Loc() const
{
    return location;
}

ASTNode::Ptr ASTNode::FirstChild() const
{
    return firstChild;
}

ASTNode::Ptr ASTNode::Next() const
{
    return next;
}

void ASTNode::AddChild(ASTNode::Ptr child)
{
    if (firstChild == nullptr)
    {
        firstChild = lastChild = child;
    }
    else
    {
        lastChild->AddSibling(child);
        lastChild = child;
    }
}

void ASTNode::AddSibling(ASTNode::Ptr sibling)
{
    next = sibling;
}

} // namespace Assembler
