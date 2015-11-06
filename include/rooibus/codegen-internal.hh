#include "rooibus/codegen.hh"

#include <llvm/IR/InstVisitor.h>

namespace rooibus
{
  struct Identifiers
  {
    const std::shared_ptr<IdentifierAST>
      adaptors, ASM, asm_, asmExtern, ffi, globals, heap, stdlib, this_;

    Identifiers();

    std::shared_ptr<IdentifierAST> forFunction(const std::string & name);
    std::shared_ptr<IdentifierAST> forFunctionExtern(const std::string & name);
    std::shared_ptr<IdentifierAST> forParameter(const std::string & name);
  };

  void codegen(llvm::Function & func,
               Identifiers & idents,
               std::shared_ptr<FunctionExpressionAST> asmFunc,
               std::shared_ptr<ObjectExpressionAST> asmRet,
               std::shared_ptr<ObjectExpressionAST> adaptors);

  class InstCodegenVisitor : public llvm::InstVisitor<InstCodegenVisitor>
  {
  public:
    InstCodegenVisitor(Identifiers & idents,
                       std::shared_ptr<BlockStatementAST> impl)
    : _idents(idents), _impl(impl)
    {}

    void visitCallInst(llvm::CallInst &);
    void visitInstruction(llvm::Instruction &);
    void visitReturnInst(llvm::ReturnInst &);

  private:
    Identifiers & _idents;
    std::shared_ptr<BlockStatementAST> _impl;
  };
}
