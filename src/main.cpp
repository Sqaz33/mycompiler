#include <fstream>
#include <memory>
#include <exception>
#include <iostream>
#include <string.h>

#include <FlexLexer.h>

#include "driver.hpp" 
#include "codegen.hpp"
#include "node.hpp"


codegen::CodeGen* codegen::gen;
node::IScope* node::curScope;

namespace {

auto createGC(const char* moduleName) {
    std::string printFnName("__sl_print");
    std::string inputFnName("__sl_input");
    auto gc = new codegen::CodeGen(
        moduleName, printFnName, inputFnName);
    llvm::Type *Tys[] = {gc->getInt32Ty()};
    llvm::FunctionType* printFnTy =
        llvm::FunctionType::get(gc->getVoidTy(), Tys, /* va args? */ false);
    llvm::FunctionType* inputFnTy =
        llvm::FunctionType::get(gc->getInt32Ty(), /* va args? */ false);   
    gc->createFnDecl(printFnTy, printFnName);
    gc->createFnDecl(inputFnTy, inputFnName);
    gc->startMainFn("__sl_start");
    return gc;
}

} // namespace

int yyFlexLexer::yywrap() { return 1; }

int main(int argc, char** argv) try {
    std::unique_ptr<char*[]> argvOwner(new char*[3]);
    if (argc != 3) {
        argc = 3;
        argv = argvOwner.get();
        argv[0] = strdup("1");
        argv[1] = strdup("../test/fib.sl");
        argv[2] = strdup("main.ll");
    }


    if (argc != 3) return 1;

    std::unique_ptr<codegen::CodeGen> 
        genOwner(createGC(argv[1]));
    codegen::gen = genOwner.get();

    auto globalScope = std::make_unique<node::Scope>();
    node::curScope = globalScope.get();
    std::unique_ptr<codegen::CodeGen> gcOwner(createGC(argv[1]));
    codegen::gen = gcOwner.get();

    std::ifstream ifs(argv[1]);
    if (!ifs.is_open()) return 1;
    auto lexer = std::make_shared<yyFlexLexer>(&ifs);
    
    yy::Driver driver(lexer);
    int res = driver.parse();

    std::ofstream ofs(argv[2]);
    if (!ofs.is_open()) return 1;
    codegen::gen->printModule(ofs);

    return !res;
} catch (const std::exception& e) {
    std::cout << e.what() << '\n';
    return 1;
} catch (...) {
    return 1;
}