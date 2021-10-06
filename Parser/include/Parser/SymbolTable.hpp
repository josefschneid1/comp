#include <unordered_map>
#include <variant>
#include <vector>

class SymbolTable
{
public:
    enum class primitiveType
    {
        Int,
        Float,
        Str,
        Bool
    };

    struct Variable
    {
        std::string name;
        primitiveType type;
        std::size_t size;
        std::size_t offset;
    };

    struct Function
    {
        std::string name;
        std::vector<std::string> parameters;
        std::size_t offset;
    };

    using entry_type = std::variant<Variable, Function>;
    SymbolTable(SymbolTable *parent);
    entry_type &get(const std::string &key);
    entry_type &put(const std::string &key, entry_type type);

private:
    std::unordered_map<std::string, entry_type> table;
    SymbolTable *parent;
};