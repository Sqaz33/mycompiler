#include "codegen.hpp"

#include <llvm/Support/raw_os_ostream.h>
#include <llvm/IR/Function.h>

namespace codegen {

std::pair<llvm::FunctionType*, llvm::Function*> 
CodeGen::declare_putchar_() 
{
    using namespace llvm;
    auto* putcharFT = FunctionType::get(
                Type::getInt32Ty(ctx_), { Type::getInt8Ty(ctx_) }, false);
    auto* putcharF = Function::Create(
                putcharFT, Function::ExternalLinkage, "putchar");
    mdl_.getFunctionList().push_back(putcharF);
    return { putcharFT, putcharF };
}

void CodeGen::define_print_() {
    using namespace llvm;

    auto [pft, pf] = declare_putchar_();
    printFT = FunctionType::get(
        Type::getVoidTy(ctx_), { Type::getInt32Ty(ctx_) }, false);
    printF = Function::Create(
        printFT, Function::ExternalLinkage, "print");
    mdl_.getFunctionList().push_back(printF);

    auto* entryBB = BasicBlock::Create(ctx_, "entry", printF);
    builder_.SetInsertPoint(entryBB);
    auto* trk = builder_.CreateTrunc(printF->getArg(0), builder_.getInt8Ty(), "trk");
    auto* out = builder_.CreateAdd(trk, builder_.getInt8(48));
    builder_.CreateCall(pft, pf, out);
    builder_.CreateCall(pft, pf, builder_.getInt8(10));
    builder_.CreateRetVoid();
}

CodeGen::CodeGen(const std::string& moduleName) :
    mdl_(moduleName, ctx_)    
    , builder_(ctx_)
{
    using namespace llvm;

    mdl_.setDataLayout(
        "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128");
    mdl_.setTargetTriple("x86_64-pc-linux-gnu");

    define_print_();
        
    auto* mainFT = FunctionType::get(Type::getInt32Ty(ctx_), false);
    auto* mainF = Function::Create(mainFT, Function::ExternalLinkage, "main");
    mdl_.getFunctionList().push_back(mainF);
    auto* entryBB = BasicBlock::Create(ctx_, "entry", mainF);
    builder_.SetInsertPoint(entryBB);

}

llvm::Value* CodeGen::createNumber(int x) {
    return builder_.getInt32(x);
}

llvm::Value* 
CodeGen::createVarDecl(const std::string& name, llvm::Value* val) 
{ 
    auto* zero = createNumber(0);
    return builder_.CreateAdd(zero, val, name.c_str());
}

llvm::Value* CodeGen::createPrint(llvm::Value* arg) {
    return builder_.CreateCall(printFT, printF, arg);
}

void CodeGen::finishGen() {
    builder_.CreateRet(builder_.getInt32(0));
}

void CodeGen::printModule(std::ostream& os) {
    llvm::raw_os_ostream out(os);
    mdl_.print(out, nullptr);
}



} // namespace codegen