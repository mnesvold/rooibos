#ifndef ROOIBOS_CODEGEN_HH
#define ROOIBOS_CODEGEN_HH

#include <memory>
#include <set>
#include <vector>

#include <llvm/ADT/SmallSet.h>
#include <llvm/IR/Module.h>

#include "rooibos/ast.hh"
#include "rooibos/identifiers.hh"

namespace rooibos
{
  std::unique_ptr<ProgramAST> codegen(llvm::Module & module);

  struct CodegenContext
  {
    typedef llvm::SmallSet<IdentifierAST::ptr, 2> heap_set_type;

    Identifiers & idents;
    std::set<std::string> & stdlib;
    heap_set_type & heaps;
    bool & needsStackPointer;
  };

  void codegen(llvm::Function & func,
               CodegenContext & ctx,
               std::vector<StatementAST::ptr> & impls,
               ObjectExpressionAST::ptr asmRet,
               ObjectExpressionAST::ptr adaptors);

  ExpressionAST::ptr codegen(Identifiers &, llvm::Value *);

  ExpressionAST::ptr
  coerce(llvm::Type *, ExpressionAST::ptr);

  ExpressionAST::ptr coerceToDouble(ExpressionAST::ptr);
  ExpressionAST::ptr coerceToInt(ExpressionAST::ptr);

  IdentifierAST::ptr
  codegenHeapIdent(Identifiers &, const llvm::Type *);

  unsigned
  codegenHeapShift(const llvm::Type *);

  ExpressionAST::ptr
  codegenDefaultValue(const llvm::Type *);

  long
  codegenTypeSize(const llvm::Type *);
}

#endif
