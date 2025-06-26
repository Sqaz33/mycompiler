#ifndef SLDRIVER_HPP
#define SLDRIVER_HPP

#include <cstring>
#include <string>
#include <unordered_map>
#include <utility>

#include <llvm/IR/Value.h>

#include "codegen.hpp"

#include <FlexLexer.h>
#include "slgrammar.tab.hh"

namespace yy {

class SLDriver {
    FlexLexer* lexer_;
    codegen::CodeGen* gc_;
    std::unordered_map<std::string, llvm::Value*> vars_;
    std::unordered_map<std::string, float> vals_;

public:
    SLDriver(FlexLexer* lexer, codegen::CodeGen* gc) : 
        lexer_(lexer)
        , gc_(gc)
    {}

public:
    parser::token_type yylex(parser::semantic_type* yyval) {
        auto tt = static_cast<parser::token_type>(lexer_->yylex());
        if (tt == yy::parser::token_type::NUMBER)
            yyval->as<int>() = std::stoi(lexer_->YYText());
        else if (tt == yy::parser::token_type::VAR) {
            yyval->emplace<std::string>() = lexer_->YYText();
        }
        return tt;
    }

    bool addVar(const std::string& name, llvm::Value* val) {
        if (!vars_.contains(name)) {
            vars_[name] = val;
            return true;
        }
        return false;
    }

    llvm::Value* var(const std::string& name) {
        if (vars_.contains(name)) return vars_[name];
        return nullptr;
    }

    llvm::Value* createNumber(int x) {
        return gc_->createNumber(x);
    }

    llvm::Value* createVarDecl(const std::string& name, llvm::Value* val) {
        return gc_->createVarDecl(name, val);
    }

    llvm::Value* createPrint(llvm::Value* arg) {
        return gc_->createPrint(arg);
    }

    float varVal(const std::string& name) const {
        if (vars_.contains(name)) return vals_.at(name);
        throw std::invalid_argument(
            "It is impossible to create a compute expression");
    }

    void addExplicitVarVal(const std::string& name, float val) {
        vals_[name] = val;
    }

    bool parse() {
        parser parser(this);
        parser.set_debug_level(1);
        bool res = parser.parse();
        gc_->finishGen();
        return !res;
    }
};

} // namespace yy 

#endif // SLDRIVER_HPP