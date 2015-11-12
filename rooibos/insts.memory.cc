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
      auto offset = make_shared<BinaryExpressionAST>(
          unshiftedOffset, BinaryOp::SHIFT_RIGHT,
          make_shared<NumberLiteralAST>(2));
      auto expr = make_shared<SubscriptExpressionAST>(idents.HEAP32, offset);
      return expr;
    }
  }

  void
  InstCodegenVisitor::visitAllocaInst(AllocaInst & inst)
  {
    _ctx.needsStackPointer = true;
    auto FP = _ctx.idents.FP;
    auto SP = _ctx.idents.SP;
    auto coercedSP = make_shared<BinaryExpressionAST>(SP, BinaryOp::BITWISE_OR,
        make_shared<NumberLiteralAST>(0));
    auto spSub = make_shared<BinaryExpressionAST>(coercedSP, BinaryOp::SUB,
        make_shared<NumberLiteralAST>(4));
    auto coercedSPSub = make_shared<BinaryExpressionAST>(spSub,
        BinaryOp::BITWISE_OR, make_shared<NumberLiteralAST>(0));
    _stmts.push_back(make_shared<ExpressionStatementAST>(
        make_shared<AssignmentExpressionAST>(SP, coercedSPSub)));
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
    auto expr = make_shared<AssignmentExpressionAST>(access,
        codegen(_ctx.idents, inst.getValueOperand()));
    _emit(inst, expr);
  }
}
