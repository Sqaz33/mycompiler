#ifndef SYMTAB_HPP
#define SYMTAB_HPP

#include <unordered_map>
#include <string>

#include "inode.hpp"

namespace symtab {

class LocalSymTab {
    std::unordered_map<std::string, node::INode*> symbols_;

public:
    void add(const std::string& name, node::INode* node);
    node::INode* find(const std::string& name);
    void free();
};

class SymTab {
    std::unordered_map<const node::IScope*, LocalSymTab> tables_;

public:
    void add(const node::IScope* scope, 
        const std::string& name, node::INode* node);

    node::INode* find(const node::IScope* scope, const std::string& name);

    void free(const node::IScope* scope);
};

extern SymTab tab;

} // namespace symtab

#endif // SYMTAB_HPP