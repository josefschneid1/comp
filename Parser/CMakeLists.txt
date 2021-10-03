add_library(Parser OBJECT)

target_compile_features(Parser
    PUBLIC
        cxx_std_23
)

target_sources(Parser
    PRIVATE
        src/Parser.cpp
)

target_include_directories(Parser
    PRIVATE
        ./include/Parser
    INTERFACE
        ./include
)

target_link_libraries(Parser Lexer Ast)