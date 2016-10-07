#pragma once

#include <memory>
#include <string>
#include "assembler/Location.h"

namespace Assembler
{

enum class ASTNodeType
{
    Tree,
    StatementLine,
    Comment,
    Label,
    // Statements
    Empty,
    CPU,
    ORG,
    END,
    EQU,
    SET,
    DB,
    DW,
    Opcode,
    //Operands
    Register8,
    Register16,
    Expression,
    LocCounter,
    RefAddress,
    RefData,
    Data8,
    Data16,
    RSTCode,
    UndefinedOperand,
};
std::wostream & operator << (std::wostream & stream, ASTNodeType nodeType);

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

    virtual std::wstring ToString() const { return Value(); }

protected:
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

inline std::wostream & operator << (std::wostream & stream, ASTNode const & node)
{
    stream << node.Value();
    return stream;
}

} // namespace Assembler
