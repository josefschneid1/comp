#pragma once
#include <vector>
#include <memory>
#include <string>
#include "SymbolTable.hpp"

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
    };

    /*
        A statement does not produce a value, it has a side effect.
    */
    struct Stmt : public Visitable
    {
        std::string label;
    };

    /*
        A expression evaluates to a value.
    */
    struct Expr : public Visitable
    {
        SymbolTable::Type* type;
        Expr(SymbolTable::Type* type):
            type{type}
        {}
    };

    /*
        A program is a sequence of declarations. 
    */
    struct Program 
    {
        std::vector<std::unique_ptr<Decl>> declarations;
        std::unique_ptr<SymbolTable> sym_table;

        Program(std::unique_ptr<SymbolTable> sym_table):
            sym_table{std::move(sym_table)}
        {}
    };

    struct VarDecl : public Decl, public Stmt // A variable declaration can appear at global as well as local scope
    {
        SymbolTable::Variable* var;
        std::unique_ptr<Expr> init;
        VarDecl(SymbolTable::Variable* var, std::unique_ptr<Expr> init):
            var{var}, init{std::move(init)}
        {}
        void accept(Visitor& v) override;
    };

    struct Block : public Stmt
    {
        std::vector<std::unique_ptr<Stmt>> stmts;
        SymbolTable* symbols;
        Block(std::vector<std::unique_ptr<Stmt>> stmts, SymbolTable* symbols):
            stmts{std::move(stmts)}, symbols{symbols}
        {}
        Block(){}
        void accept(Visitor& v) override;
    };

    struct FuncDef : public Decl
    {
        std::string name;
        std::unique_ptr<Block> block;
        SymbolTable* params;
        SymbolTable::Type* returnType;
        FuncDef(std::string name, SymbolTable* params, std::unique_ptr<Block> block);
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

    struct Return : public Stmt
    {
        std::unique_ptr<Expr> expr;
        Return(std::unique_ptr<Expr> expr = nullptr):
            expr{std::move(expr)}{}
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
        BinExpr(std::unique_ptr<Expr> leftHs, std::unique_ptr<Expr> rightHs,
         BinOperator op, SymbolTable::Type* type):
            Expr{type},
            leftHs{std::move(leftHs)}, rightHs{std::move(rightHs)}, op{op}
        {}
        void accept(Visitor& v) override;
    };

    struct FuncCall : public Expr
    {
        SymbolTable::Function* function;
        std::vector<std::unique_ptr<Expr>> args;
        FuncCall(SymbolTable::Function* function, std::vector<std::unique_ptr<Expr>> args):
            Expr{function->def->returnType},
            args{std::move(args)}
            {}
        void accept(Visitor& v) override;
    };

    struct VariableAccess : public Expr
    {
        SymbolTable::Variable* var;
        VariableAccess(SymbolTable::Variable* var):
            Expr{var->type},
            var{var}
            {}
        void accept(Visitor& v) override;
    };

    template <typename T>
    struct Literal : public Expr
    {
        T v;
        Literal(T v, SymbolTable::Type* type):
            Expr{type},
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
        virtual void visit(Return&) = 0;
        virtual void visit(ExprStmt&) = 0;
        virtual void visit(FuncCall&) = 0;
        virtual void visit(BinExpr&) = 0;
        virtual void visit(StrLiteral&) = 0;
        virtual void visit(IntLiteral&) = 0;
        virtual void visit(FloatLiteral&) = 0;
        virtual void visit(BooleanLiteral&) = 0;
        virtual void visit(VariableAccess&) = 0;
        virtual void visit(Program& program) = 0;

        virtual ~Visitor() = default;
    };

    template<typename T>
    void Literal<T>::accept(Visitor& v)
    {
        v.visit(*this);
    }
}