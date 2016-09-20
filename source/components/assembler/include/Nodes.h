#pragma once

#include "AbstractSyntaxTree.h"
#include "MachineCode.h"

namespace Assembler
{

class CPUNode : public ASTNode
{
public:
    using Ptr = std::shared_ptr<CPUNode>;
    CPUNode(std::wstring const & value, Location const & location)
        : ASTNode(ASTNodeType::CPU, value, location)
    {}
    virtual ~CPUNode() {}

    std::wstring ToString() const override { return L"CPU " + Value(); }
};

class ORGNode : public ASTNode
{
public:
    using Ptr = std::shared_ptr<ORGNode>;
    ORGNode(std::wstring const & value, Location const & location)
        : ASTNode(ASTNodeType::ORG, value, location)
    {}
    virtual ~ORGNode() {}

    std::wstring ToString() const override { return L"ORG " + Value(); }
};

class ENDNode : public ASTNode
{
public:
    using Ptr = std::shared_ptr<ENDNode>;
    ENDNode(std::wstring const & value, Location const & location)
        : ASTNode(ASTNodeType::END, value, location)
    {}
    virtual ~ENDNode() {}

    std::wstring ToString() const override { return L"END"; }
};

class LabelNode : public ASTNode
{
public:
    using Ptr = std::shared_ptr<LabelNode>;
    LabelNode(std::wstring const & value, Location const & location)
        : ASTNode(ASTNodeType::Label, value, location)
    {}
    virtual ~LabelNode() {}

    std::wstring ToString() const override { return Value() + L":"; }
};

template<class AddressType>
class RefAddressNode : public ASTNode
{
public:
    using Ptr = std::shared_ptr<RefAddressNode<AddressType>>;
    RefAddressNode(std::wstring const & value, AddressType address, Location const & location)
        : ASTNode(ASTNodeType::RefAddress, value, location)
        , address(address)
    {}
    virtual ~RefAddressNode() {}

    AddressType Address() const { return address; }

private:
    AddressType address;
};

template<class DataType>
class RefDataNode : public ASTNode
{
public:
    using Ptr = std::shared_ptr<RefDataNode<DataType>>;
    RefDataNode(std::wstring const & value, DataType data, Location const & location)
        : ASTNode(ASTNodeType::RefAddress, value, location)
        , data(data)
    {}
    virtual ~RefDataNode() {}

    DataType Data() const { return data; }

private:
    DataType data;
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

template<class OpcodeType, class AddressType>
class OpcodeNode : public StatementNode
{
public:
    using Ptr = std::shared_ptr<OpcodeNode<OpcodeType, AddressType>>;
    OpcodeNode(AddressType address, OpcodeType opcodeType, std::wstring const & value, LabelNode::Ptr label, Location const & location)
        : StatementNode(ASTNodeType::Opcode, value, label, location)
        , opcodeType(opcodeType)
        , address(address)
        , machineCode()
    {}
    virtual ~OpcodeNode() {}

    OpcodeType Type() const { return opcodeType; }
    AddressType Address() const { return address; }
    MachineCode Code() const { return machineCode; }
    void SetCode(MachineCode const & code) { machineCode = code; }

private:
    OpcodeType opcodeType;
    AddressType address;
    MachineCode machineCode;
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

template<class AddressType>
class LocCounterNode : public ASTNode
{
public:
    LocCounterNode(AddressType address, std::wstring const & valueStr, Location const & location)
        : ASTNode(ASTNodeType::LocCounter, valueStr, location)
        , address(address)
    {}
    virtual ~LocCounterNode() {}

    std::wstring const & Data() const { return ASTNode::Value(); }
    AddressType Address() const { return address; }

private:
    AddressType address;
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
    template<class AddressType>
    static typename RefAddressNode<AddressType>::Ptr CreateRefAddress(std::wstring const & value, AddressType address, Location const & location)
    {
        return std::make_shared<RefAddressNode<AddressType>>(value, address, location);
    }
    template<class AddressType>
    static typename RefAddressNode<AddressType>::Ptr CreateRefAddressUndefined(std::wstring const & value, Location const & location)
    {
        return std::make_shared<RefAddressNode<AddressType>>(value, 0, location);
    }
    template<class DataType>
    static typename RefDataNode<DataType>::Ptr CreateRefData(std::wstring const & value, DataType data, Location const & location)
    {
        return std::make_shared<RefDataNode<DataType>>(value, data, location);
    }
    static ASTNode::Ptr CreateNode(ASTNodeType type, std::wstring const & value, Location const & location)
    {
        return std::make_shared<ASTNode>(type, value, location);
    }
    static StatementNode::Ptr CreateStatement(ASTNodeType type, std::wstring const & value, LabelNode::Ptr label, Location const & location)
    {
        return std::make_shared<StatementNode>(type, value, label, location);
    }
    template<class OpcodeType, class AddressType>
    static typename OpcodeNode<OpcodeType, AddressType>::Ptr CreateOpcode(AddressType address, OpcodeType opcodeType, std::wstring const & value, LabelNode::Ptr label, Location const & location)
    {
        return std::make_shared<OpcodeNode<OpcodeType, AddressType>>(address, opcodeType, value, label, location);
    }
    static ExpressionNode::Ptr CreateExpression(std::wstring const & valueStr, Location const & location)
    {
        return std::make_shared<ExpressionNode>(valueStr, location);
    }
    template<class AddressType>
    static typename LocCounterNode<AddressType>::Ptr CreateLocCounter(AddressType address, std::wstring const & valueStr, Location const & location)
    {
        return std::make_shared<LocCounterNode<AddressType>>(address, valueStr, location);
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

