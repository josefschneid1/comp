#pragma once
#include "Lexer/Lexer.hpp"
#include "Ast/Ast.hpp"
#include <string>
#include <memory>
#include <concepts>
#include <stdexcept>
#include <format>

namespace language
{
    class UnexpectedToken : public std::runtime_error
    {
    public:
        template <std::same_as<TokenType>... TArgs>
        UnexpectedToken(const Token &token, TArgs... expectedTokenTypes) : std::runtime_error(std::format(
                                                                               "Unexpected Token {} at ({},{}), expected any of " +
                                                                                   ((tokenTypeToStr(expectedTokenTypes) + ", ") + ...),
                                                                               tokenTypeToStr(token.type), token.lexem))
        {
        }
    };

    class Parser
    {
    public:
        Parser(const std::string &program);
        auto program() -> Program;
        auto declaration() -> std::unique_ptr<Decl>;
        auto variableDeclaration() -> std::unique_ptr<VarDecl>;
        auto functionDefinition() -> std::unique_ptr<FuncDef>;
        auto parameterList() -> std::vector<std::string>;
        auto block() -> std::unique_ptr<Block>;
        auto statement() -> std::unique_ptr<Stmt>;
        auto ifStatement() -> std::unique_ptr<If>;
        auto whileStatement() -> std::unique_ptr<While>;
        auto forStatement() -> std::unique_ptr<Stmt>;
        auto expressionStatement() -> std::unique_ptr<ExprStmt>;
        auto assignment() -> std::unique_ptr<Expr>;
        auto logicalOr() -> std::unique_ptr<Expr>;
        auto logicalAnd() -> std::unique_ptr<Expr>;
        auto comparision() -> std::unique_ptr<Expr>;
        auto term() -> std::unique_ptr<Expr>;
        auto product() -> std::unique_ptr<Expr>;
        auto primary() -> std::unique_ptr<Expr>;
        auto argumentList() -> std::vector<std::unique_ptr<Expr>>;

    private:
        auto advance() -> void;
        auto peek() -> const Token &;

        template <typename ... TArgs>
        auto match(TokenType first, TArgs... tokenTypes) -> bool
        {
            if (peek().type == first)
            {
                return true;
            }
            else
            {
                return match(tokenTypes...);
            }
        }

        auto match(TokenType first) -> bool
        {
            return peek().type == first;
        }

        template <std::same_as<TokenType>... TArgs>
        auto consume(TokenType first, TArgs... tokenTypes) -> void
        {
            if (peek().type == first)
            {
                advance();
                consume(tokenTypes...);
            }
            else
            {
                throw UnexpectedToken(peek(), first);
            }
        }

        auto consume(TokenType first) -> void
        {
            if (peek().type == first)
            {
                advance();
            }
            else
            {
                throw UnexpectedToken(peek(), first);
            }
        }

        Lexer lexer;
        Token next;
    };
}
