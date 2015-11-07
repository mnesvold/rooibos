#include "rooibos/codegen.hh"

#include <unordered_map>

#include <llvm/IR/InstVisitor.h>

namespace rooibos
{
  struct Identifiers
  {
    const std::shared_ptr<IdentifierAST>
      adaptors, ASM, asm_, asmExtern, ffi, globals, heap, stdlib, this_;

    Identifiers();

    std::shared_ptr<IdentifierAST> forFunction(const std::string & name);
    std::shared_ptr<IdentifierAST> forFunctionExtern(const std::string & name);
    std::shared_ptr<IdentifierAST> forInstruction(const llvm::Instruction &);
    std::shared_ptr<IdentifierAST> forParameter(const std::string & name);

  private:
    std::unordered_map<const llvm::Instruction *, unsigned int> _instIDMap;
    unsigned int _nextInstID = 0;
  };

  void codegen(llvm::Function & func,
               Identifiers & idents,
               std::shared_ptr<FunctionExpressionAST> asmFunc,
               std::shared_ptr<ObjectExpressionAST> asmRet,
               std::shared_ptr<ObjectExpressionAST> adaptors);

  std::shared_ptr<ExpressionAST> codegen(Identifiers &, llvm::Value *);

  std::shared_ptr<ExpressionAST>
  coerce(llvm::Type *, std::shared_ptr<ExpressionAST>);

  std::shared_ptr<ExpressionAST>
  codegenDefaultValue(const llvm::Type *);

  class InstCodegenVisitor : public llvm::InstVisitor<InstCodegenVisitor>
  {
  public:
    InstCodegenVisitor(Identifiers & idents,
                       std::vector<std::shared_ptr<StatementAST>> & stmts)
    : _idents(idents), _stmts(stmts)
    {}

    void visitCallInst(llvm::CallInst &);
    void visitInstruction(llvm::Instruction &);
    void visitReturnInst(llvm::ReturnInst &);

  private:
    Identifiers & _idents;
    std::vector<std::shared_ptr<StatementAST>> & _stmts;

    void _emit(llvm::Instruction &, std::shared_ptr<ExpressionAST>);
    std::shared_ptr<ExpressionAST>
    _assign(llvm::Instruction &, std::shared_ptr<ExpressionAST>);
  };
}
