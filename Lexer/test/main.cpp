#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Lexer/Lexer.hpp"

TEST_CASE("Keywords","[Single Tokens]")
{
    std::string prog = R"(
        while else for if true false var
    )";

    language::Lexer l{prog};
    std::vector<language::Token> tokens;
    for(auto t = l.next(); t.type != language::TokenType::Eof;t = l.next())
    {
        tokens.push_back(t);
    }

    REQUIRE(tokens[0].type == language::TokenType::While);
    REQUIRE(tokens[1].type == language::TokenType::Else);
    REQUIRE(tokens[2].type == language::TokenType::For);
    REQUIRE(tokens[3].type == language::TokenType::If);
    REQUIRE(tokens[4].type == language::TokenType::BooleanLiteral);
    REQUIRE(tokens[4].lexem == "true");
    REQUIRE(tokens[5].type == language::TokenType::BooleanLiteral);
    REQUIRE(tokens[5].lexem == "false");
    REQUIRE(tokens[6].type == language::TokenType::Var);
}