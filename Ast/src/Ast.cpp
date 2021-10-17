#include "Ast.hpp"
#include <algorithm>

namespace language
{
    void VarDecl::accept(Visitor &v)
    {
        v.visit(*this);
    }

    void Block::accept(Visitor &v)
    {
        v.visit(*this);
    }

    void FuncDef::accept(Visitor &v)
    {
        v.visit(*this);
    }

    void If::accept(Visitor &v)
    {
        v.visit(*this);
    }

    void While::accept(Visitor &v)
    {
        v.visit(*this);
    }

    void Return::accept(Visitor &v)
    {
        v.visit(*this);
    }

    void ExprStmt::accept(Visitor &v)
    {
        v.visit(*this);
    }

    void BinExpr::accept(Visitor &v)
    {
        v.visit(*this);
    }

    void FuncCall::accept(Visitor &v)
    {
        v.visit(*this);
    }

    void VariableAccess::accept(Visitor &v)
    {
        v.visit(*this);
    }

    FuncDef::FuncDef(std::string name, SymbolTable *params, std::unique_ptr<Block> block) : name{std::move(name)}, params{params}, block{std::move(block)}
    {
        struct ReturnVisitor : public Visitor
        {
            std::vector<SymbolTable::Type*> types;

            // TODO Create seperate visitor types for statements and expressions

            // Not interessting
            virtual void visit(VarDecl &){}
            virtual void visit(Program &program){}
            virtual void visit(FuncDef &){}
            virtual void visit(ExprStmt &) {}
            virtual void visit(FuncCall &){}
            virtual void visit(BinExpr &) {}
            virtual void visit(StrLiteral &){}
            virtual void visit(IntLiteral &) {}
            virtual void visit(FloatLiteral &){}
            virtual void visit(BooleanLiteral &){}
            virtual void visit(VariableAccess &) {}

            virtual void visit(Block & b)
            {
                for(auto& s : b.stmts)
                {
                    s->accept(*this);
                }
            }

            virtual void visit(If & ifStmt)
            {
                ifStmt.trueStmt->accept(*this);
                if(ifStmt.falseStmt)
                {
                    ifStmt.falseStmt->accept(*this);
                }
            }

            virtual void visit(While & whileStmt) 
            {
                whileStmt.stmt->accept(*this);
            }

            virtual void visit(Return & returnStmt)
            {   
                //Found a returnStmt
                types.push_back(returnStmt.expr->type);
            }

        } visitor;

        block->accept(visitor);

        auto all_equal = std::equal(visitor.types.begin(), visitor.types.end() - 1, visitor.types.begin() + 1);

        if(!all_equal)
        {
            throw 1;
        }

        this->returnType = visitor.types[0]; 

    }

}