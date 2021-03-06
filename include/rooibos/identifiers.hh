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
    const IdentifierAST::ptr
      adaptors, ArrayBuffer, ASM, asm_, asmExtern,
      ffi, Float64Array, FP,
      globals,
      heap, HEAP32, HEAP64F,
      Int32Array,
      PC,
      SP, stdlib,
      this_;

    Identifiers();

    void
    clearInstructionMap()
    {
      _instIDMap.clear();
      _nextInstID = 0;
    }

    IdentifierAST::ptr forFunction(const std::string & name);
    IdentifierAST::ptr forFunctionExtern(const std::string & name);
    IdentifierAST::ptr forInstruction(const llvm::Instruction & i);
    IdentifierAST::ptr forParameter(const std::string & name);
    IdentifierAST::ptr forStdlibFunc(const std::string & name);

  private:
    std::unordered_map<const llvm::Instruction *, unsigned int> _instIDMap;
    unsigned int _nextInstID = 0;
  };
}

#endif
