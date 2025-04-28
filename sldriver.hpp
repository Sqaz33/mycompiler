#ifndef SLDRIVER_HPP
#define SLDRIVER_HPP

#include <string>

#include <FlexLexer.h>
#include "slgrammar.tab.hh"

namespace yy {

class SLDriver {
    FlexLexer* lexer_;

public:
    SLDriver(FlexLexer* lexer) : 
        lexer_(lexer)
    {}

public:
    parser::token_type yylex(parser::semantic_type* yyval) {
        auto tt = static_cast<parser::token_type>(lexer_->yylex());
        if (tt == yy::parser::token_type::NUMBER)
            yyval->as<int>() = std::stoi(lexer_->YYText());
        return tt;
    }

    bool parse() {
        parser parser(this);
        bool res = parser.parse();
        return !res;
    }
};

} // namespace yy 

#endif // SLDRIVER_HPP