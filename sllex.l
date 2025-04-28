%option c++

%{  

   #include "slgrammar.tab.hh"

%}

WS     [ \t\n\v]+
DIGIT  [0-9]
PRINT  print

%%

{WS}      /* skip */
{DIGIT}*  return yy::parser::token_type::NUMBER;
";"       return yy::parser::token_type::SCOLON;
{PRINT}   return yy::parser::token_type::PRINT;


%%
