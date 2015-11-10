#ifndef ROOIBOS_IDENTIFIERS_HH
#define ROOIBOS_IDENTIFIERS_HH

#include <memory>
#include <unordered_map>

#include <llvm/IR/Instruction.h>

#include "rooibos/ast.hh"

namespace rooibos
{
  struct Identifiers
  {
    const std::shared_ptr<IdentifierAST>
      adaptors, ArrayBuffer, ASM, asm_, asmExtern,
      ffi, FP,
      globals,
      heap, HEAP32,
      Int32Array,
      SP, stdlib,
      this_;

    Identifiers();

    void
    clearInstructionMap()
    {
      _instIDMap.clear();
      _nextInstID = 0;
    }

    std::shared_ptr<IdentifierAST> forFunction(const std::string & name);
    std::shared_ptr<IdentifierAST> forFunctionExtern(const std::string & name);
    std::shared_ptr<IdentifierAST> forInstruction(const llvm::Instruction & i);
    std::shared_ptr<IdentifierAST> forParameter(const std::string & name);
    std::shared_ptr<IdentifierAST> forStdlibFunc(const std::string & name);

  private:
    std::unordered_map<const llvm::Instruction *, unsigned int> _instIDMap;
    unsigned int _nextInstID = 0;
  };
}

#endif
