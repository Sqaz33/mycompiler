#ifndef AST_HPP
#define AST_HPP

#include <vector>

#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"

#include "inode.hpp"
#include "codegen.hpp"


namespace ast {

class AST {
    using INode = inode::INode;

    std::vector<std::shared_ptr<INode>> branches_;

public:
    void pushNode(std::shared_ptr<INode> node);
    void translate(codegen::CodeGen& cg);
};

} // namespace ast

#endif // AST_HPP