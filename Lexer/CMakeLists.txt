add_library(Lexer STATIC)

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
    PUBLIC
        ./include
)

add_executable(lexer_test test/main.cpp)
target_compile_features(lexer_test
    PUBLIC
        cxx_std_23
)
target_link_libraries(lexer_test PRIVATE Catch2::Catch2 Lexer)
catch_discover_tests(lexer_test)