#ifndef ROOIBUS_CODEGEN_HH
#define ROOIBUS_CODEGEN_HH

#include <memory>

#include <llvm/IR/Module.h>

#include "rooibos/ast.hh"

namespace rooibos
{
  std::unique_ptr<ProgramAST> codegen(llvm::Module & module);
}

#endif
