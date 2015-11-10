#ifndef ROOIBOS_CODEGEN_HH
#define ROOIBOS_CODEGEN_HH

#include <memory>
#include <set>
#include <vector>

#include <llvm/IR/Module.h>

#include "rooibos/ast.hh"
#include "rooibos/identifiers.hh"

namespace rooibos
{
  std::unique_ptr<ProgramAST> codegen(llvm::Module & module);

  struct CodegenContext
  {
      Identifiers & idents;
      std::set<std::string> & stdlib;
      bool & needsHeap32;
  };

  void codegen(llvm::Function & func,
               CodegenContext & ctx,
               std::vector<std::shared_ptr<StatementAST>> & impls,
               std::shared_ptr<ObjectExpressionAST> asmRet,
               std::shared_ptr<ObjectExpressionAST> adaptors);

  std::shared_ptr<ExpressionAST> codegen(Identifiers &, llvm::Value *);

  std::shared_ptr<ExpressionAST>
  coerce(llvm::Type *, std::shared_ptr<ExpressionAST>);

  std::shared_ptr<ExpressionAST>
  codegenDefaultValue(const llvm::Type *);
}

#endif
