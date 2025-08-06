#ifndef SLDRIVER_HPP
#define SLDRIVER_HPP

#include <string>
#include <memory>

#include "codegen.hpp"
#include "symtab.hpp"
#include "inode.hpp"

#include <FlexLexer.h>
#include "grammar.tab.hh"

namespace yy {

class Driver {
    std::shared_ptr<FlexLexer> lexer_; 
    
public:
    Driver(std::shared_ptr<FlexLexer> lexer);
    
public:
    parser::token_type yylex(parser::semantic_type* yyval);
    bool parse();
};

} // namespace yy 

#endif // SLDRIVER_HPP