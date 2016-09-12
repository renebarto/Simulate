#include "AbstractSyntaxTree.h"

namespace Assembler
{

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
