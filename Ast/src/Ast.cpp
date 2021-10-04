#include "Ast.hpp"

namespace language
{
    void VarDecl::accept(Visitor& v)
    {
        v.visit(*this);
    }

    void Block::accept(Visitor& v)
    {
        v.visit(*this);
    }

    void FuncDef::accept(Visitor& v)
    {
        v.visit(*this);
    }

    void If::accept(Visitor& v)
    {
        v.visit(*this);
    }

    void While::accept(Visitor& v)
    {
        v.visit(*this);
    }

    void ExprStmt::accept(Visitor& v)
    {
        v.visit(*this);
    }

    void BinExpr::accept(Visitor& v)
    {
        v.visit(*this);
    }

    void FuncCall::accept(Visitor& v)
    {
        v.visit(*this);
    }

    void Variable::accept(Visitor& v)
    {
        v.visit(*this);
    }
}