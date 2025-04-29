#include <fstream>
#include <memory>

#include <FlexLexer.h>

#include "sldriver.hpp" 

int yyFlexLexer::yywrap() { return 1; }

int main(int argc, char** argv) {
    if (argc != 2) return 1;

    std::ifstream ifs(argv[1]);
    if (!ifs.is_open()) return 1;
    auto lexer = std::make_unique<yyFlexLexer>(&ifs);

    yy::SLDriver driver(lexer.get());
    driver.parse();
}