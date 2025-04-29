#include <fstream>
#include <memory>
#include <exception>
#include <iostream>

#include <FlexLexer.h>

#include "sldriver.hpp" 
#include "codegen.hpp"

int yyFlexLexer::yywrap() { return 1; }

int main(int argc, char** argv) try {
    if (argc != 3) return 1;

    std::ifstream ifs(argv[1]);
    if (!ifs.is_open()) return 1;
    auto lexer = std::make_unique<yyFlexLexer>(&ifs);

    codegen::CodeGen gc("main");    

    yy::SLDriver driver(lexer.get(), &gc);
    driver.parse();

    std::ofstream ofs(argv[2]);
    if (!ofs.is_open()) return 1;
    gc.printModule(ofs);

} catch (const std::exception& e) {
    std::cout << e.what() << '\n';
    return 1;
} catch (...) {
    return 1;
}