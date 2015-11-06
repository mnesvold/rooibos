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

    _impl->body.push_back(make_shared<ExpressionStatementAST>(
          make_shared<CallExpressionAST>(calleeIdent)));
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
}
