#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Parser/Parser.hpp"
#include <string>

TEST_CASE("Global Variables", "[variable]")
{
    std::string source = R"(
        var a = 5;
        var b = true;
    )";

    language::Parser p{source};

    auto program = p.program();

    auto &varA = dynamic_cast<language::VarDecl &>(*program.declarations[0]);
    auto &varB = dynamic_cast<language::VarDecl &>(*program.declarations[1]);

    REQUIRE(varA.var->name == "a");
    REQUIRE(dynamic_cast<language::IntLiteral &>(*varA.init).v == 5);
    REQUIRE(varB.var->name == "b");
    REQUIRE(dynamic_cast<language::BooleanLiteral &>(*varB.init).v == true);
}