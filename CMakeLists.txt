cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(sl)

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

add_executable(${PROJECT_NAME} 
    driver.cpp
    ${BISON_parser_OUTPUTS}
    ${FLEX_scanner_OUTPUTS}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

