#pragma once

#include <iostream>
#include <memory>
#include <vector>

namespace Simulate
{

class TreeNode
{
public:
    TreeNode();
    virtual ~TreeNode();

    virtual void Load(std::istream & stream);
    virtual void Save(std::ostream & stream);
protected:
    std::vector<std::shared_ptr<TreeNode>> children;
};

class Keyword : public TreeNode
{
    std::string value;
};

class Statement : public TreeNode
{

};

class WhileStatement : public Statement
{

};

class Expression : public TreeNode
{

};

class AbstractSyntaxTree
{
public:
    AbstractSyntaxTree();
    virtual ~AbstractSyntaxTree();

    void Load(std::istream & stream);
    void Save(std::ostream & stream);

protected:
    std::shared_ptr<TreeNode> root;
};

} // namespace Simulate
