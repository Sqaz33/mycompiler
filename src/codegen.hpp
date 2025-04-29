#ifndef CODEGEN_HPP
#define CODEGEN_HPP

#include <string>
#include <ostream>

#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Value.h>

namespace codegen {

class CodeGen {
    llvm::LLVMContext ctx_;
    llvm::Module mdl_;  
    llvm::IRBuilder<> builder_;
    llvm::FunctionType* printFT;
    llvm::Function* printF;

    std::pair<llvm::FunctionType*, llvm::Function*> declare_putchar_();
    void define_print_();

public: 
    CodeGen(const std::string& moduleName);

public:
    llvm::Value* createNumber(int x);
    llvm::Value* createVarDecl(const std::string& name, llvm::Value* val);
    llvm::Value* createPrint(llvm::Value* arg);

    void finishGen();
    void printModule(std::ostream& os);
};

} // namespace codegen


#endif // CODEGEN_HPP