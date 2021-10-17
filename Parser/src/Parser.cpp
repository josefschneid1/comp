#include "Parser.hpp"


namespace language
{
    Parser::Parser(const std::string &program) : lexer{program},
                                                 next{lexer.next()}, top{nullptr}
    {
    }

    auto Parser::advance() -> void
    {
        next = lexer.next();
    }

    auto Parser::peek() -> const Token &
    {
        return next;
    }

    auto Parser::program() -> Program
    {
        Program prog{std::make_unique<SymbolTable>(top)};
        top = prog.sym_table.get();
        top->put("int", SymbolTable::Type{.size = 4});
        top->put("float", SymbolTable::Type{.size = 4});
        top->put("bool", SymbolTable::Type{.size = 1});

        while (!match(TokenType::Eof))
        {
            prog.declarations.push_back(declaration());
        }
        return prog;
    }

    auto Parser::declaration() -> std::unique_ptr<Decl>
    {
        if (match(TokenType::Var))
        {
            return variableDeclaration();
        }
        else if (match(TokenType::Id))
        {
            return functionDefinition();
        }
        else
        {
            throw UnexpectedToken(peek(), TokenType::Var, TokenType::Id);
        }
    }

    auto Parser::variableDeclaration() -> std::unique_ptr<VarDecl>
    {
        consume(TokenType::Var);
        auto varName = peek().lexem;
        consume(TokenType::Id, TokenType::Equal);
        auto init = assignment();
        consume(TokenType::Semicolon);

        auto& vardecl = top->put(varName, SymbolTable::Variable{
            .name = varName,
            .type = init->type,
            .scope = (top->parent == nullptr ? SymbolTable::Scope::Global : SymbolTable::Scope::Local),
            .offset = -1,
            .active = true,
            .alive = true,
            .temp = false
        });

        return std::make_unique<VarDecl>(&std::get<SymbolTable::Variable>(vardecl), std::move(init));
    }

    auto Parser::functionDefinition() -> std::unique_ptr<FuncDef>
    {
        auto funcName = peek().lexem;
        consume(TokenType::Id);
        parameterList();
        auto def =  std::make_unique<FuncDef>(funcName, top, block());
        top = top->parent;
        top->put(std::move(funcName), SymbolTable::Function{def.get()});
        return std::move(def);
    }

    auto Parser::parameterList() -> void
    {
        top->addChild(std::make_unique<SymbolTable>(top));
        top = top->children[top->children.size()-1].get();
        int offset = 0;
        consume(TokenType::OpenParenthesis);
        while (peek().type != TokenType::CloseParenthesis)
        {
            auto paraName = peek().lexem;
            consume(TokenType::Id, TokenType::Colon);
            auto typeName = peek().lexem;
            consume(TokenType::Id);

            auto& type = top->get(typeName);
            if(!std::holds_alternative<SymbolTable::Type>(type))
            {
                throw 1;
            }

            top->put(paraName, SymbolTable::Variable{
                .name = paraName,
                .type = &std::get<SymbolTable::Type>(type),
                .scope = SymbolTable::Scope::Para,
                .offset = offset,
                .active = true,
                .alive = true, // ????
                .temp = false
            });

            offset += std::get<SymbolTable::Type>(type).size;

            if (peek().type == TokenType::Comma)
                advance();
        }
        consume(TokenType::CloseParenthesis);
    }

    auto Parser::block() -> std::unique_ptr<Block>
    {
        top->addChild(std::make_unique<SymbolTable>(top));
        top = top->children[top->children.size()-1].get();
        consume(TokenType::OpenCurlyBracket);
        std::vector<std::unique_ptr<Stmt>> stmts;
        while (peek().type != TokenType::CloseCurlyBracket)
        {
            stmts.push_back(statement());
        }
        consume(TokenType::CloseCurlyBracket);
        auto blck = std::make_unique<Block>(std::move(stmts), top);
        top = top->parent;
        return std::move(blck);
    }

    auto Parser::statement() -> std::unique_ptr<Stmt>
    {
        if (match(TokenType::If))
        {
            return ifStatement();
        }
        else if (match(TokenType::While))
        {
            return whileStatement();
        }
        else if (match(TokenType::For))
        {
            return forStatement();
        }
        else if (match(TokenType::Var))
        {
            return variableDeclaration();
        }
        else if (match(TokenType::Return))
        {
            return returnStatement();
        }
        else if (match(TokenType::IntegerLiteral,
                       TokenType::FloatingPointLiteral,
                       TokenType::StringLiteral,
                       TokenType::BooleanLiteral,
                       TokenType::OpenParenthesis))
        {
            return expressionStatement();
        }
        else
        {
            throw UnexpectedToken(peek(),
                                  TokenType::If,
                                  TokenType::While,
                                  TokenType::For,
                                  TokenType::Var,
                                  TokenType::OpenParenthesis,
                                  TokenType::IntegerLiteral,
                                  TokenType::FloatingPointLiteral,
                                  TokenType::StringLiteral,
                                  TokenType::BooleanLiteral);
        }
    }

    auto Parser::ifStatement() -> std::unique_ptr<If>
    {
        consume(TokenType::If, TokenType::OpenParenthesis);
        auto expr = assignment();
        consume(TokenType::CloseParenthesis);
        auto stmt = statement();
        if (match(TokenType::Else))
        {
            advance();
            return std::make_unique<If>(std::move(expr), std::move(stmt), statement());
        }
        return std::make_unique<If>(std::move(expr), std::move(stmt));
    }

    auto Parser::whileStatement() -> std::unique_ptr<While>
    {
        consume(TokenType::While, TokenType::OpenParenthesis);
        auto expr = assignment();
        consume(TokenType::CloseParenthesis);
        auto stmt = statement();
        return std::make_unique<While>(std::move(expr), std::move(stmt));
    }

    auto Parser::forStatement() -> std::unique_ptr<Stmt>
    {
        consume(TokenType::For, TokenType::OpenParenthesis);
        auto init = statement();
        auto exprStmt = expressionStatement();
        auto post = assignment();

        auto blck = std::make_unique<Block>();
        blck->stmts.push_back(std::move(init));

        consume(TokenType::CloseParenthesis);

        auto stmt = statement();

        auto loopBody = std::make_unique<Block>();
        loopBody->stmts.push_back(std::move(stmt));
        loopBody->stmts.push_back(std::make_unique<ExprStmt>(std::move(post)));

        auto whileStmt = std::make_unique<While>(
            std::move(exprStmt->expr),
            std::move(loopBody));

        blck->stmts.push_back(std::move(whileStmt));

        return std::move(blck);
    }

    auto Parser::returnStatement() -> std::unique_ptr<Return>
    {
        consume(TokenType::Return);
        auto ret =  std::make_unique<Return>(assignment());
        consume(TokenType::Semicolon);
        return std::move(ret);
    }

    auto Parser::expressionStatement() -> std::unique_ptr<ExprStmt>
    {
        auto expr = assignment();
        consume(TokenType::Semicolon);
        return std::make_unique<ExprStmt>(std::move(expr));
    }

    auto Parser::assignment() -> std::unique_ptr<Expr>
    {
        auto lo = logicalOr();
        if (match(TokenType::Equal))
        {
            advance();
            return std::make_unique<BinExpr>(std::move(lo), assignment(), BinOperator::Equal, lo->type);     //TODO Type Check/Conversion
        }
        return lo;
    }

    auto Parser::logicalOr() -> std::unique_ptr<Expr>
    {
        auto la = logicalAnd();
        while (match(TokenType::DoublePipe))
        {
            advance();
            la = std::make_unique<BinExpr>(std::move(la), logicalAnd(), BinOperator::Or, la->type);
        }
        return la;
    }

    auto Parser::logicalAnd() -> std::unique_ptr<Expr>
    {
        auto comp = comparision();
        while (match(TokenType::DoubleAmpersand))
        {
            advance();
            comp = std::make_unique<BinExpr>(std::move(comp), comparision(), BinOperator::And, comp->type);
        }
        return comp;
    }

    auto Parser::comparision() -> std::unique_ptr<Expr>
    {
        auto ter = term();
        while (match(TokenType::Less,
                     TokenType::LessEqual,
                     TokenType::Greater,
                     TokenType::GreaterEqual,
                     TokenType::DoubleEqual,
                     TokenType::ExclamationMarkEqual))
        {
#pragma warning(push)
#pragma warning(disable : 4715)
            auto op = [this]() -> BinOperator
            {
                switch (this->peek().type)
                {
                case TokenType::Less:
                    return BinOperator::Less;
                case TokenType::LessEqual:
                    return BinOperator::LessEqual;
                case TokenType::Greater:
                    return BinOperator::Greater;
                case TokenType::GreaterEqual:
                    return BinOperator::GreaterEqual;
                case TokenType::Equal:
                    return BinOperator::Equal;
                case TokenType::ExclamationMarkEqual:
                    return BinOperator::NotEqual;
                }
            }();
#pragma warning(pop)

            advance();
            ter = std::make_unique<BinExpr>(std::move(ter), term(), op, ter->type);
        }
        return ter;
    }
    auto Parser::term() -> std::unique_ptr<Expr>
    {
        auto p = product();
        while (match(TokenType::Plus, TokenType::Minus))
        {
            auto op = peek().type == TokenType::Plus ? BinOperator::Plus : BinOperator::Minus;
            advance();
            p = std::make_unique<BinExpr>(std::move(p), product(), op, p->type);
        }
        return p;
    }

    auto Parser::product() -> std::unique_ptr<Expr>
    {
        auto p = primary();
        while (match(TokenType::Star, TokenType::Slash))
        {
            auto op = peek().type == TokenType::Star ? BinOperator::Mul : BinOperator::Div;
            advance();
            p = std::make_unique<BinExpr>(std::move(p), primary(), op, p->type);
        }
        return p;
    }

    auto Parser::primary() -> std::unique_ptr<Expr>
    {
        if (match(TokenType::IntegerLiteral))
        {
            auto lit = peek().lexem;
            advance();
            return std::make_unique<IntLiteral>(std::stoi(lit), &std::get<SymbolTable::Type>(top->get("int")));
        }
        else if (match(TokenType::FloatingPointLiteral))
        {
            auto lit = peek().lexem;
            advance();
            return std::make_unique<FloatLiteral>(std::stod(lit), &std::get<SymbolTable::Type>(top->get("float")));
        }
        else if (match(TokenType::BooleanLiteral))
        {
            auto lit = peek().lexem;
            advance();
            return std::make_unique<BooleanLiteral>(lit == "true" ? true : false, &std::get<SymbolTable::Type>(top->get("bool")));
        }
        else if (match(TokenType::StringLiteral))
        {
            auto lit = peek().lexem;
            advance();
            return std::make_unique<StrLiteral>(lit, nullptr); // TODO
        }
        else if (match(TokenType::Id))
        {
            auto name = peek().lexem;
            advance();
            if (match(TokenType::OpenParenthesis))
            {
                auto args = argumentList();
                auto& function = top->get(name);
                if(!std::holds_alternative<SymbolTable::Function>(function))
                {
                    throw 1;
                }
                return std::make_unique<FuncCall>(&std::get<SymbolTable::Function>(function), std::move(args));
            }
            auto& variable = top->get(name);

            if(!std::holds_alternative<SymbolTable::Variable>(variable))
            {
                throw 1;
            }

            return std::make_unique<VariableAccess>(&std::get<SymbolTable::Variable>(variable));
        }
        else
        {
            throw UnexpectedToken(peek(),
                                  TokenType::IntegerLiteral,
                                  TokenType::FloatingPointLiteral,
                                  TokenType::BooleanLiteral,
                                  TokenType::StringLiteral,
                                  TokenType::Id);
        }
    }

    auto Parser::argumentList() -> std::vector<std::unique_ptr<Expr>>
    {
        consume(TokenType::OpenParenthesis);
        std::vector<std::unique_ptr<Expr>> args;
        while (!match(TokenType::CloseParenthesis))
        {
            args.push_back(assignment());
            if (match(TokenType::Semicolon))
            {
                advance();
            }
        }
        consume(TokenType::CloseParenthesis);
        return args;
    }

}
