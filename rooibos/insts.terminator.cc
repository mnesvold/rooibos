#include "rooibos/InstCodegenVisitor.hh"

#include "rooibos/codegen.hh"
#include "rooibos/util.hh"

using llvm::BranchInst;
using llvm::ReturnInst;
using llvm::Value;

namespace rooibos
{
  void
  InstCodegenVisitor::visitBranchInst(BranchInst & inst)
  {
    if(inst.isConditional())
    {
      inst.dump();
      panic("^-- is conditional (thus not yet supported)");
    }
    auto nextPC = _bbIndices.at(inst.getSuccessor(0));
    auto pcUpdate = ExpressionStatementAST::create(
        AssignmentExpressionAST::create(
          _ctx.idents.PC,
          coerceToInt(NumberLiteralAST::create(nextPC))));
    auto continue_stmt = ContinueStatementAST::create();

    _stmts.push_back(pcUpdate);
    _stmts.push_back(continue_stmt);
  }

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
