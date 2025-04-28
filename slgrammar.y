/* ------------------------------------------------------------------------- **
 *
 *  LINE -> EXPR; LINE | empty
 *  EXPR -> print number
 *
 * ------------------------------------------------------------------------- */

%language "c++"


%skeleton "lalr1.cc"
%defines
%define api.value.type variant
%param {yy::SLDriver* driver}

%code requires
{
#include <iostream>
#include <string>

namespace yy { class SLDriver; }
}

%code 
{

#include "sldriver.hpp"

namespace yy {
parser::token_type yylex(parser::semantic_type* yylval,                         
                         SLDriver* driver);
}
    
}

%token 
  PRINT   print
  SCOLON  ";"
  ERR
;

%token <int>  NUMBER
%nterm expr

%start program

%%

program: line
;

line: expr SCOLON line 
    | %empty
;

expr: PRINT NUMBER { std::cout << $2 << '\n'; }
;

%%

namespace yy {

parser::token_type yylex(parser::semantic_type* yylval,                         
                         SLDriver* driver) 
{
  return driver->yylex(yylval);
}

void parser::error(const std::string&) { std::cout << "err\n"; }

} 