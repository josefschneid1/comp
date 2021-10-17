#include "Lexer.hpp"
#include <algorithm>
#include <format>

namespace language
{
    Lexer::Lexer(const std::string &program) : program{program}, pos{this->program.begin()}
    {
    }

    class UnexpectedCharacter
    {
    };

    Token::Token(TokenType type, const std::string& lexem):
        type{type},lexem{lexem}
        {}

    InvalidToken::InvalidToken(const std::string& chars, int line, int column):
        std::runtime_error(std::format("Invalid Token {}... ({},{})", chars, line, column))
    {}

    Token Lexer::next()
    {
        pos = std::find_if_not(pos, program.end(), [](auto e){ return std::isspace(e);});
        if (pos == program.end())
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
            case ':':
                pos++;
                return {TokenType::Colon};
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
            throw InvalidToken{std::string{start,std::min(start + 10, program.end())}, line,column};
        }
        return {TokenType::BooleanLiteral};
    }

    std::string tokenTypeToStr(TokenType type)
    {
        switch (type)
        {
        case TokenType::Plus:
            return "Plus";
        case TokenType::Minus:
            return "Minus";
        case TokenType::Slash:
            return "Slash";
        case TokenType::Star:
            return "Star";
        case TokenType::OpenParenthesis:
            return "OpenParenthesis";
        case TokenType::CloseParenthesis:
            return "CloseParenthesis";
        case TokenType::OpenCurlyBracket:
            return "OpenCurlyBracket";
        case TokenType::CloseCurlyBracket:
            return "CloseCurlyBracket";
        case TokenType::OpenSquareBracket:
            return "OpenSquareBracket";
        case TokenType::CloseSquareBracket:
            return "CloseSquareBracket";
        case TokenType::Comma:
            return "Comma";
        case TokenType::Semicolon:
            return "Semicolon";
        case TokenType::Dot:
            return "Dot";
        case TokenType::Equal:
            return "Equal";
        case TokenType::ExclamationMark:
            return "ExclamationMark";
        case TokenType::Less:
            return "Less";
        case TokenType::Greater:
            return "Greater";
        case TokenType::LessEqual:
            return "LessEqual";
        case TokenType::GreaterEqual:
            return "GreaterEqual";
        case TokenType::DoubleAmpersand:
            return "DoubleAmpersand";
        case TokenType::DoubleEqual:
            return "DoubleEqual";
        case TokenType::DoublePipe:
            return "DoublePipe";
        case TokenType::ExclamationMarkEqual:
            return "ExclamationMarkEqual";
        case TokenType::If:
            return "If";
        case TokenType::While:
            return "While";
        case TokenType::For:
            return "For";
        case TokenType::Else:
            return "Else";
        case TokenType::Id:
            return "Id";
        case TokenType::IntegerLiteral:
            return "IntegerLiteral";
        case TokenType::FloatingPointLiteral:
            return "FloatingPointLiteral";
        case TokenType::BooleanLiteral:
            return "BooleanLiteral";
        case TokenType::StringLiteral:
            return "StringLiteral";
        case TokenType::Var:
            return "Var";
        case TokenType::Eof:
            return "Eof";
        default:
            throw std::invalid_argument("Missing TokenType");
        }
    }
}