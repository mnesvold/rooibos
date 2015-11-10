#ifndef ROOIBOS_INSTCODEGENVISITOR_HH
#define ROOIBOS_INSTCODEGENVISITOR_HH

#include <memory>
#include <set>
#include <string>
#include <vector>

#include <llvm/IR/InstVisitor.h>

#include "rooibos/ast.hh"
#include "rooibos/identifiers.hh"

namespace rooibos
{
  class InstCodegenVisitor : public llvm::InstVisitor<InstCodegenVisitor>
  {
  public:
    InstCodegenVisitor(Identifiers & idents,
        std::set<std::string> & stdlib,
        bool & needsHeap32,
        std::vector<std::shared_ptr<VariableDeclaratorAST>> & vars,
        std::vector<std::shared_ptr<StatementAST>> & stmts)
    : _idents(idents), _stdlib(stdlib), _needsHeap32(needsHeap32), _vars(vars),
    _stmts(stmts)
    {}

    // Terminator instructions
    void visitReturnInst(llvm::ReturnInst &);

    // Memory instructions
    void visitLoadInst(llvm::LoadInst &);
    void visitStoreInst(llvm::StoreInst &);

    // Other instructions
    void visitCallInst(llvm::CallInst &);
    void visitInstruction(llvm::Instruction &); // fallback

  private:
    Identifiers & _idents;
    std::set<std::string> & _stdlib;
    bool & _needsHeap32;
    std::vector<std::shared_ptr<VariableDeclaratorAST>> & _vars;
    std::vector<std::shared_ptr<StatementAST>> & _stmts;

    void _emit(llvm::Instruction &, std::shared_ptr<ExpressionAST>);
    std::shared_ptr<ExpressionAST>
    _assign(llvm::Instruction &, std::shared_ptr<ExpressionAST>);
  };
}

#endif
