#include "rooibus/codegen-internal.hh"

using std::make_shared;
using std::shared_ptr;

using llvm::CallInst;
using llvm::Instruction;
using llvm::ReturnInst;

namespace rooibus
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
          make_shared<LiteralAST>(inst.getOpcodeName())));
  }

  void
  InstCodegenVisitor::visitReturnInst(ReturnInst & inst)
  {
    _impl->body.push_back(make_shared<ReturnStatementAST>());
  }
}
