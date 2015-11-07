#include "rooibos/codegen-internal.hh"

using std::make_shared;
using std::shared_ptr;

using llvm::Argument;
using llvm::CallInst;
using llvm::ConstantInt;
using llvm::dyn_cast_or_null;
using llvm::Instruction;
using llvm::ReturnInst;
using llvm::Value;

namespace rooibos
{
  void
  InstCodegenVisitor::visitCallInst(CallInst & inst)
  {
    auto callee = inst.getCalledFunction();
    auto calleeName = callee->getName();
    auto calleeIdent = _idents.forFunction(calleeName);
    auto call = make_shared<CallExpressionAST>(calleeIdent);

    for(auto & arg : inst.arg_operands())
    {
      call->arguments.push_back(codegen(_idents, arg));
    }

    _emit(inst, call);
  }

  void
  InstCodegenVisitor::visitInstruction(Instruction & inst)
  {
    _impl->body.push_back(make_shared<ExpressionStatementAST>(
          make_shared<StringLiteralAST>(inst.getOpcodeName())));
  }

  void
  InstCodegenVisitor::visitReturnInst(ReturnInst & inst)
  {
    auto stmt = make_shared<ReturnStatementAST>();
    Value * value = inst.getReturnValue();
    if(value)
    {
      stmt->argument = codegen(_idents, value);
    }
    _impl->body.push_back(stmt);
  }

  void
  InstCodegenVisitor::_emit(Instruction & inst, shared_ptr<ExpressionAST> expr)
  {
    auto stmtExpr = expr;
    if(inst.hasNUsesOrMore(1))
    {
      stmtExpr = _assign(inst, coerce(inst.getType(), expr));
    }
    auto stmt = make_shared<ExpressionStatementAST>(stmtExpr);
    _impl->body.push_back(stmt);
  }

  shared_ptr<ExpressionAST>
  InstCodegenVisitor::_assign(Instruction & inst,
                              shared_ptr<ExpressionAST> expr)
  {
    auto ident = _idents.forInstruction(inst);
    auto decl = make_shared<VariableDeclarationAST>(
        ident, codegenDefaultValue(inst.getType()));
    _impl->body.insert(_impl->body.begin(), decl);
    return make_shared<AssignmentExpressionAST>(ident, expr);
  }
}
