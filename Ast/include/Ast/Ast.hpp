#pragma once
#include <vector>
#include <memory>
#include <string>

// I want to define the Classes in the order they appear in the grammar
// Thats why i need to forward declare those classes.

namespace language
{

    struct Program;
    struct Decl;
    struct VarDecl;
    struct FuncDef;
    struct Variable;
    struct ParamList;
    struct Block;
    struct Stmt;
    struct If;
    struct While;
    struct For;
    struct ExprStmt;
    struct Expr;
    struct BinOperator; // One class for all bin operators. Precedence is encoded in the ast.
    struct FuncCall;
    struct ArgList;

    template <typename T>
    struct Literal;

    struct Program
    {
        std::vector<std::unique_ptr<Decl>> declarations;
    };

    struct Decl
    {
        virtual ~Decl() = default;
    };

    struct VarDecl : public Decl
    {
        std::string name;
        std::unique_ptr<Expr> init;
    };

    struct FuncDef : public Decl
    {
        std::string name;
        std::vector<std::string> params;
        std::unique_ptr<Block> block;       // could be just a Block bc no polymorphism needed here, but Block is still incomplete type
    };

    

}