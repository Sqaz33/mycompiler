cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(sl)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
find_package(LLVM REQUIRED CONFIG)

flex_target(scanner
    src/sllex.l
    ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc
)

bison_target(parser
    src/slgrammar.y
    ${CMAKE_CURRENT_BINARY_DIR}/parser.cc
    COMPILE_FLAGS "--defines=${CMAKE_CURRENT_BINARY_DIR}/slgrammar.tab.hh"
)

add_flex_bison_dependency(scanner parser)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs core support)

add_executable(${PROJECT_NAME} 
    src/driver.cpp
    src/codegen.cpp
    ${BISON_parser_OUTPUTS}
    ${FLEX_scanner_OUTPUTS}
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall)

target_link_libraries(${PROJECT_NAME} ${llvm_libs})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

