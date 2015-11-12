#include "rooibos/InstCodegenVisitor.hh"

#include "rooibos/codegen.hh"

using std::make_shared;
using std::shared_ptr;

using llvm::AllocaInst;
using llvm::LoadInst;
using llvm::StoreInst;

namespace rooibos
{
  namespace
  {
    template<typename T>
    ExpressionAST::ptr
    codegenHeapAccess(Identifiers & idents, T & inst)
    {
      auto unshiftedOffset = codegen(idents, inst.getPointerOperand());
      auto offset = BinaryExpressionAST::create(
          unshiftedOffset, BinaryOp::SHIFT_RIGHT,
          NumberLiteralAST::create(2));
      auto expr = SubscriptExpressionAST::create(idents.HEAP32, offset);
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
    _ctx.needsHeap32 = true;
    auto expr = codegenHeapAccess(_ctx.idents, inst);
    _emit(inst, expr);
  }

  void
  InstCodegenVisitor::visitStoreInst(StoreInst & inst)
  {
    _ctx.needsHeap32 = true;
    auto access = codegenHeapAccess(_ctx.idents, inst);
    auto expr = AssignmentExpressionAST::create(access,
        codegen(_ctx.idents, inst.getValueOperand()));
    _emit(inst, expr);
  }
}
