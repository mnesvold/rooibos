#ifndef ROOIBOS_INSTCONTEXTVISITOR_HH
#define ROOIBOS_INSTCONTEXTVISITOR_HH

#include <llvm/IR/InstVisitor.h>

namespace rooibos
{
  class InstContextVisitor : public llvm::InstVisitor<InstContextVisitor>
  {
  public:
    InstContextVisitor(llvm::Function & func)
    : _needsFP(false), _needsPC(func.size() > 1)
    {
      visit(func);
    }

    bool needsFramePointer() const { return _needsFP; }
    bool needsProgramCounter() const { return _needsPC; }

    // Memory instructions
    void visitAllocaInst(llvm::AllocaInst &);

    void visitInstruction(llvm::Instruction &); // fallback

  private:
    bool _needsFP;
    bool _needsPC;
  };
}

#endif
