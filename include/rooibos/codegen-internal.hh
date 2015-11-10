#include "rooibos/codegen.hh"

#include <set>
#include <unordered_map>

#include <llvm/IR/InstVisitor.h>

namespace rooibos
{
  struct Identifiers
  {
    const std::shared_ptr<IdentifierAST>
      adaptors, ArrayBuffer, ASM, asm_, asmExtern,
      ffi, globals, heap, HEAP32, Int32Array, stdlib, this_;

    Identifiers();

    std::shared_ptr<IdentifierAST> forFunction(const std::string & name);
    std::shared_ptr<IdentifierAST> forFunctionExtern(const std::string & name);
    std::shared_ptr<IdentifierAST> forInstruction(const llvm::Instruction &);
    std::shared_ptr<IdentifierAST> forParameter(const std::string & name);
    std::shared_ptr<IdentifierAST> forStdlibFunc(const std::string & name);

  private:
    std::unordered_map<const llvm::Instruction *, unsigned int> _instIDMap;
    unsigned int _nextInstID = 0;
  };

  void codegen(llvm::Function & func,
               Identifiers & idents,
               std::set<std::string> & stdlib,
               bool & needsHeap32,
               std::vector<std::shared_ptr<StatementAST>> & impls,
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
        std::set<std::string> & stdlib,
        bool & needsHeap32,
        std::vector<std::shared_ptr<VariableDeclaratorAST>> & vars,
        std::vector<std::shared_ptr<StatementAST>> & stmts)
    : _idents(idents), _stdlib(stdlib), _needsHeap32(needsHeap32),_vars(vars),
      _stmts(stmts)
    {}

    void visitCallInst(llvm::CallInst &);
    void visitInstruction(llvm::Instruction &);
    void visitReturnInst(llvm::ReturnInst &);

    // Memory instructions
    void visitLoadInst(llvm::LoadInst &);
    void visitStoreInst(llvm::StoreInst &);

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
