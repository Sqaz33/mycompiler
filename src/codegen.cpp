#include "codegen.hpp"

#include <llvm/Support/raw_os_ostream.h>

namespace codegen {

CodeGen::CodeGen(
    const std::string& moduleName,
    const std::string& outFnName,
    const std::string& inFnName
) :
    mdl_(moduleName, ctx_)
    , builder_(ctx_)
    , outFnName_(outFnName)
    , inFnName_(inFnName)
{
    mdl_.setDataLayout(
        "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128");
    mdl_.setTargetTriple("x86_64-pc-linux-gnu");
}

void CodeGen::createFnDecl(llvm::FunctionType* Ft, const std::string& name) {
    llvm::Function::Create(Ft, 
        llvm::GlobalValue::LinkageTypes::ExternalLinkage, name, mdl_);
}

void CodeGen::startMainFn(const std::string& name) {
    llvm::FunctionType* ft = llvm::FunctionType::get(
        llvm::Type::getVoidTy(ctx_), false);

    mainFn_ = llvm::Function::Create(ft, 
        llvm::GlobalValue::LinkageTypes::ExternalLinkage, name, mdl_);

    auto* entryBB = llvm::BasicBlock::Create(
        ctx_, "entry", mainFn_);

    builder_.SetInsertPoint(entryBB);
}

void CodeGen::endMainFn() {
    builder_.CreateRetVoid();
}

llvm::Value* CodeGen::createIntVal(int val) {
    auto* ty = getInt32Ty();
    return llvm::ConstantInt::get(ty, val);
}

void CodeGen::addDecl(const std::string& varName) {
    auto ty = llvm::Type::getInt32Ty(ctx_);
    auto&& bb = mainFn_->getEntryBlock();
    llvm::IRBuilder<> tmpb(&bb, bb.begin());
    auto* alc = tmpb.CreateAlloca(ty, 0, varName);
    namedValues_[varName] = alc;
}

llvm::Value* CodeGen::addDeclRead(const std::string& varName) {
    auto* alc = namedValues_[varName];
    auto* alcTy = static_cast<llvm::AllocaInst*>
                    (alc)->getAllocatedType();   
    return builder_.CreateLoad(alcTy, alc, varName);
}

llvm::Value* CodeGen::addDeclWrite(
    const std::string& varName, llvm::Value* rhs) 
{
    auto* alc = namedValues_[varName];
    return builder_.CreateStore(rhs, alc);
}

llvm::BasicBlock* CodeGen::startIf(llvm::Value* cond) {
    using namespace llvm;
    auto* thenBB = BasicBlock::Create(ctx_, "then", mainFn_);
    auto* mergeBB = BasicBlock::Create(ctx_, "endIf", mainFn_);
    builder_.CreateCondBr(cond, thenBB, mergeBB);
    builder_.SetInsertPoint(thenBB);
    return mergeBB;
}

void CodeGen::endIf(llvm::BasicBlock* mergeBB) {
    builder_.CreateBr(mergeBB);
    builder_.SetInsertPoint(mergeBB);   
}

CodeGen::While_t CodeGen::startWhile(llvm::Value* cond) {
    using namespace llvm;
    auto* thenBB = BasicBlock::Create(ctx_, "body", mainFn_);
    auto* mergeBB = BasicBlock::Create(ctx_, "endWhile", mainFn_);
    builder_.CreateCondBr(cond, thenBB, mergeBB);
    builder_.SetInsertPoint(thenBB);
    return {thenBB, mergeBB};
}

void CodeGen::endWhile(llvm::Value* cond, While_t BBs) {
    builder_.CreateCondBr(cond, BBs.first, BBs.second);
    builder_.SetInsertPoint(BBs.second);   
}

llvm::Value* CodeGen::addOp(
    node::Operation op, llvm::Value* lhs, llvm::Value* rhs)
{   
    using namespace node;
    switch (op) {
        case Operation::Plus:
            if (nullptr == lhs) {
                lhs = llvm::ConstantInt::get(getInt32Ty(), 0);
            }
            return builder_.CreateAdd(lhs, rhs);
        case Operation::Minus:
            if (nullptr == lhs) {
                lhs = llvm::ConstantInt::get(getInt32Ty(), 0);
            }
            return builder_.CreateSub(lhs, rhs);
        case Operation::Greater:
            return builder_.CreateICmpSGT(lhs, rhs);
        case Operation::Less:
            return builder_.CreateICmpSLT(lhs, rhs);
        case Operation::GreaterEq:
            return builder_.CreateICmpSGE(lhs, rhs);
        case Operation::LessEq:
            return builder_.CreateICmpSLE(lhs, rhs);
        case Operation::Out: {
            auto* callee = mdl_.getFunction(outFnName_.c_str());
            llvm::Value* argsV = {rhs};
            return builder_.CreateCall(callee, argsV);
        }
        case Operation::In: {
            auto* callee = mdl_.getFunction(inFnName_.c_str());
            return builder_.CreateCall(callee);
        }
        case Operation::Equal:
            return builder_.CreateICmpEQ(lhs, rhs);
        case Operation::NotEqual:
            return builder_.CreateICmpNE(lhs, rhs);
        case Operation::Not:
            return builder_.CreateNot(rhs);
        case Operation::And:
            return builder_.CreateAnd(lhs, rhs);
        case Operation::Or:
            return builder_.CreateOr(lhs, rhs);
        case Operation::Div:
            return builder_.CreateSDiv(lhs, rhs);
        case Operation::Mul:
            return builder_.CreateMul(lhs, rhs);
        case Operation::Mod:
            return builder_.CreateSRem(lhs, rhs);
        default: 
            break;
    }

  throw std::runtime_error("Node::Operation not implemented");

}

llvm::Type* CodeGen::getInt32Ty() {
    return llvm::Type::getInt32Ty(ctx_);
}

llvm::Type* CodeGen::getVoidTy() {
    return llvm::Type::getVoidTy(ctx_);
}

void CodeGen::printModule(std::ostream& os) {
    llvm::raw_os_ostream out(os);
    endMainFn();
    mdl_.print(out, nullptr);
}

} // namespace codegen