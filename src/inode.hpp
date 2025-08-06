#ifndef INODE_HPP
#define INODE_HPP

#include <string>

#include <llvm/IR/Value.h>

namespace codegen {
    class CodeGen;
}

namespace node {
 
// start AST interface   
struct INode {
    virtual llvm::Value* codegen() = 0;   
    virtual ~INode() = default;
};

struct IScope : public INode {
    virtual IScope* push() = 0;
    virtual IScope* reset() = 0;
    virtual void addBranch(INode* br) = 0;
    virtual INode* access(const std::string& name) = 0;
    virtual INode* visible(const std::string& name) = 0;    
};
// end AST interface

// ops
enum class Operation {
  Plus,
  Minus,
  Assign,
  Greater,
  Less,
  GreaterEq,
  LessEq,
  Out,
  In,
  Equal,
  NotEqual,
  Div,
  Mul,
  Mod,
  Not,
  And,
  Or
};

INode* makeOp(INode* lhs, Operation op, INode* rhs);
INode* makeVal(int val);
INode* makeIf(INode* cond, INode* then);
INode* makeIfElse(INode* cond, INode* ifBody, INode* elseBody);
INode* makeWhile(INode* cond, INode* body);

extern IScope* curScope;

}

#endif // INODE_HPP