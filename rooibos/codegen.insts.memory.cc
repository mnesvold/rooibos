#include "rooibos/codegen-internal.hh"

using std::make_shared;
using std::shared_ptr;

using llvm::LoadInst;
using llvm::StoreInst;

namespace rooibos
{
  namespace
  {
    template<typename T>
    shared_ptr<ExpressionAST>
    codegenHeapAccess(Identifiers & idents, T & inst)
    {
      auto unshiftedOffset = codegen(idents, inst.getPointerOperand());
      auto offset = make_shared<BinaryExpressionAST>(
          unshiftedOffset, BinaryOp::SHIFT_RIGHT,
          make_shared<NumberLiteralAST>(2));
      auto expr = make_shared<SubscriptExpressionAST>(idents.HEAP32, offset);
      return expr;
    }
  }

  void
  InstCodegenVisitor::visitLoadInst(LoadInst & inst)
  {
    _needsHeap32 = true;
    auto expr = codegenHeapAccess(_idents, inst);
    _emit(inst, expr);
  }

  void
  InstCodegenVisitor::visitStoreInst(StoreInst & inst)
  {
    _needsHeap32 = true;
    auto access = codegenHeapAccess(_idents, inst);
    auto expr = make_shared<AssignmentExpressionAST>(access,
        codegen(_idents, inst.getValueOperand()));
    _emit(inst, expr);
  }
}
