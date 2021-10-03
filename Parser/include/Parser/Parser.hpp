#pragma once
#include "Lexer/Lexer.hpp"
#include <string>

namespace language
{
    class Parser
    {
    public:
        Parser(const std::string& program);
    private:
        Lexer lexer;
    };
}