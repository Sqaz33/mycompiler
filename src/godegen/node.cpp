#include "inode.hpp"

#include <cassert>

namespace {

class Number : public inode::INode {
    int val_;

public:
    Number(int val) : val_(val) {}

public:
    llvm::Value* codegen(codegen::CodeGen& cg) override {
        return cg.createNumber(val_);
    }
};

class VarDecl : public inode::INode {
    std::string name_;
    Number* val_;

public:
    VarDecl(const std::string& name, INode* val) :
        name_(name)
        , val_(dynamic_cast<Number*>(val))
    { 
        assert(val_ != nullptr); 
        hasName_ = name;
    }

public:
    llvm::Value* codegen(codegen::CodeGen& cg) override {
        return cg.createVarDecl(val_);
    }

}


} // namespace

