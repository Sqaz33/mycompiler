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
#include <stdexcept>

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

%token<int> NUMBER
%token<std::string> VAR

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

out: PRINT NUMBER       { 
                          auto* val = driver->createNumber($2);
                          driver->createPrint(val);
                        }
   | PRINT VAR          { 
                          if (auto* var = driver->var($2)) {
                            driver->createPrint(var);
                          } else {
                              throw std::invalid_argument(
                                "It is impossible to create a print");
                          }
                        } 
;     

vardecl: VAR EQ NUMBER  { 
                          auto* val = driver->createNumber($3);
                          auto* var = driver->createVarDecl($1, val);
                          if (!driver->addVar($1, var)) 
                            throw std::invalid_argument(
                              "It is impossible to create a variable");
                        }

%%

namespace yy {

parser::token_type yylex(parser::semantic_type* yylval,                         
                         SLDriver* driver) 
{
  return driver->yylex(yylval);
}

void parser::error(const std::string&) { throw std::invalid_argument("Unknown token"); }

} 