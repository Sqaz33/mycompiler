#include "node.hpp"

#include "codegen.hpp"
#include "symtab.hpp"

namespace node {

Value::Value(int val) : val_(val) {}

llvm::Value* Value::codegen() {
    return codegen::gen->createIntVal(val_);
}

// Decl
Decl::Decl(const std::string& varName) : 
    varName_(varName)
{}

llvm::Value* Decl::codegen() {
    return codegen::gen->addDeclRead(varName_);
}

llvm::Value* Decl::assign(llvm::Value* rhs) {
    return codegen::gen->addDeclWrite(varName_, rhs);
}

// Op 
Op::Op(INode* lhs, 
    Operation op, 
    INode* rhs) :
    lhs_(lhs)
    , op_(op)
    , rhs_(rhs)
{}

Op::~Op() {
    if (lhs_ && dynamic_cast<Decl*>(lhs_) == nullptr) 
        delete lhs_;
    if (rhs_ && dynamic_cast<Decl*>(rhs_) == nullptr) 
        delete rhs_;   
}

llvm::Value* Op::codegen() {
    llvm::Value* rightV = rhs_ ? rhs_->codegen() : nullptr;

    if (op_ == Operation::Assign) 
        return static_cast<Decl*>(lhs_)->assign(rightV);

    llvm::Value* leftV = lhs_ ? lhs_->codegen() : nullptr;
    
    return codegen::gen->addOp(op_, leftV, rightV);
}

// If
If::If(INode* cond, INode* then) :
    cond_(cond)
    , then_(then)
{} 

If::~If() {
    if (cond_ && dynamic_cast<Decl*>(cond_) == nullptr) 
        delete cond_;
    delete then_;
}

llvm::Value* If::codegen() {
    auto condV = cond_->codegen(); 
    auto mergeBB = codegen::gen->startIf(condV);
    then_->codegen();
    codegen::gen->endIf(mergeBB);
    return nullptr;
}

// While 
While::While(INode* cond, INode* body) :
    cond_(cond)
    , body_(body)
{}

While::~While() {
    if (cond_ && dynamic_cast<Decl*>(cond_) == nullptr) 
        delete cond_;
    delete body_;
}

llvm::Value* While::codegen() {
    auto condV1 = cond_->codegen(); 
    auto BBs = codegen::gen->startWhile(condV1);
    body_->codegen();
    auto* condV2 = cond_->codegen();
    codegen::gen->endWhile(condV2, BBs);
    return nullptr;
}

// Scope 
Scope::Scope(IScope* prev) : prev_(prev) {}

Scope::~Scope() {
    for (auto* br : branches_) {
        delete br;
    }
    symtab::tab.free(this);
}

IScope* Scope::push() {
    return new Scope(this);
}

IScope* Scope::reset() {
    return prev_;
} 

void Scope::addBranch(INode* br) {
    branches_.push_back(br);
}

INode* Scope::access(const std::string& name) {
    auto* sym = visible(name);
    if (sym) return sym;
    auto* newSym = new Decl(name);
    symtab::tab.add(this, name, newSym);
    codegen::gen->addDecl(name);
    return newSym;
}

INode* Scope::visible(const std::string& name) {
    auto* sym = 
        symtab::tab.find(this, name);
    if (sym) return sym;
    if (prev_) return prev_->visible(name);
    return nullptr;
}

llvm::Value* Scope::codegen() {
    for (auto* br : branches_) {
        br->codegen();
    } 
    return nullptr;
}

INode* makeOp(INode* lhs, Operation op, INode* rhs) {
    return new Op(lhs, op, rhs);
}

INode* makeVal(int val) { 
    return new Value(val);
}

INode* makeIf(INode* cond, INode* then) {
    return new If(cond, then);
}

INode* makeWhile(INode* cond, INode* body) {
    return new While(cond, body);
}

} // namespace node