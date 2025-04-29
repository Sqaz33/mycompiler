#ifndef INODE_HPP
#define INODE_HPP

#include "codegen.hpp"

#include <memory>
#include <optional>
#include <utility>

namespace inode {

class INode {
protected:
    std::optional<std::string> hasName_;

public:
    std::pair<bool, std::string> hasName() const {
        if (hasName_) return { true, *hasName_ };
        return { false, "" };
    }

    virtual llvm::Value* codegen(codegen::CodeGen& cg) = 0;
    virtual ~INode() { }
};

std::shared_ptr<INode> makeNumber(int x);
std::shared_ptr<INode> makeVarDecl(const std::string& name, INode* val);
std::shared_ptr<INode> makePrint(INode* val);

} // namespace inode

#endif // INODE_HPP