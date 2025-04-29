/* ------------------------------------------------------------------------- **
 *
 *  INSTR -> OUT; INSTR | VARDECL; INSTR | empty
 *  OUT -> print number | print var
 *  VARDECL -> var = number
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
  PRINT  
  SCOLON 
  EQ     
  ERR
;

%token <int> NUMBER
%token <std::string> VAR

%nterm out
%nterm vardecl

%start program

%%

program: instr
;

instr: out SCOLON instr
     | vardecl SCOLON instr
     | %empty
;

out: PRINT NUMBER { //TODO: AST interaction }
   | PRINT VAR    { //..................... }
;

vardecl: VAR EQ NUMBER {//TODO: AST interaction}

%%

namespace yy {

parser::token_type yylex(parser::semantic_type* yylval,                         
                         SLDriver* driver) 
{
  return driver->yylex(yylval);
}

void parser::error(const std::string&) { std::cout << "err\n"; }

} 