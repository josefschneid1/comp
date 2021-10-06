#include <string>
#include "Parser/Parser.hpp"
#include <exception>

namespace lang = language;

int main()
{
    std::string source_file = R"(
        = a = 5;
        = main(){

        }
    )";
    lang::Parser p{source_file};
    try{
        auto program = p.program();
    }
    catch(std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}