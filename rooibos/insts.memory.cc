#include "rooibos/InstCodegenVisitor.hh"

#include "rooibos/codegen.hh"

using std::make_shared;
using std::shared_ptr;

using llvm::AllocaInst;
using llvm::cast;
using llvm::LoadInst;
using llvm::PointerType;
using llvm::StoreInst;

namespace rooibos
{
  namespace
  {
    template<typename T, typename S>
    ExpressionAST::ptr
    codegenHeapAccess(Identifiers & idents, T & inst, S & heaps)
    {
      auto unshiftedOffset = codegen(idents, inst.getPointerOperand());
      auto offset = BinaryExpressionAST::create(
          unshiftedOffset, BinaryOp::SHIFT_RIGHT,
          NumberLiteralAST::create(2));
      auto ptrType = inst.getPointerOperand()->getType();
      auto heapType = cast<PointerType>(ptrType)->getElementType();
      auto heap = codegenHeapIdent(idents, heapType);
      heaps.insert(heap);
      auto expr = SubscriptExpressionAST::create(heap, offset);
      return expr;
    }
  }

  void
  InstCodegenVisitor::visitAllocaInst(AllocaInst & inst)
  {
    _ctx.needsStackPointer = true;
    auto FP = _ctx.idents.FP;
    auto SP = _ctx.idents.SP;
    auto coercedSP = BinaryExpressionAST::create(SP, BinaryOp::BITWISE_OR,
        NumberLiteralAST::create(0));
    auto spSub = BinaryExpressionAST::create(coercedSP, BinaryOp::SUB,
        NumberLiteralAST::create(4));
    auto coercedSPSub = BinaryExpressionAST::create(spSub,
        BinaryOp::BITWISE_OR, NumberLiteralAST::create(0));
    _stmts.push_back(ExpressionStatementAST::create(
        AssignmentExpressionAST::create(SP, coercedSPSub)));
    _emit(inst, SP);
  }

  void
  InstCodegenVisitor::visitLoadInst(LoadInst & inst)
  {
    auto expr = codegenHeapAccess(_ctx.idents, inst, _ctx.heaps);
    _emit(inst, expr);
  }

  void
  InstCodegenVisitor::visitStoreInst(StoreInst & inst)
  {
    auto access = codegenHeapAccess(_ctx.idents, inst, _ctx.heaps);
    auto expr = AssignmentExpressionAST::create(access,
        codegen(_ctx.idents, inst.getValueOperand()));
    _emit(inst, expr);
  }
}
