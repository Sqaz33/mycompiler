#include "driver.hpp"

#include <unordered_map>
#include <string>

#include "node.hpp"

namespace yy {

Driver::Driver(std::shared_ptr<FlexLexer> lexer) :
    lexer_(lexer)
{}

parser::token_type Driver::yylex(parser::semantic_type* yyval) {
    static const std::unordered_map<std::string, node::Operation> lop {
        {"<", node::Operation::Less},
        {"<=", node::Operation::LessEq},
        {">", node::Operation::Greater},
        {">=", node::Operation::GreaterEq},
        {"==", node::Operation::Equal},
        {"!=", node::Operation::NotEqual}
    };

    auto tt = static_cast<parser::token_type>(lexer_->yylex());
    switch (tt) {
        case (yy::parser::token_type::NUMBER):
            yyval->as<int>() 
                = std::stoi(lexer_->YYText());
            break;
        case (yy::parser::token_type::NAME):
            yyval->emplace<std::string>() 
                = lexer_->YYText();
            break;
        case (yy::parser::token_type::RELOP):
            yyval->as<node::Operation>() 
                = lop.at(lexer_->YYText());
            break;
    }
    
    return tt;
}

bool Driver::parse() {
    parser parser(this);
    parser.set_debug_level(1);
    bool res = parser.parse();
    return !res;
}

}

