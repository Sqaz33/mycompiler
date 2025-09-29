/* ------------------------------------------------------------------------- **
 *  deprecated:
 * ///////////////////////////////////////////////////////////////////////
 *  INSTR -> OUT; INSTR | VARDECL; INSTR | empty
 *  OUT -> print number | print var
 *  VARDECL -> var = EXPR 
 *  EXPR -> TERM - EXPR | TERM + EXPR | TERM
 *  TERM -> MULT * TERM | MULT / TERM | MULT
 *  MULT -> number | (EXPR) | var
 * ///////////////////////////////////////////////////////////////////////
 * ------------------------------------------------------------------------- */

%language "c++"


%skeleton "lalr1.cc"
%defines
%define api.value.type variant
%define parse.trace  
%param {yy::Driver* driver}

%code requires
{
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

namespace yy { class Driver; }
namespace node { 
  class IScope;  
  class INode;
  enum class Operation;
}

}

%code 
{
#include "inode.hpp"
#include "driver.hpp"

namespace yy {

parser::token_type yylex(parser::semantic_type* yylval, 
                         Driver* driver);
}
    
}

%defines

%token <std::string> NAME
%token <int> NUMBER
%token WHILE IF ELSE
%token SCOLON LBR RBR LPAR RPAR LCB RCB ILLEGAL
%token PRINT
%right ASG
%nonassoc <node::Operation> RELOP
%left INPUT
%left OR
%left AND
%left NOT
%left PLUS MINUS
%left MUL DIV MOD
%nonassoc UMINUS

%nterm <node::INode*> stm
%nterm <node::INode*> scope
%nterm <node::INode*> close_sc
%nterm <node::INode*> oper
%nterm <node::INode*> assign
%nterm <node::INode*> expr
%nterm <node::INode*> output
%nterm <node::INode*> if
%nterm <node::INode*> while
%nterm <node::INode*> while_head
%nterm <node::INode*> if_head
%nterm <node::INode*> lval
%nterm <node::INode*> lexpr
%nterm <node::INode*> else
%nterm <node::INode*> if_else


%start program

%%

program:          stms                        { node::curScope->codegen(); }


scope:            open_sc stms close_sc       { $$ = $3; }
      
open_sc:          LCB                         { node::curScope = node::curScope->push(); }
      
close_sc:         RCB                         { 
                                                $$ = node::curScope;
                                                node::curScope = node::curScope->reset();
                                              }
      
stms:             stm                         { node::curScope->addBranch($1); }
                | stms stm                    { node::curScope->addBranch($2); }
                | stms scope                  { node::curScope->addBranch($2); }

stm:              oper                        { $$ = $1; }

oper:             if_else                     {
                                                $$ = $1;
                                                node::curScope = node::curScope->reset();
                                              }
                | while                       {
                                                $$ = $1;
                                                node::curScope = node::curScope->reset();
                                              }
                | if                          {
                                                $$ = $1;
                                                node::curScope = node::curScope->reset();
                                              }
                | assign                      { $$ = $1; }
                | output                      { $$ = $1; }

assign:           lval ASG expr SCOLON        { $$ = makeOp($1, node::Operation::Assign, $3); }

lval:             NAME                        { $$ = node::curScope->access($1); }
expr:             expr PLUS expr              { $$ = makeOp($1, node::Operation::Plus, $3); }
                | expr MINUS expr             { $$ = makeOp($1, node::Operation::Minus, $3); }
                | expr MUL expr               { $$ = makeOp($1, node::Operation::Mul, $3); }
                | expr DIV expr               { $$ = makeOp($1, node::Operation::Div, $3); }
                | expr MOD expr               { $$ = makeOp($1, node::Operation::Mod, $3); }
                | MINUS expr %prec UMINUS     { $$ = makeOp(node::makeVal(0), node::Operation::Minus, $2); }
                | NAME                        {
                                                $$ = node::curScope->visible($1);
                                                if (!$$) 
                                                  throw std::logic_error("Using undeclared variable");
                                              }
                | NUMBER                      { $$ = node::makeVal($1); }
                | INPUT                       { $$ = makeOp(nullptr, node::Operation::In, nullptr); }
                | LPAR expr RPAR              { $$ = $2; }


if:               if_head scope               { 
                                                node::curScope->addBranch($2); 
                                                $$ = makeIf($1, node::curScope);
                                              }
                | if_head stm                 {
                                                node::curScope->addBranch($2); 
                                                $$ = makeIf($1, node::curScope);
                                              }

if_head:          IF LPAR lexpr RPAR          {
                                                node::curScope = node::curScope->push();
                                                $$ = $3;
                                              }

else:             ELSE stm                    { 
                                                $$ = node::curScope;
                                                node::curScope = node::curScope->reset();
                                                node::curScope = node::curScope->push();
                                                node::curScope->addBranch($2);
                                              }
                | ELSE scope                  { 
                                                $$ = node::curScope;
                                                node::curScope = node::curScope->reset();
                                                node::curScope = node::curScope->push();
                                                node::curScope->addBranch($2);
                                              }


if_else:          if_head scope else          { 
                                                static_cast<node::IScope*>($3)->addBranch($2);
                                                $$ = node::makeIfElse($1, $3, node::curScope);
                                              }
                | if_head stm else            {
                                                static_cast<node::IScope*>($3)->addBranch($2);
                                                $$ = node::makeIfElse($1, $3, node::curScope);
                                              }

lexpr:            expr                        { $$ = $1; }
                | lexpr RELOP lexpr           { $$ = makeOp($1, $2, $3); }
                | lexpr AND lexpr             { $$ = makeOp($1, node::Operation::And, $3); }
                | lexpr OR lexpr              { $$ = makeOp($1, node::Operation::Or, $3); }
                | NOT lexpr                   { $$ = makeOp(nullptr, node::Operation::Not, $2); }
                | LPAR lexpr RPAR             { $$ = $2; }

while:            while_head scope            { 
                                                node::curScope->addBranch($2); 
                                                $$ = makeWhile($1, node::curScope);
                                              }
                | while_head stm              {
                                                node::curScope->addBranch($2); 
                                                $$ = makeWhile($1, node::curScope);
                                              }

while_head:       WHILE LPAR lexpr RPAR       {
                                                node::curScope = node::curScope->push();
                                                $$ = $3;
                                              }
output:           PRINT expr SCOLON           { $$ = makeOp(nullptr, node::Operation::Out, $2); }

%%

namespace yy {


parser::token_type yylex(parser::semantic_type* yylval, 
                         Driver* driver)
{
  return driver->yylex(yylval);
}

void parser::error(const std::string&) { throw std::invalid_argument("Unknown token"); }

} 