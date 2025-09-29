#include "symtab.hpp"

namespace symtab {

void LocalSymTab::add(const std::string& name, node::INode* node) {
    symbols_[name] = node;
}

node::INode* LocalSymTab::find(const std::string& name) {
    auto it = symbols_.find(name);
    if (it != symbols_.end()) 
        return it->second;
    return nullptr;
}

void LocalSymTab::free() {
    for (auto [_, node] : symbols_) 
        delete node;
    symbols_.clear();
}

void SymTab::add(const node::IScope* scope, 
        const std::string& name, node::INode* node)
{ tables_[scope].add(name, node); }

node::INode* SymTab::find(
    const node::IScope* scope, const std::string& name) 
{
    auto it = tables_.find(scope);
    if (it != tables_.end()) 
        return it->second.find(name);
    return nullptr;
}

void SymTab::free(const node::IScope* scope) {
    if (tables_.contains(scope))
        tables_[scope].free();
}

SymTab tab;

} // namespace symtab