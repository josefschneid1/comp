#pragma once
#include <string>
#include <stdexcept>
#include <unordered_map>

namespace language
{
    enum class TokenType
    {
        //Single Character Tokens
        Plus,
        Minus,
        Slash,
        Star,
        OpenParenthesis,    // (
        CloseParenthesis,
        OpenCurlyBracket,   // {
        CloseCurlyBracket,
        OpenSquareBracket,  // [
        CloseSquareBracket,
        Comma,
        Semicolon,
        Dot,
        Equal,
        ExclamationMark,
        Less,
        Greater,

        //Two Character Tokens
        DoubleAmpersand,    // &&
        DoublePipe,         // ||
        DoubleEqual,
        ExclamationMarkEqual,
        LessEqual,
        GreaterEqual,

        If,

        //Multiple Character Tokens
        While,
        Else,
        For,
        Id,
        IntegerLiteral,
        FloatingPointLiteral,
        BooleanLiteral, // false, true
        StringLiteral,
        Var,
        Eof,
    };

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


    class InvalidToken : public std::runtime_error
    {
    public:
        InvalidToken(const std::string& chars, int line, int column);
    };

    struct Token
    {
        Token(TokenType type, const std::string& lexem = "");
        TokenType type;
        std::string lexem;
    };

    class Lexer
    {
    public:
        Lexer(const std::string& program);
        Token next();
    private:
        std::string program;
        std::string::const_iterator pos;
        std::unordered_map<std::string_view, TokenType> keywords{
            {"while", TokenType::While},
            {"else", TokenType::Else},
            {"for", TokenType::For},
            {"if", TokenType::If},
            {"true", TokenType::BooleanLiteral},
            {"false", TokenType::BooleanLiteral},
            {"var", TokenType::Var},
        };
    };
}
