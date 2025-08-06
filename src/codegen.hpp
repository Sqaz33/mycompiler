#ifndef CODEGEN_HPP
#define CODEGEN_HPP

#include <string>
#include <unordered_map>
#include <utility>
#include <memory>

#include "llvm/ADT/APInt.h"
#include "llvm/IR/BasicBlock.h" 
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"

#include "inode.hpp"

namespace codegen {

class CodeGen {
    llvm::LLVMContext ctx_;
    llvm::Module mdl_;  
    llvm::IRBuilder<> builder_;
    std::unordered_map<std::string, llvm::Value*> namedValues_;

    llvm::Function* mainFn_ = nullptr;

    std::string outFnName_;
    std::string inFnName_;

public: 
    CodeGen(
        const std::string& moduleName,
        const std::string& outFnName,
        const std::string& inFnName
    );
    
public:
    void createFnDecl(llvm::FunctionType* Ft, const std::string& name);
    void startMainFn(const std::string& name);
    void endMainFn();

    llvm::Value* createIntVal(int val);

    void addDecl(const std::string& varName);
    llvm::Value* addDeclRead(const std::string& varName);
    llvm::Value* addDeclWrite(const std::string& Varname, llvm::Value* rhs);

    llvm::BasicBlock* startIf(llvm::Value* cond);
    void endIf(llvm::BasicBlock* mergeBB);

    using While_t = std::pair<llvm::BasicBlock*, llvm::BasicBlock*>;

    While_t startWhile(llvm::Value* cond);
    void endWhile(llvm::Value* cond, While_t BBs);

    llvm::Value* addOp(node::Operation op, llvm::Value* lhs, llvm::Value* rhs);

    llvm::Type* getInt32Ty();
    llvm::Type* getVoidTy();
    
    void printModule(std::ostream& os);
};

extern CodeGen* gen;

} // namespace codegen


#endif // CODEGEN_HPP