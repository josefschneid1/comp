#include "SymbolTable.hpp"
#include <stdexcept>

namespace language
{

    SymbolTable::SymbolTable(SymbolTable *parent) : parent{parent}
    {
    }

    SymbolTable::entry_type &SymbolTable::get(const std::string &key)
    {
        if (table.contains(key))
        {
            return table[key];
        }
        if (parent != nullptr)
        {
            return parent->get(key);
        }
        else
        {
            throw std::runtime_error("Symbol " + key + " not found!");
        }
    }

    SymbolTable::entry_type &SymbolTable::put(const std::string &key, entry_type type)
    {
        if (table.contains(key))
        {
            throw std::runtime_error("Symbol " + key + " already defined!");
        }
        return table[key] = type;
    }

    void SymbolTable::addChild(std::unique_ptr<SymbolTable> child)
    {
        children.push_back(std::move(child));
    }

}
