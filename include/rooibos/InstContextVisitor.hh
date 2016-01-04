#ifndef ROOIBOS_INSTCONTEXTVISITOR_HH
#define ROOIBOS_INSTCONTEXTVISITOR_HH

#include <llvm/IR/InstVisitor.h>

namespace rooibos
{
  class InstContextVisitor : public llvm::InstVisitor<InstContextVisitor>
  {
  public:
    InstContextVisitor()
    : _needsFP(false)
    {
    }

    bool needsFramePointer() const { return _needsFP; }

    // Memory instructions
    void visitAllocaInst(llvm::AllocaInst &);

    void visitInstruction(llvm::Instruction &); // fallback

  private:
    bool _needsFP;
  };
}

#endif
