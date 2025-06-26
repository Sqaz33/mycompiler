/* ------------------------------------------------------------------------- **
 *
 *  INSTR -> OUT; INSTR | VARDECL; INSTR | empty
 *  OUT -> print number | print var
 *  VARDECL -> var = EXPR 
 *  EXPR -> TERM - EXPR | TERM + EXPR | TERM
 *  TERM -> MULT * TERM | MULT / TERM | MULT
 *  MULT -> number | (EXPR) | var
 *
 * ------------------------------------------------------------------------- */

%language "c++"


%skeleton "lalr1.cc"
%defines
%define api.value.type variant
%define parse.trace  
%param {yy::SLDriver* driver}



%code requires
{
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

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
  PLUS
  MINUS
  MULT
  DIV
  LBRAC
  RBRAC
  ERR
;

%token <int> NUMBER
%token <std::string> VAR

%nterm out
%nterm vardecl
%nterm <float> expr
%nterm <float> term
%nterm <float> mult

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

vardecl: VAR EQ expr {
                          auto* val = driver->createNumber($3);
                          auto* var = driver->createVarDecl($1, val);
                          driver->addExplicitVarVal($1, $3); 
                          if (!driver->addVar($1, var)) 
                            throw std::invalid_argument(
                              "It is impossible to create a variable");
                     }
;

expr: expr MINUS term { $$ = $1 - $3; std::cout << $$ << "------expr-----\n"; }
    | expr PLUS term  { $$ = $1 + $3; std::cout << $$ << "------expr-----\n"; }
    | term            { $$ = $1;  std::cout << $$ << "------expr-----\n"; }
;

term: term MULT mult { $$ = $1 * $3; std::cout << $$ << "-------term----\n"; }
    | term DIV mult  { $$ = $1 / $3; std::cout << $$ << "-------term----\n"; }
    | mult           { $$ = $1;  std::cout << $1 << "-------term----\n"; }
;

mult: NUMBER            { $$ = $1; }
    | LBRAC expr RBRAC  { $$ = $2; std::cout << $2 << "-------mult----\n"; }
    | VAR               {
                            if (driver->var($1)) {
                              $$ = driver->varVal($1);
                            } else {
                              throw std::invalid_argument(
                                "It is impossible to create a compute expression");
                            }
                        }
;

%%

namespace yy {

parser::token_type yylex(parser::semantic_type* yylval,                         
                         SLDriver* driver) 
{
  return driver->yylex(yylval);
}

void parser::error(const std::string&) { throw std::invalid_argument("Unknown token"); }

} 