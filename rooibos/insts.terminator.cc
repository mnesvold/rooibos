#include "rooibos/InstCodegenVisitor.hh"

#include "rooibos/codegen.hh"

using llvm::ReturnInst;
using llvm::Value;

namespace rooibos
{
  void
  InstCodegenVisitor::visitReturnInst(ReturnInst & inst)
  {
    auto stmt = ReturnStatementAST::create();
    Value * value = inst.getReturnValue();
    if(value)
    {
      stmt->argument = codegen(_ctx.idents, value);
    }
    _stmts.insert(_stmts.end(), _epilogue.begin(), _epilogue.end());
    _stmts.push_back(stmt);
  }
}
