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

        Eof,
    };

    class UnexpectedToken : public std::runtime_error
    {
    public:
        UnexpectedToken(const std::string& chars, int line, int column);
    };

    class Token
    {
    public:
        Token(TokenType type, const std::string& lexem = "");
    private:
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
            {"false", TokenType::BooleanLiteral}
        };
    };
}
