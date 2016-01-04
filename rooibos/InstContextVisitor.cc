#include "rooibos/InstContextVisitor.hh"

using llvm::Instruction;

namespace rooibos
{
  void
  InstContextVisitor::visitAllocaInst(llvm::AllocaInst &)
  {
    _needsFP = true;
  }

  void
  InstContextVisitor::visitInstruction(llvm::Instruction & inst)
  {
  }
}
