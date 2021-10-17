#pragma once
#include <map>
#include <variant>
#include <vector>
#include <string>
#include <memory>

namespace language
{
    class FuncDef;

    class SymbolTable
    {
    public:

        enum class Scope
        {
            Local, Para, Global
        };

        struct Type
        {
            std::size_t size;
        };

        struct Variable
        {
            std::string name;
            Type* type;
            Scope scope;
            int offset;
            bool active;
            bool alive;
            bool temp;
        };

        struct Function
        {
            FuncDef* def;
        };

        using entry_type = std::variant<Variable, Function, Type>;

        SymbolTable(SymbolTable *parent);

        entry_type &get(const std::string &key);
        entry_type &put(const std::string &key, entry_type type);

        void addChild(std::unique_ptr<SymbolTable> child);
        SymbolTable *parent;
        std::vector<std::unique_ptr<SymbolTable>> children;
    private:
        std::map<std::string, entry_type> table;
        
    };
}
