#include "Lexer.hpp"
#include <algorithm>
#include <format>

namespace language
{
    Lexer::Lexer(const std::string &program) : program{program}, pos{program.cbegin()}
    {
    }

    class UnexpectedCharacter
    {
    };

    UnexpectedToken::UnexpectedToken(const std::string& chars, int line, int column):
        std::runtime_error(std::format("Unexpected Token {}... ({},{})", chars, line, column))
    {}

    Token Lexer::next()
    {
        pos = std::find_if_not(pos, program.cend(), [](auto e){ return std::isspace(e);});
        if (pos == program.cend())
            return {TokenType::Eof, "$"};
        auto start = pos;
        try
        {
            switch (auto c = *pos)
            {
            case '+':
                pos++;
                return {TokenType::Plus};
            case '-':
                pos++;
                return {TokenType::Minus};
            case '/':
                pos++;
                return {TokenType::Slash};
            case '*':
                pos++;
                return {TokenType::Star};
            case '(':
                pos++;
                return {TokenType::OpenParenthesis};
            case ')':
                pos++;
                return {TokenType::CloseParenthesis};
            case '{':
                pos++;
                return {TokenType::OpenCurlyBracket};
            case '}':
                pos++;
                return {TokenType::CloseCurlyBracket};
            case '[':
                pos++;
                return {TokenType::OpenSquareBracket};
            case ']':
                pos++;
                return {TokenType::CloseSquareBracket};
            case ',':
                pos++;
                return {TokenType::Comma};
            case ';':
                pos++;
                return {TokenType::Semicolon};
            case '.':
                pos++;
                return {TokenType::Dot};
            case '=':
                pos++;
                if (pos != program.cend() && *pos == '=')
                {
                    pos++;
                    return {TokenType::DoubleEqual};
                }
                return {TokenType::Equal};
            case '!':
                pos++;
                if (pos != program.cend() && *pos == '=')
                {
                    pos++;
                    return {TokenType::ExclamationMarkEqual};
                }
                return {TokenType::ExclamationMark};

            case '&':
                pos++;
                if (pos != program.cend() && *pos == '&')
                {
                    pos++;
                    return {TokenType::ExclamationMarkEqual};
                }
                else
                {
                    pos--;
                    throw UnexpectedCharacter{};
                }
            case '|':
                pos++;
                if (pos != program.cend() && *pos == '|')
                {
                    pos++;
                    return {TokenType::DoublePipe};
                }
                else
                {
                    pos--;
                    throw UnexpectedCharacter{};
                }
            case '<':
                pos++;
                if (pos != program.cend() && *pos == '=')
                {
                    pos++;
                    return {TokenType::LessEqual};
                }
                return {TokenType::Less};

            case '>':
                pos++;
                if (pos != program.cend() && *pos == '=')
                {
                    pos++;
                    return {TokenType::GreaterEqual};
                }
                return {TokenType::Greater};

            default:
                {
                    if (std::isalpha(c) || c == '_')
                    {
                        while (std::isalpha(*pos) || *pos == '_')
                        {
                            pos++;
                        }
                        std::string lexem{start, pos};
                        if (keywords.contains(lexem))
                        {
                            return {keywords[lexem], lexem};
                        }
                        return {TokenType::Id, lexem};
                    }
                    else if (std::isdigit(c))
                    {
                        while(std::isdigit(*pos))
                        {
                            pos++;
                        }
                        if(*pos == '.')
                        {
                            pos++;
                            while(std::isdigit(*pos))
                            {
                                pos++;
                            }
                            return {TokenType::FloatingPointLiteral, std::string{start,pos}};
                        }
                        return {TokenType::IntegerLiteral, std::string{start,pos}};
                    }
                    else
                    {
                        throw UnexpectedCharacter{};
                    }
                }
            }
        }
        catch (UnexpectedCharacter)
        {
            int line = 1;
            auto iter = start;
            while(iter != program.cbegin() && *iter != '\n')
            {
                iter--;
            }
            int column = static_cast<int>(start - iter);
            while(iter != program.cbegin())
            {
                if(*iter == '\n')
                    line++;
                iter--;
            }
            throw UnexpectedToken{std::string{start,std::min(start + 10, program.cend())}, line,column};
        }
        return {TokenType::BooleanLiteral};
    }
}