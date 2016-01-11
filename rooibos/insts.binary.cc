#include "rooibos/InstCodegenVisitor.hh"

#include <llvm/IR/Instruction.h>

#include "rooibos/codegen.hh"
#include "rooibos/util.hh"

using llvm::BinaryOperator;
using llvm::Instruction;

namespace rooibos
{
  namespace
  {
    std::string
    _genOp(const BinaryOperator & inst)
    {
      switch(inst.getOpcode())
      {
        case Instruction::BinaryOps::FAdd:
          return BinaryOp::ADD;
        case Instruction::BinaryOps::FMul:
          return BinaryOp::MUL;
        case Instruction::BinaryOps::FSub:
          return BinaryOp::SUB;
        case Instruction::BinaryOps::FDiv:
          return BinaryOp::DIV;
        case Instruction::BinaryOps::FRem:
          return BinaryOp::MOD;
        default:
          inst.dump();
          panic("^-- has un-codegen-able operation");
      }
    }
  }

  void
  InstCodegenVisitor::visitBinaryOperator(BinaryOperator & inst)
  {
    auto op = _genOp(inst);
    auto lhs = codegen(_ctx.idents, inst.getOperand(0));
    auto rhs = codegen(_ctx.idents, inst.getOperand(1));
    auto stmt = BinaryExpressionAST::create(lhs, op, rhs);
    _emit(inst, stmt);
  }
}
