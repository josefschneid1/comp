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

    /*
        A declaration is either a variable declaration or a function defintion.
    */
    struct Decl : public Visitable
    {
        virtual ~Decl() = default;
    };

    /*
        A statement does not produce a value, it has a side effect.
    */
    struct Stmt : public Visitable
    {
        virtual ~Stmt() = default;
    };

    /*
        A expression evaluates to a value.
    */
    struct Expr : public Visitable
    {
        virtual ~Expr() = default;
    };

    /*
        A program is a sequence of declarations. 
    */
    struct Program
    {
        std::vector<std::unique_ptr<Decl>> declarations;
    };

    struct VarDecl : public Decl, public Stmt // A variable declaration can appear at global as well as local scope
    {
        std::string name;
        std::unique_ptr<Expr> init;
        void accept(Visitor& v) override;
    };

    struct Block : public Stmt
    {
        std::vector<std::unique_ptr<Stmt>> stmts;
        void accept(Visitor& v) override;
    };

    struct FuncDef : public Decl
    {
        std::string name;
        std::vector<std::string> params;
        std::unique_ptr<Block> block;
        void accept(Visitor& v) override;
    };

    struct If : public Stmt
    {
        std::unique_ptr<Expr> expr;
        std::unique_ptr<Stmt> trueStmt;
        std::unique_ptr<Stmt> falseStmt;
        void accept(Visitor& v) override;
    };

    struct While : public Stmt
    {
        std::unique_ptr<Expr> expr;
        std::unique_ptr<Stmt> stmt;
        void accept(Visitor& v) override;
    };

    struct ExprStmt : public Stmt
    {
        std::unique_ptr<Expr> expr;
        void accept(Visitor& v) override;
    };

    struct BinExpr : public Expr
    {
        std::unique_ptr<Expr> leftHs;
        std::unique_ptr<Expr> rightHs;
        BinOperator op;
        void accept(Visitor& v) override;
    };

    struct FuncCall : public Expr
    {
        std::string name;
        std::vector<std::unique_ptr<Expr>> args;
        void accept(Visitor& v) override;
    };

    struct Variable : public Expr
    {
        std::string name;
        void accept(Visitor& v) override;
    };

    template <typename T>
    struct Literal : public Expr
    {
        T v;
        void accept(Visitor& v) override;
    };

    using StrLiteral = Literal<std::string>;
    using IntLiteral = Literal<int>;
    using FloatLiteral = Literal<double>;

    struct Visitor
    {
        virtual void visit(VarDecl& decl) = 0;
        virtual void visit(FuncDef& decl) = 0;
        virtual void visit(Block& decl) = 0;
        virtual void visit(If& decl) = 0;
        virtual void visit(While& decl) = 0;
        virtual void visit(ExprStmt& decl) = 0;
        virtual void visit(FuncCall& decl) = 0;
        virtual void visit(BinExpr& decl) = 0;
        virtual void visit(StrLiteral& decl) = 0;
        virtual void visit(IntLiteral& decl) = 0;
        virtual void visit(FloatLiteral& decl) = 0;
        virtual void visit(Variable& decl) = 0;

        virtual ~Visitor() = default;
    };

    template<typename T>
    void Literal<T>::accept(Visitor& v)
    {
            v.visit(*this);
    }
}