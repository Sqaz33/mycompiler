find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

flex_target(scanner
    sllex.l
    ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc
)

bison_target(parser
    slgrammar.y
    ${CMAKE_CURRENT_BINARY_DIR}/parser.cc
    COMPILE_FLAGS "--defines=${CMAKE_CURRENT_BINARY_DIR}/slgrammar.tab.hh"
)

add_flex_bison_dependency(scanner parser)