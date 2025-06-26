%option c++
%option debug

%{  

   #include "slgrammar.tab.hh"

%}

WS     [ \t\n\r\v]+
DIGIT  [0-9]
PRINT  print
VAR    [a-zA-Z_][a-zA-Z0-9_]*

%%

{WS}      /* skip */
{DIGIT}*  return yy::parser::token_type::NUMBER;
{PRINT}   return yy::parser::token_type::PRINT;
{VAR}     return yy::parser::token_type::VAR;
"="       return yy::parser::token_type::EQ;
";"       return yy::parser::token_type::SCOLON;
"+"       return yy::parser::token_type::PLUS;
"-"       return yy::parser::token_type::MINUS;
"*"       return yy::parser::token_type::MULT;
"/"       return yy::parser::token_type::DIV;
"("       return yy::parser::token_type::LBRAC;
")"       return yy::parser::token_type::RBRAC;
.         return yy::parser::token_type::ERR;

%%
