#include "SymbolTable.hpp"
#include <stdexcept>

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