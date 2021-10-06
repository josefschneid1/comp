add_library(Parser STATIC)

target_compile_features(Parser
    PUBLIC
        cxx_std_23
)

target_sources(Parser
    PRIVATE
        src/Parser.cpp
        src/SymbolTable.cpp
)

target_include_directories(Parser
    PRIVATE
        ./include/Parser
    PUBLIC
        ./include
)

target_link_libraries(Parser Lexer Ast)

add_executable(parser_test test/main.cpp)
target_link_libraries(parser_test PRIVATE Catch2::Catch2 Parser)
catch_discover_tests(parser_test)