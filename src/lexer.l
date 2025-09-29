%option c++
%option debug

%{  

   #include "grammar.tab.hh"

%}

WS     [ \t\n\r\v]+
DIGIT  [0-9]
PRINT  print
INPUT  input
WHILE  while
ELSE   else
IF     if
NAME   [a-zA-Z_][a-zA-Z0-9_]*

%%

{WS}         /* skip */
{DIGIT}*     return yy::parser::token_type::NUMBER;
{PRINT}      return yy::parser::token_type::PRINT;
{INPUT}      return yy::parser::token_type::INPUT;
"="          return yy::parser::token_type::ASG;
"+"          return yy::parser::token_type::PLUS;
"-"          return yy::parser::token_type::MINUS;
"*"          return yy::parser::token_type::MUL;
"/"          return yy::parser::token_type::DIV;
"%"          return yy::parser::token_type::MOD;
"&&"         return yy::parser::token_type::AND;
"||"         return yy::parser::token_type::OR;
"!"          return yy::parser::token_type::NOT;
{WHILE}      return yy::parser::token_type::WHILE;
{IF}         return yy::parser::token_type::IF;
{ELSE}       return yy::parser::token_type::ELSE;
">"          return yy::parser::token_type::RELOP;
"<"          return yy::parser::token_type::RELOP;
">="         return yy::parser::token_type::RELOP;
"<="         return yy::parser::token_type::RELOP;
"=="         return yy::parser::token_type::RELOP;
"!="         return yy::parser::token_type::RELOP;
"("          return yy::parser::token_type::LPAR;
")"          return yy::parser::token_type::RPAR;
"{"          return yy::parser::token_type::LCB;
"}"          return yy::parser::token_type::RCB;
";"          return yy::parser::token_type::SCOLON;
{NAME}       return yy::parser::token_type::NAME;

%%
