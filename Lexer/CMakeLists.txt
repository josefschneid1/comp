add_library(Lexer OBJECT)

target_compile_features(Lexer
    PUBLIC
        cxx_std_23
)

target_sources(Lexer
    PRIVATE
        src/Lexer.cpp
)

target_include_directories(Lexer
    PRIVATE
        ./include/Lexer
    INTERFACE
        ./include
)