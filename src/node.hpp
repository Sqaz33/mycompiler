#ifndef NODE_HPP
#define NODE_HPP

#include <vector>

#include "inode.hpp"

namespace node {

class Value : public INode {
    int val_;

public:
    Value(int val);

public:
    llvm::Value* codegen() override;
};

class Decl : public INode {
    std::string varName_;

public:
    Decl(const std::string& varName);

public:
    llvm::Value* codegen() override;
    llvm::Value* assign(llvm::Value* rhs);
};

class Op : public INode {
    INode* lhs_;
    Operation op_;
    INode* rhs_;

public:
    Op(INode* lhs, 
       Operation op, 
       INode* rhs);

    ~Op();
    
public:
    llvm::Value* codegen() override;
};

class If : public INode {
    INode* cond_;
    INode* then_;

public:
    If(INode* cond, INode* then);

    ~If();

public:
    llvm::Value* codegen() override;
};

class IfElse : public INode {
    If* if_;
    INode* cond_;
    INode* elseBody_;

public:
    IfElse(INode* cond, INode* ifBody, INode* elseBody);

    ~IfElse();

public:
    llvm::Value* codegen() override;
};

class While : public INode {
    INode* cond_;
    INode* body_;

public:
    While(INode* cond, INode* body);

    ~While();

public:
    llvm::Value* codegen() override;
};

class Scope : public IScope {
    IScope* prev_;
    std::vector<INode*> branches_;

public:
    Scope(IScope* prev = nullptr);

    ~Scope();

public:
    IScope* push() override;
    IScope* reset() override;
    void addBranch(INode* br) override;
    INode* access(const std::string& name) override;
    INode* visible(const std::string& name) override;

public:
    llvm::Value* codegen() override;
};

} // namespace node

#endif // NODE_HPP