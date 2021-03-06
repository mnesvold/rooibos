#include "rooibos/InstCodegenVisitor.hh"

#include <algorithm>

#include "rooibos/codegen.hh"
#include "rooibos/util.hh"

using std::equal;
using std::make_shared;
using std::shared_ptr;
using std::string;

using llvm::Argument;
using llvm::CallInst;
using llvm::CmpInst;
using llvm::ConstantInt;
using llvm::dyn_cast_or_null;
using llvm::ICmpInst;
using llvm::Instruction;
using llvm::SelectInst;

namespace rooibos
{
  namespace
  {
    bool
    startsWith(const std::string & string, const std::string & prefix)
    {
      return equal(prefix.begin(), prefix.end(), string.begin());
    }

    bool
    isStdlibCall(std::string & name)
    {
      string prefix = "__rooibos_stdlib_";
      if(!startsWith(name, prefix))
      {
        return false;
      }
      name = name.substr(prefix.length());
      return true;
    }
  }

  void
  InstCodegenVisitor::visitICmpInst(ICmpInst & inst)
  {
    string binaryOp;
    switch(inst.getPredicate())
    {
      case CmpInst::ICMP_EQ:
        binaryOp = BinaryOp::EQ;
        break;
      case CmpInst::ICMP_NE:
        binaryOp = BinaryOp::NEQ;
        break;
      case CmpInst::ICMP_SLT:
        binaryOp = BinaryOp::LT;
        break;
      case CmpInst::ICMP_SLE:
        binaryOp = BinaryOp::LTE;
        break;
      case CmpInst::ICMP_SGT:
        binaryOp = BinaryOp::GT;
        break;
      case CmpInst::ICMP_SGE:
        binaryOp = BinaryOp::GTE;
        break;
      default:
        inst.dump();
        panic("^-- has unimplemented predicate");
    }
    auto lhs = codegen(_ctx.idents, inst.getOperand(0));
    auto rhs = codegen(_ctx.idents, inst.getOperand(1));
    auto test = BinaryExpressionAST::create(lhs, binaryOp, rhs);
    auto conditional = ConditionalExpressionAST::create(test,
        NumberLiteralAST::create(1),
        NumberLiteralAST::create(0));
    _emit(inst, conditional);
  }

  void
  InstCodegenVisitor::visitSelectInst(SelectInst & inst)
  {
    auto condition = codegen(_ctx.idents, inst.getCondition());
    auto trueVal = codegen(_ctx.idents, inst.getTrueValue());
    auto falseVal = codegen(_ctx.idents, inst.getFalseValue());
    auto conditional = ConditionalExpressionAST::create(condition,
        trueVal, falseVal);
    _emit(inst, conditional);
  }

  void
  InstCodegenVisitor::visitCallInst(CallInst & inst)
  {
    auto callee = inst.getCalledFunction();
    string calleeName = callee->getName();
    IdentifierAST::ptr calleeIdent;
    if(isStdlibCall(calleeName))
    {
      _ctx.stdlib.insert(calleeName);
      calleeIdent = _ctx.idents.forStdlibFunc(calleeName);
    }
      else
    {
      calleeIdent = _ctx.idents.forFunction(calleeName);
    }
    auto call = CallExpressionAST::create(calleeIdent);

    for(auto & arg : inst.arg_operands())
    {
      call->arguments.push_back(codegen(_ctx.idents, arg));
    }

    _emit(inst, call);
  }

  void
  InstCodegenVisitor::visitInstruction(Instruction & inst)
  {
    _stmts.push_back(ExpressionStatementAST::create(
          StringLiteralAST::create(inst.getOpcodeName())));
  }

  void
  InstCodegenVisitor::_emit(Instruction & inst, ExpressionAST::ptr expr)
  {
    auto stmtExpr = expr;
    if(inst.hasNUsesOrMore(1))
    {
      stmtExpr = _assign(inst, coerce(inst.getType(), expr));
    }
    auto stmt = ExpressionStatementAST::create(stmtExpr);
    _stmts.push_back(stmt);
  }

  ExpressionAST::ptr
  InstCodegenVisitor::_assign(Instruction & inst,
                              ExpressionAST::ptr expr)
  {
    auto ident = _ctx.idents.forInstruction(inst);
    _vars.push_back(VariableDeclaratorAST::create(
          ident, codegenDefaultValue(inst.getType())));
    return AssignmentExpressionAST::create(ident, expr);
  }
}
