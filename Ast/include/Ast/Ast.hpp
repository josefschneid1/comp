#pragma once
#include <vector>
#include <memory>
#include <string>

namespace language
{
    struct Visitor;

    struct Visitable
    {
        virtual void accept(Visitor& v) = 0;
        virtual ~Visitable() = default;
    };

    enum class BinOperator
    {
        Plus,Minus,Mul,Div, //Arithmetic
        And,Or,//Logical
        Less,LessEqual,Greater,GreaterEqual,Equal,NotEqual, // Comparison
    };

    struct AstNode : public Visitable
    {
        virtual ~AstNode() = default;
    };

    /*
        A declaration is either a variable declaration or a function defintion.
    */
    struct Decl : public AstNode
    {
        virtual ~Decl() = default;
    };

    /*
        A statement does not produce a value, it has a side effect.
    */
    struct Stmt : public AstNode
    {
        virtual ~Stmt() = default;
    };

    /*
        A expression evaluates to a value.
    */
    struct Expr : public AstNode
    {
        virtual ~Expr() = default;
    };

    /*
        A program is a sequence of declarations. 
    */
    struct Program : public AstNode
    {
        std::vector<std::unique_ptr<Decl>> declarations;
        void accept(Visitor& v) override;
    };

    struct VarDecl : public Decl, public Stmt // A variable declaration can appear at global as well as local scope
    {
        std::string name;
        std::unique_ptr<Expr> init;
        VarDecl(std::string name, std::unique_ptr<Expr> init):
            name{std::move(name)}, init{std::move(init)}
        {}
        void accept(Visitor& v) override;
    };

    struct Block : public Stmt
    {
        std::vector<std::unique_ptr<Stmt>> stmts;
        Block(std::vector<std::unique_ptr<Stmt>> stmts):
            stmts{std::move(stmts)}
        {}
        Block(){}
        void accept(Visitor& v) override;
    };

    struct FuncDef : public Decl
    {
        std::string name;
        std::vector<std::string> params;
        std::unique_ptr<Block> block;
        FuncDef(std::string name, std::vector<std::string> params, std::unique_ptr<Block> block):
            name{std::move(name)}, params{std::move(params)}, block{std::move(block)}
        {}

        void accept(Visitor& v) override;
    };

    struct If : public Stmt
    {
        std::unique_ptr<Expr> expr;
        std::unique_ptr<Stmt> trueStmt;
        std::unique_ptr<Stmt> falseStmt;
        If(std::unique_ptr<Expr> expr, std::unique_ptr<Stmt> trueStmt, std::unique_ptr<Stmt> falseStmt = nullptr):
            expr{std::move(expr)}, trueStmt{std::move(trueStmt)}, falseStmt{std::move(falseStmt)}
        {}
        void accept(Visitor& v) override;
    };

    struct While : public Stmt
    {
        std::unique_ptr<Expr> expr;
        std::unique_ptr<Stmt> stmt;
        While(std::unique_ptr<Expr> expr, std::unique_ptr<Stmt> stmt):
            expr{std::move(expr)}, stmt{std::move(stmt)}
            {}
        void accept(Visitor& v) override;
    };

    struct ExprStmt : public Stmt
    {
        std::unique_ptr<Expr> expr;
        ExprStmt(std::unique_ptr<Expr> expr):
            expr{std::move(expr)}
        {}
        void accept(Visitor& v) override;
    };

    struct BinExpr : public Expr
    {
        std::unique_ptr<Expr> leftHs;
        std::unique_ptr<Expr> rightHs;
        BinOperator op;
        BinExpr(std::unique_ptr<Expr> leftHs, std::unique_ptr<Expr> rightHs, BinOperator op):
            leftHs{std::move(leftHs)}, rightHs{std::move(rightHs)}, op{op}
        {}
        void accept(Visitor& v) override;
    };

    struct FuncCall : public Expr
    {
        std::string name;
        std::vector<std::unique_ptr<Expr>> args;
        FuncCall(std::string name, std::vector<std::unique_ptr<Expr>> args):
            name{std::move(name)}, args{std::move(args)}
            {}
        void accept(Visitor& v) override;
    };

    struct Variable : public Expr
    {
        std::string name;
        Variable(std::string name):
            name{std::move(name)}
            {}
        void accept(Visitor& v) override;
    };

    template <typename T>
    struct Literal : public Expr
    {
        T v;
        Literal(T v):
            v{std::move(v)}
            {}
        void accept(Visitor& v) override;
    };

    using StrLiteral = Literal<std::string>;
    using IntLiteral = Literal<int>;
    using FloatLiteral = Literal<double>;
    using BooleanLiteral = Literal<bool>;

    struct Visitor
    {
        virtual void visit(VarDecl&) = 0;
        virtual void visit(FuncDef&) = 0;
        virtual void visit(Block&) = 0;
        virtual void visit(If&) = 0;
        virtual void visit(While&) = 0;
        virtual void visit(ExprStmt&) = 0;
        virtual void visit(FuncCall&) = 0;
        virtual void visit(BinExpr&) = 0;
        virtual void visit(StrLiteral&) = 0;
        virtual void visit(IntLiteral&) = 0;
        virtual void visit(FloatLiteral&) = 0;
        virtual void visit(BooleanLiteral&) = 0;
        virtual void visit(Variable&) = 0;
        virtual void visit(Program& program) = 0;

        virtual ~Visitor() = default;
    };

    template<typename T>
    void Literal<T>::accept(Visitor& v)
    {
        v.visit(*this);
    }
}