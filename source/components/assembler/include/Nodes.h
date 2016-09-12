#pragma once

#include "AbstractSyntaxTree.h"

namespace Assembler
{

class LabelNode : public ASTNode
{
public:
    using Ptr = std::shared_ptr<LabelNode>;
    LabelNode(std::wstring const & value, Location const & location)
        : ASTNode(ASTNodeType::Label, value, location)
    {}
    virtual ~LabelNode() {}
};

class RefNode : public ASTNode
{
public:
    using Ptr = std::shared_ptr<RefNode>;
    RefNode(std::wstring const & value, Location const & location)
        : ASTNode(ASTNodeType::Ref, value, location)
    {}
    virtual ~RefNode() {}
};

class StatementNode : public ASTNode
{
public:
    using Ptr = std::shared_ptr<StatementNode>;
    StatementNode(ASTNodeType type, std::wstring const & value, LabelNode::Ptr label, Location const & location)
        : ASTNode(type, value, location)
        , label(label)
    {}
    virtual ~StatementNode() {}
    LabelNode::Ptr Label() const { return label; }

private:
    LabelNode::Ptr label;
};

template<class OpcodeType>
class OpcodeNode : public StatementNode
{
public:
    using Ptr = std::shared_ptr<OpcodeNode<OpcodeType>>;
    OpcodeNode(OpcodeType opcodeType, std::wstring const & value, LabelNode::Ptr label, Location const & location)
        : StatementNode(ASTNodeType::Opcode, value, label, location)
        , opcodeType(opcodeType)
    {}
    virtual ~OpcodeNode() {}

    OpcodeType Type() const { return opcodeType; }

private:
    OpcodeType opcodeType;
};

template<class Register8Type>
class Register8Node : public ASTNode
{
public:
    Register8Node(Register8Type registerType, std::wstring const & value, Location const & location)
        : ASTNode(ASTNodeType::Register8, value, location)
        , registerType(registerType)
    {}
    virtual ~Register8Node() {}

    Register8Type RegisterType() const { return registerType; }
private:
    Register8Type registerType;    
};

template<class Register16Type>
class Register16Node : public ASTNode
{
public:
    Register16Node(Register16Type registerType, std::wstring const & value, Location const & location)
        : ASTNode(ASTNodeType::Register16, value, location)
        , registerType(registerType)
    {}
    virtual ~Register16Node() {}

    Register16Type RegisterType() const { return registerType; }
private:
    Register16Type registerType;    
};

class ExpressionNode : public ASTNode
{
public:
    ExpressionNode(std::wstring const & valueStr, Location const & location)
        : ASTNode(ASTNodeType::Expression, valueStr, location)
    {}
    virtual ~ExpressionNode() {}

    std::wstring const & Data() const { return ASTNode::Value(); }
};

class LocCounterNode : public ASTNode
{
public:
    LocCounterNode(std::wstring const & valueStr, Location const & location)
        : ASTNode(ASTNodeType::LocCounter, valueStr, location)
    {}
    virtual ~LocCounterNode() {}

    std::wstring const & Data() const { return ASTNode::Value(); }
};

class Data8Node : public ASTNode
{
public:
    Data8Node(std::wstring const & valueStr, Location const & location)
        : ASTNode(ASTNodeType::Data8, valueStr, location)
    {}
    virtual ~Data8Node() {}

    std::wstring const & Data() const { return ASTNode::Value(); }
};

class Data16Node : public ASTNode
{
public:
    Data16Node(std::wstring const & valueStr, Location const & location)
        : ASTNode(ASTNodeType::Data16, valueStr, location)
    {}
    virtual ~Data16Node() {}

    std::wstring const & Data() const { return ASTNode::Value(); }
};

struct Nodes
{
    static LabelNode::Ptr CreateLabel(std::wstring const & value, Location const & location)
    {
        return std::make_shared<LabelNode>(value, location);
    }
    static RefNode::Ptr CreateRef(std::wstring const & value, Location const & location)
    {
        return std::make_shared<RefNode>(value, location);
    }
    static StatementNode::Ptr CreateStatement(ASTNodeType type, std::wstring const & value, LabelNode::Ptr label, Location const & location)
    {
        return std::make_shared<StatementNode>(type, value, label, location);
    }
    template<class OpcodeType>
    static typename OpcodeNode<OpcodeType>::Ptr CreateOpcode(OpcodeType opcodeType, std::wstring const & value, LabelNode::Ptr label, Location const & location)
    {
        return std::make_shared<OpcodeNode<OpcodeType>>(opcodeType, value, label, location);
    }
    static ExpressionNode::Ptr CreateExpression(std::wstring const & valueStr, Location const & location)
    {
        return std::make_shared<ExpressionNode>(valueStr, location);
    }
    static LocCounterNode::Ptr CreateLocCounter(std::wstring const & valueStr, Location const & location)
    {
        return std::make_shared<LocCounterNode>(valueStr, location);
    }
    static Data8Node::Ptr CreateData8(std::wstring const & valueStr, Location const & location)
    {
        return std::make_shared<Data8Node>(valueStr, location);
    }
    static Data16Node::Ptr CreateData16(std::wstring const & valueStr, Location const & location)
    {
        return std::make_shared<Data16Node>(valueStr, location);
    }
};

} // namespace Assembler

