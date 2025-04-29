#ifndef CODEGEN_HPP
#define CODEGEN_HPP

#include <string>
#include <ostream>

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


namespace codegen {

class CodeGen {
    llvm::LLVMContext ctx_;
    llvm::IRBuilder<> builder_;
    llvm::Module mdl_;  

public: 
    CodeGen(const std::string& moduleName) :
        builder_(ctx_)
        , mdl_(moduleName, ctx_)
    {}

public:
    llvm::Value* createBB(const std::string& name);
    llvm::Value* createVarDecl(const std::string& name, llvm::Value* val);
    llvm::Value* createNumber(int x);
    llvm::Value* createPrint(llvm::Value* arg);

    void printModule(std::ostream& os)
}

} // namespace codegen


#endif // CODEGEN_HPP