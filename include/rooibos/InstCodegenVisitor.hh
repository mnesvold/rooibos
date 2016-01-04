#ifndef ROOIBOS_INSTCODEGENVISITOR_HH
#define ROOIBOS_INSTCODEGENVISITOR_HH

#include <memory>
#include <set>
#include <string>
#include <vector>

#include <llvm/IR/InstVisitor.h>

#include "rooibos/ast.hh"
#include "rooibos/codegen.hh"
#include "rooibos/identifiers.hh"

namespace rooibos
{
  class InstCodegenVisitor : public llvm::InstVisitor<InstCodegenVisitor>
  {
  public:
    InstCodegenVisitor(CodegenContext & ctx,
        std::vector<VariableDeclaratorAST::ptr> & vars,
        std::vector<StatementAST::ptr> & stmts,
        const std::vector<StatementAST::ptr> & epilogue)
    : _ctx(ctx), _vars(vars), _stmts(stmts), _epilogue(epilogue)
    {}

    // Terminator instructions
    void visitReturnInst(llvm::ReturnInst &);

    // Binary instructions
    void visitBinaryOperator(llvm::BinaryOperator &);

    // Memory instructions
    void visitAllocaInst(llvm::AllocaInst &);
    void visitLoadInst(llvm::LoadInst &);
    void visitStoreInst(llvm::StoreInst &);
    void visitGetElementPtrInst(llvm::GetElementPtrInst &);

    // Other instructions
    void visitCallInst(llvm::CallInst &);
    void visitInstruction(llvm::Instruction &); // fallback

  private:
    CodegenContext & _ctx;
    std::vector<VariableDeclaratorAST::ptr> & _vars;
    std::vector<StatementAST::ptr> & _stmts;
    const std::vector<StatementAST::ptr> & _epilogue;

    void _emit(llvm::Instruction &, ExpressionAST::ptr);
    ExpressionAST::ptr
    _assign(llvm::Instruction &, ExpressionAST::ptr);
  };
}

#endif
