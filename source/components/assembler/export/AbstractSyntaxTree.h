#pragma once

#include <memory>
#include <string>
#include "Location.h"

namespace Assembler
{

enum class ASTNodeType
{
    Tree,
    Opcode,
    Register8,
    Register16,
    Expression,
    LocCounter,
    Ref,
    Data8,
    Data16,
    RSTCode,
    Label,
    ORG,
    EQU,
    SET,
    DB,
    DW,
};

class ASTNode
{
public:
    using Ptr = std::shared_ptr<ASTNode>;
    ASTNode(ASTNodeType type, std::wstring const & value, Location const & location);
    virtual ~ASTNode();

    ASTNodeType NodeType() const;
    std::wstring const & Value() const;
    Location const & Loc() const;
    Ptr FirstChild() const;
    Ptr Next() const;

    void AddChild(Ptr child);
    void AddSibling(Ptr sibling);

private:
    ASTNodeType type;
    std::wstring value;
    Location location;
    Ptr next;
    Ptr firstChild;
    Ptr lastChild;
};

class ASTree : protected ASTNode
{
public:
    ASTree();
    virtual ~ASTree();

    Ptr FirstNode() const;
    Ptr Next() const;
    void AddNode(Ptr node);
};

} // namespace Assembler
