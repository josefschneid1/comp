add_library(Ast STATIC)

target_compile_features(Ast
    PUBLIC
        cxx_std_23
)

target_sources(Ast
    PRIVATE
        src/Ast.cpp
)

target_include_directories(Ast
    PRIVATE
        ./include/Ast
    PUBLIC
        ./include
)