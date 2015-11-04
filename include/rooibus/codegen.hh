#ifndef ROOIBUS_CODEGEN_HH
#define ROOIBUS_CODEGEN_HH

#include <memory>

#include <llvm/IR/Module.h>

#include "rooibus/ast.hh"

namespace rooibus
{
  std::unique_ptr<ProgramAST> codegen(llvm::Module & module);
}

#endif
